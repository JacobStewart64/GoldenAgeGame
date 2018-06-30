#include "stdafx.h"
#include <GoldenAge/Debug.h>
#include <GoldenAge/cryptinfo.h>
#include <GoldenAge/winsockwrapper.h>

struct gameserver {
	std::string host;
	unsigned int port;

	gameserver() {}

	gameserver(std::string host, unsigned int port)
	{
		debug("constructing gameserver data");
		this->host = host;
		this->port = port;
		debug("constructing gameserver data good");
	}

private:
	friend std::ostream& operator<<(std::ostream &os, const  gameserver  &e);
	friend std::istream& operator>>(std::istream &os, gameserver  &e);
};

std::ostream& operator<<(std::ostream &os, const  gameserver  &e)
{
	debug("writing gameserver data to stream");
	debug("writing host to stream");
	os << e.host << "\n";
	debug("writing port to stream");
	os << e.port;
	debug("writing gameserver data to stream good");
	return os;
}

std::istream& operator>>(std::istream &os, gameserver  &e)
{
	debug("reading gameserver data from stream");
	debug("reading host from stream");
	os >> e.host;
	debug("reading port from stream");
	os >> e.port;
	debug("reading gameserver data from stream good");
	return os;
}

struct sessiondata {
	std::string email;
	std::string password;
	std::string gameservername;
	cryptinfo ci;
	httplib::Response* res;

	bool loggedin = false;
	bool awaitinggamelogin = false;

	void fillinsd(std::string email, std::string password, std::string gameservername)
	{
		debug("filling in session data\n", email, "\n", password, "\n", gameservername);
		this->email = email;
		this->password = password;
		this->gameservername = gameservername;
		debug("filling in session data good");
	}
};

std::unordered_map<std::string, sessiondata> accounts;
std::unordered_map<std::string, gameserver> gameservermap;

//takes char email[100] and char pass[100], never overwrites the buffer
//but seems huge. at least it handles large emails and passwords
void getEmailPasswordGameServer(char* email, char* pass, char* gameserver, const httplib::Request& req)
{
	debug("reading email and password from request");
	int i = 0;
	for (; i < 21; ++i)
	{
		if (req.body[i] == ' ')
		{
			email[i] = '\0';
			++i;
			break;
		}
		else {
			email[i] = req.body[i];
		}
	}
	debug(email);
	for (int j = 0; i < 42; ++i, ++j)
	{
		if (req.body[i] == ' ')
		{
			pass[j] = '\0';
			++i;
			break;
		}
		else {
			pass[j] = req.body[i];
		}
	}
	debug(pass);
	for (int k = 0; i < 63; ++k, ++i)
	{
		if (req.body[i] == '\0')
		{
			gameserver[k] = '\0';
			//++i;
			break;
		}
		else {
			gameserver[k] = req.body[i];
		}
	}
	debug(gameserver);
	debug("reading email and password from request good");
}

void getAccountFromReq(sessiondata& sd, const httplib::Request& req)
{
	debug("getting session data from request");
	char email[LOGINBUFFERMAX];
	char pass[LOGINBUFFERMAX];
	char gameserver[LOGINBUFFERMAX];
	getEmailPasswordGameServer(email, pass, gameserver, req);
	sd.fillinsd(email, pass, gameserver);
	debug("getting session data from request good");
}

void initAccountsMap()
{
	debug("initializing accounts map");
	debug("opening file ", ACCOUNTMANIFESTLOCATION);
	std::ifstream accountmanifest(ACCOUNTMANIFESTLOCATION);
	for (std::string line = ""; std::getline(accountmanifest, line);)
	{
		debug("opening ", "./Accounts/" + line + ".txt");
		std::ifstream acc("./Accounts/" + line + ".txt");
		std::string p, gsn;
		acc >> p;
		acc >> gsn;
		sessiondata newsession;
		newsession.fillinsd(line, p, gsn);
		debug("inserting into accounts map a newsession at ", line);
		accounts.insert(make_pair(line, newsession));
		debug("closing file");
		acc.close();
	}
	debug("closing file ", ACCOUNTMANIFESTLOCATION);
	accountmanifest.close();
	debug("intializing accounts map good");
}

