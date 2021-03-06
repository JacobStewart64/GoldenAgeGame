#include "stdafx.h"
#include <GoldenAge/debug.h>
#include <GoldenAge/cryptinfo.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/secretkey.h>
#include <GoldenAge/toon.h>

#define MAXCLIENTS 1000
#define MAXCHANNELS 2

irr::IrrlichtDevice* device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
irr::scene::ISceneManager* smgr = device->getSceneManager();
unsigned int runloop = 1;

enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};

class addrwrap {
public:
	ENetAddress addr;

	addrwrap() {}

	addrwrap(ENetAddress& addr)
	{
		this->addr = addr;
	}

	bool operator==(const addrwrap& rhs)const 
	{
		return (addr.host == rhs.addr.host && addr.port == rhs.addr.port);
	}
};

struct account {
	std::vector<ga::toon> toons;
	std::string name;

	account() {}

	account(std::string name)
	{
		this->name = name;
		setupAccountToons();
	}

	void setupAccountToons()
	{
		debug("setting up account toons (for now just toongraphic stuff for character select)");
		std::ifstream intoons("./Accounts/" + name + ".txt");
		for (std::string line; std::getline(intoons, line);)
		{
			debug("opening file ./Accounts/toons/", line, ".txt");
			std::ifstream toonfile("./Accounts/toons/" + line + ".txt");
			ga::toon loadedtoon(line);
			toonfile >> loadedtoon;
			toons.push_back(loadedtoon);
			toonfile.close();
		}
		intoons.close();
		debug("setup account toons done");
	}
};

struct client {
	ga::cryptinfo ci;
	account acc;
	ga::secretkey secret_key;
	ga::toon* current_toon;

	client() {}

	client(std::string name)
	{
		this->acc = account(name);
	}
};

//don't put non-shared code in namespace ga so u can do things like ga::toon toon definition lol
struct server_toon {
	ga::toon toon;
	irr::scene::ISceneNode* node;

	server_toon(ga::toon& toon) : toon(toon), node(0) {}

	void init_position()
	{
		node = smgr->addEmptySceneNode();
		node->setPosition(toon.vpos);
	}
};

struct AddrHash
{
	std::size_t operator()(addrwrap const& addr) const noexcept
	{
		std::size_t h1 = std::hash<unsigned int>{}(addr.addr.host);
		std::size_t h2 = std::hash<unsigned short>{}(addr.addr.port);
		return h1 ^ (h2 << 1);
	}
};

irr::scene::ITriangleSelector* selector = 0;
std::string areamap[12][12] = {
	{ "../GAClient/system/resources/3dart/terrain-heightmap.bmp" }
};

std::unordered_map<std::string, client> clientsmap; //clients ready to be logged in, account name key
std::unordered_map<addrwrap, client, AddrHash> addrmap; //logged in clients accessed directly with address
std::unordered_map<addrwrap, server_toon, AddrHash> server_toon_map;

void load_area(ga::world_position pos)
{
	// add terrain scene node
	irr::scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(areamap[pos.row][pos.col].c_str());
	if (!terrain)
	{
		debug("terrain was null");
		system("pause");
	}
	// create triangle selector for the terrain 
	selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
}

//clients will have collision for all new areas loaded...
void add_ground_collision(irr::scene::ISceneNode* node)
{
	if (selector)
	{
		irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, node, irr::core::vector3df(30, 50, 30),
			irr::core::vector3df(0, -10, 0), irr::core::vector3df(0, 30, 0));
		node->addAnimator(anim);
		anim->drop();  // And likewise, drop the animator when we're done referring to it.
	}
}


