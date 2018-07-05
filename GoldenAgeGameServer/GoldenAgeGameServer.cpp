#include "stdafx.h"
#include <GoldenAge/debug.h>
#include <GoldenAge/cryptinfo.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/toongraphics.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/secretkey.h>
#include <GoldenAge/toon.h>

#define MAXCLIENTS 1000
#define MAXCHANNELS 2

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
			toonfile >> loadedtoon.tg;
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
	bool authed = false;

	client() {}

	client(std::string name)
	{
		this->acc = account(name);
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

unsigned int runloop = 1;
std::unordered_map<std::string, client> clientsmap; //clients ready to be logged in, account name key
std::unordered_map<addrwrap, client, AddrHash> addrmap; //logged in clients accessed directly with address

int main()
{
	ga::udp_com com(MAXCLIENTS, MAXCHANNELS, 3001, true);

	com.register_receive([&com](ENetEvent& e) {
		debug("My receive handler");
		ga::array_packet packet;
		packet.from_buf((const char*)e.packet->data, e.packet->dataLength);
		std::string type = packet.get();
		if (type == "c")
		{
			debug("received a login preparation packet");
			std::string key = packet.get();
			std::string iv = packet.get();
			std::string sk = packet.get();
			std::string name = packet.get();
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
				
				debug("checking the secret key");
				std::string secret_key = packet.get();
				if (cli.secret_key.to_string() == secret_key)
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
		else
		{
			debug("received unkown packet type");
		}
	});

	com.register_connect([](ENetPeer* peer) {
		debug("My connect handler");
		//check if their ip is the same as last time
		client cli();
				
	});

	com.register_disconnect([](ENetPeer* e) {
		debug("My disconnect handler");
	});

	com.register_delta([](unsigned int last_time)->unsigned int {
		unsigned int current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		return current_time - last_time;
	});

	com.register_tick([](unsigned int delta_time) {
		//update toons
		//do tasks
	});

	unsigned int current_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	com.listenLoop(runloop, current_time);
	debug("exiting");
	system("pause");
	return 0;
}