void initGameServerMap()
{
	debug("initializing game server map");
	debug("opening file ", GAMESERVERMANIFESTLOCATION);
	std::ifstream gameservermanifest(GAMESERVERMANIFESTLOCATION);
	for (std::string line = ""; std::getline(gameservermanifest, line);)
	{
		debug("opening ", "./GameServers/" + line + ".txt");
		std::ifstream gs("./GameServers/" + line + ".txt");
		gameserver newgs;
		gs >> newgs;
		debug(newgs);
		debug("inserting into gameservermap a gameserver data at ", line);
		gameservermap.insert(std::make_pair(line, newgs));
		debug("closing file");
		gs.close();
	}
	debug("closing file ", GAMESERVERMANIFESTLOCATION);
	gameservermanifest.close();
	debug("initializing game server map good");
}

void appendEmailToManifest(std::string email)
{
	debug("appending account ", email, " to manifest");
	debug("opening file ", ACCOUNTMANIFESTLOCATION);
	std::ofstream manifest(ACCOUNTMANIFESTLOCATION, std::ios::app);
	debug("writing account name to file");
	manifest << email << "\n";
	debug("closing file");
	manifest.close();
	debug("appending account to manifest good");
}

void writeAccountFile(cryptinfo& ci, sessiondata& data)
{
	debug("writing sessiondata info to account file of ", data.email);
	std::string encryptedpassword;
	ci.encrypt(data.password, encryptedpassword);
	debug("writing password [", encryptedpassword, "] to file ./Accounts/",	data.email,	".txt");
	debug("opening file ", "./Accounts/" + data.email + ".txt");
	std::ofstream accountfile("./Accounts/" + data.email + ".txt");
	
	accountfile << encryptedpassword << "\n";
	accountfile << data.gameservername << "\n";

	debug("updating the sessiondata to the new encrypted password");
	data.password = encryptedpassword;

	debug("closing file");
	accountfile.close();
	debug("writing sessiondata to file good");
}

void readInPassCryptInfo(cryptinfo& ci)
{
	debug("reading in the password encryption key and iv");
	debug("reading in file", PASSWORDKEYLOCATION);
	std::ifstream inpass(PASSWORDKEYLOCATION);
	inpass >> ci;
	debug("closing file");
	inpass.close();
	debug("key and iv: ", ci);
}

void serverLoop(ENetHost* client)
{
	debug("entering server loop");
	while (true)
	{
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case _ENetEventType::ENET_EVENT_TYPE_CONNECT:
				debug("A new client connected from ", event.peer->address.host, ":", event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = (void*)"Client information";
				break;
			case _ENetEventType::ENET_EVENT_TYPE_RECEIVE:
				debug("A packet of length ", event.packet->dataLength, " containing ", event.packet->data, " was received from ", event.peer->data, " on channel ", event.peer->data, ".\n");
				debug("handling packet");

				debug("packet handled");

				debug("destroying packet");
				enet_packet_destroy(event.packet);
				debug("destroying packet good");
				break;

			case _ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
				debug(event.peer->data, " disconnected");
				debug("null their data");
				event.peer->data = NULL;
			}
		}
	}
	debug("leaving server loop");
}