int main()
{
	debug("checking device");
	if (device == 0)
	{
		debug("Failed to create device.");
		system("pause");
		return 1; // could not create selected driver.
	}

	debug("checking smgr");
	if (smgr == 0)
	{
		debug("Failed to create scene manager!");
		system("pause");
		return 1;
	}

	ga::udp_com com(MAXCLIENTS, MAXCHANNELS, 3001, true);

	com.register_receive([&com](ENetEvent& e) {
		debug("My receive handler");
		ga::array_packet packet;
		packet.from_buf((const char*)e.packet->data, e.packet->dataLength);
		std::string type(packet.get());
		if (type == "c")
		{
			debug("received a login preparation packet");
			std::string key(packet.get(), 0, 32);
			std::string iv(packet.get(), 0, 16);
			std::string sk(packet.get(), 0, 256);
			std::string name(packet.get(), 0, 16);
			client cli(name);
			cli.ci.keyFromString(key);
			cli.ci.ivFromString(iv);
			cli.secret_key.from_string(sk);
			debug("making an entry in the clientsmap");
			clientsmap.insert(std::make_pair(name, cli));
		}
		else if (type == "a")
		{
			debug("receive a login request from client, checking their ip and seeing if they are awaiting login, and checking their secret login key");
			std::string accname = packet.get();
			if (clientsmap.find(accname) != clientsmap.end())
			{
				debug("they were awaiting!");
				client& cli = clientsmap.at(accname);
				
				std::string skstr(packet.get(), 0, 256);

				debug("checking the secret key");
				if (cli.secret_key.to_string() == skstr)
				{
					debug("authorization successful, hope it's secure lol");
					debug("telling the client");
					debug("sending all their toon graphics lol");
					std::string type("f");
					std::string size = std::to_string(cli.acc.toons.size());
					ga::array_packet pack;
					pack.fill(type, size);
					for (ga::toon t : cli.acc.toons)
					{
						pack.add(t.name);
						pack.add(t.tg.to_string());
					}
					com.send(e.peer, pack(), pack.size(), ENET_PACKET_FLAG_RELIABLE);
					
					debug("adding client to logged in map");
					addrmap.insert(std::make_pair(addrwrap(e.peer->address), cli));
				}
				else
				{
					debug("they gave the wrong secret key, what's going on?");
				}
			}
			else
			{
				debug("the login server hasn't even told me about this dude");
			}
		}
		else if (type == "b")
		{
			debug("got a spawn packet");
			client& cli = addrmap.at(e.peer->address);
			std::string ptype("s");
			cli.current_toon = &cli.acc.toons[std::stoi(packet.get())];
			std::string toondata = cli.current_toon->to_string();
			ga::array_packet pack(ga::array_packet::get_args_size(ptype, toondata));
			pack.fill(ptype, toondata);
			com.send(e.peer, pack(), pack.size(), ENET_PACKET_FLAG_RELIABLE);

			//add a server toon to the "active toons map"
			//erase this account from the logged in clients map
			//now they are but a toon
			//when they "back out" to their character select, just reset to beginning
			//take alternate path where just send packet to login with previous account and password
			//then the right screen will appear, ci get redone, game server ram saved from making data held less long
			//traffic increase in every aspect though... once every 15 min? who care
			//so we can erase here, much better even
			server_toon st(*cli.current_toon);
			debug("init position");
			st.init_position();
			debug("load area");
			load_area(st.toon.pos);
			debug("add ground collision");
			add_ground_collision(st.node);

			server_toon_map.insert(std::make_pair(addrwrap(e.peer->address), st));
			//erase them later! because it didn't work before lol
		}
		else if (type == "g")
		{
			debug("got a create toon packet");

			//add a toon name to their account file
			//add a toon file for the toon name
			//put the data structures in the addrmap so we can select them this session
			debug("creating toon server side...");
			client& cli = addrmap.at(e.peer->address);
			ga::toon toon(true);
			toon.name = packet.get();

			debug("appending toon name to account file");
			std::ofstream accountfile("./Accounts/" + cli.acc.name + ".txt", std::ios::app);
			accountfile << toon.name << "\n";
			accountfile.close();

			debug("writing new default toon to toon file");
			std::ofstream toonfile("./Accounts/toons/" + toon.name + ".txt");
			toonfile << toon << "\n";
			toonfile.close();

			debug("adding the new toon to the addrmap");
			cli.acc.toons.push_back(std::move(toon));

			debug("telling the client we created it!");
			std::string typ("p");
			ga::array_packet pack(ga::array_packet::get_args_size(typ));
			pack.fill(typ);
			com.send(e.peer, pack(), pack.size(), ENET_PACKET_FLAG_RELIABLE);
		}
		else
		{
			debug("received unkown packet type");
		}
	});

	com.register_connect([](ENetPeer* peer) {
		debug("My connect handler");				
	});

	com.register_disconnect([](ENetPeer* e) {
		debug("My disconnect handler");
	});

	com.register_delta([](unsigned int last_time)->unsigned int {
		unsigned int current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		return current_time - last_time;
	});

	com.register_tick([](unsigned int delta_time) {
		device->run();
		//update toons
		//do tasks
	});

	unsigned int current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	com.listenLoop(runloop, current_time);
	debug("exiting");
	system("pause");
	return 0;
}