int main()
{
	debug("size of cryptinfo ", sizeof(cryptinfo));
	debug("load crypto error strings");
	ERR_load_crypto_strings();

	initAccountsMap();
	initGameServerMap();
	
	initENet();
	ENetEvent event;
	ENetHost* client = makeClient(1, 2);
	ENetAddress address;
	ENetPeer *peer;
	enet_address_set_host(&address, "localhost");
	address.port = 3001;
	debug("connect to game server...");
	peer = enet_host_connect(client, &address, 2, 0);
	if (peer == NULL)
	{
		debug("No available peers for initiating an ENet connection.\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		debug("Connection to some.server.net:1234 succeeded.");
	}
	else
	{
		debug("nulling peer data");
		enet_peer_reset(peer);
		debug("Connection to localhost:1234 failed.");
	}
	debug("setting up password cryptinfo");
	cryptinfo passci;
	readInPassCryptInfo(passci);


	debug("Creating the server, passing it the cert and private key.");	
	httplib::SSLServer server(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);

	debug("checking server");
	if (!server.is_valid()) {
		debug("Error creating server, shutting down...\n\n\t");
		system("pause");
		return -1;
	}
	debug("server good");

	debug("creating post method /login");
	server.Post("/login", [&passci, client](const httplib::Request& req, httplib::Response& res) {
		debug("got a login post");
		debug(req.body);

		sessiondata sessdata;
		getAccountFromReq(sessdata, req);

		debug("search for email ", sessdata.email, " in account map");
		if (accounts.find(sessdata.email) != accounts.end())
		{
			debug("account exists");
			debug("comparing passwords and checking bloggedin");
			sessiondata& sessd = accounts.at(sessdata.email);
			std::string decryptedpass;
			passci.decrypt(sessd.password, decryptedpass);
			if ((sessdata.password == decryptedpass) && (!sessd.loggedin))
			{
				sessd.loggedin = true;
				debug("password matched");
				cryptinfo ci;
				ci.fillkeyiv();
				debug("filled crypt info ", ci);
				char buffer[70];
				int i = 0;
				buffer[i] = 'c'; // c - client login
				++i;
				(buffer + i) << ci;
				i += 48;
				for (int j = 0; j < sessd.email.size(); ++i, ++j)
				{
					buffer[i] = sessd.email[j];
				}
				buffer[i] = '\0';
				
				gameserver& gs = gameservermap.at(sessd.gameservername);
				sessd.awaitinggamelogin = true;
				//loop and read the socket right here, these posts should spawn a thread that does dis
				
				debug("inform the client of success");
				res.set_content(" ", "text/plain");
			}
			else {
				//they got the password wrong
				debug("password did not match or they are already logged in");
					
				debug("inform the client of failure");
				res.set_content("", "text/plain");
			}
		}
		else
		{
			debug("There is no account with that email");
			
			debug("inform the client of failure");
			res.set_content("", "text/plain");
		}
		debug("login post handled");
	});
	
	debug("creating post method /createaccount");
	server.Post("/createaccount", [&passci](const httplib::Request& req, httplib::Response& res) {
		//just create an account, button to login will be different
		debug("got a createaccount post");
		debug(req.body);
		
		//get the name and the password out
		sessiondata sessdata;
		getAccountFromReq(sessdata, req);

		//if account is not in accounts
		debug("finding ", sessdata.email, " in accounts map");
		if (accounts.find(sessdata.email) == accounts.end())
		{
			debug("account was not already in accounts map");
			appendEmailToManifest(sessdata.email);
			writeAccountFile(passci, sessdata);
			accounts.insert(make_pair(sessdata.email, sessdata));

			debug("inform the client of success");
			res.set_content(" ", "text/plain");
		}
		else
		{
			debug("account was in accounts map already");

			debug("inform the client of failure");
			res.set_content("", "text/plain");
		}
		debug("create account good");
	});

	debug("Https server listening at https://localhost:1234\nRunning in its own thread");
	std::thread loginserverthread([&server]() {
		server.listen("localhost", 1234);
	});

	debug("login server listening at localhost:3002");
	serverLoop(client);

	debug("joining https server thread");
	loginserverthread.join();
	debug("joining thread good");

	debug("program exiting\n");
	system("pause");
    return 0;
}