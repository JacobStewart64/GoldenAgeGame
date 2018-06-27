// GoldenAge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../GoldenAgeGameServer/winsockwrapper.hpp"
#include "cryptinfo.hpp"

using namespace std;

struct gameserver {
	string host;
	unsigned int port;

	gameserver() {}

	gameserver(string host, unsigned int port)
	{
		this->host = host;
		this->port = port;
	}

private:
	friend std::ostream& operator<<(std::ostream &os, const  gameserver  &e);
	friend std::istream& operator>>(std::istream &os, gameserver  &e);
};

std::ostream& operator<<(std::ostream &os, const  gameserver  &e)
{
	os << e.host << "\n";
	os << e.port;
	return os;
}

std::istream& operator>>(std::istream &os, gameserver  &e)
{
	os >> e.host;
	os >> e.port;
	return os;
}

struct sessiondata {
	string email;
	string password;
	string gameservername;
	cryptinfo ci;

	bool loggedin = false;
	bool awaitinggamelogin = false;

	void fillinsd(string email, string password, string gameservername)
	{
		this->email = email;
		this->password = password;
		this->gameservername = gameservername;
	}
};

unordered_map<string, sessiondata> accounts;
unordered_map<string, gameserver> gameservermap;

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
}

void getAccountFromReq(sessiondata& sd, const httplib::Request& req)
{
	debug("making email and password structure");
	char email[LOGINBUFFERMAX];
	char pass[LOGINBUFFERMAX];
	char gameserver[LOGINBUFFERMAX];
	getEmailPasswordGameServer(email, pass, gameserver, req);
	sd.fillinsd(email, pass, gameserver);
}

void initAccountsMap()
{
	debug("initializing accounts map");
	ifstream accountmanifest(ACCOUNTMANIFESTLOCATION);
	for (string line = ""; getline(accountmanifest, line);)
	{
		debug("opening ", "./Accounts/" + line + ".txt");
		ifstream acc("./Accounts/" + line + ".txt");
		string p, gsn;
		acc >> p;
		acc >> gsn;
		sessiondata newsession;
		newsession.fillinsd(line, p, gsn);
		accounts.insert(make_pair(line, newsession));
		acc.close();
	}
	accountmanifest.close();
}

void initGameServerMap()
{
	debug("initializing game server map");
	ifstream gameservermanifest(GAMESERVERMANIFESTLOCATION);
	for (string line = ""; getline(gameservermanifest, line);)
	{
		debug("opening ", "./GameServers/" + line + ".txt");
		ifstream gs("./GameServers/" + line + ".txt");
		gameserver newgs;
		gs >> newgs;
		debug(newgs);
		gameservermap.insert(make_pair(line, newgs));
		gs.close();
	}
	gameservermanifest.close();
}

void appendEmailToManifest(string email)
{
	debug("appending email to manifest");
	ofstream manifest(ACCOUNTMANIFESTLOCATION, ios::app);
	manifest << email << "\n";
	manifest.close();
}

void writeAccountFile(cryptinfo& ci, sessiondata& data)
{
	string encryptedpassword;
	ci.encrypt(data.password, encryptedpassword);
	debug("writing password [",
		encryptedpassword,
		"] to file ./Accounts/",
		data.email,
		".txt");
	ofstream accountfile("./Accounts/" + data.email + ".txt");
	
	accountfile << encryptedpassword << "\n";
	accountfile << data.gameservername << "\n";

	data.password = encryptedpassword;

	accountfile.close();
}

void readInPassCryptInfo(cryptinfo& ci)
{
	debug("reading in file", PASSWORDKEYLOCATION);
	ifstream inpass(PASSWORDKEYLOCATION);
	inpass >> ci;
	inpass.close();
	debug("on in: ", ci);
}

void serverLoop(SOCKET& socketS, sockaddr_in& from, int* fromlen)
{
	while (true)
	{
		char buffer[1024];
		ZeroMemory(buffer, sizeof(buffer));
		if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, fromlen) != SOCKET_ERROR)
		{
			printf("Received message from %s: %s\n", inet_ntoa(from.sin_addr), buffer);
			sendto(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, *fromlen);
		}
	}
	closesocket(socketS);
}

int main()
{
	debug("size of cryptinfo ", sizeof(cryptinfo));
	ERR_load_crypto_strings();

	initAccountsMap();
	initGameServerMap();
	SOCKET socketS;
	struct sockaddr_in local;
	struct sockaddr_in from;
	initWinsock();
	int fromlen = initSocketAndSockAddr(socketS, local, 3002, true);
	debug("setting up password cryptinfo");
	cryptinfo passci;
	readInPassCryptInfo(passci);


	debug("Creating the server, passing it the cert and private key.");	
	httplib::SSLServer server(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);

	if (!server.is_valid()) {
		debug("Error creating server, shutting down...\n\n\t");
		system("pause");
		return -1;
	}

	//REST API - AUTH A USER, ESTABLISH CRYPTO SHIT, TELL GAME SERVER,
	//GET TOLD BY GAME SERVER WHEN TO TELL CLIENT, TELL CLIENT
	//ALSO - CREATE ACCOUNT (MAKE FIRST)
	server.Post("/login", [&passci, &socketS](const httplib::Request& req, httplib::Response& res) {
		//login
		debug("got a login post");
		debug(req.body);
		sessiondata sessdata;
		getAccountFromReq(sessdata, req);

		debug("search for email ", sessdata.email, " in account map");
		//if file exists
		if (accounts.find(sessdata.email) != accounts.end())
		{
			//open the file and compare the passwords
			debug("account exists");
			debug("comparing passwords and checking bloggedin");
			sessiondata& sessd = accounts.at(sessdata.email);
			string decryptedpass;
			passci.decrypt(sessd.password, decryptedpass);
			if ((sessdata.password == decryptedpass) && (!sessd.loggedin))
			{
				sessd.loggedin = true;
				debug("password matched");
				cryptinfo ci;
				ci.fillkeyiv();
				debug("filled crypt info ", ci);
				stringstream ss;
				ss << "c" << "\n"; // c - client login
				ss << ci << "\n";
				gameserver& gs = gameservermap.at(sessd.gameservername);
				ss << sessd.email << "\n";
				debug("is this the correct data?", ss.str());
				sockaddr_in gameserv;
				fillinsockaddr(gameserv, gs.port, gs.host.c_str());
				sendto(socketS, ss.str().c_str(), ss.str().size(), 0, (SOCKADDR*)&gameserv, sizeof(gameserv));
				sessd.awaitinggamelogin = true;

				//inform the client of success
				res.set_content(ss.str().c_str(), "text/plain");
			}
			else {
				//they got the password wrong
				debug("password did not match or they are already logged in");
					
				//inform the client of failure
				res.set_content("", "text/plain");
			}
		}
		else
		{
			debug("There is no account with that email");
			
			//inform the client of failure
			res.set_content("", "text/plain");
		}
	});
	
	server.Post("/createaccount", [&passci](const httplib::Request& req, httplib::Response& res) {
		//just create an account, button to login will be different
		debug("got a createaccount post");
		debug(req.body);
		
		//get the name and the password out
		sessiondata sessdata;
		getAccountFromReq(sessdata, req);

		//if account is not in accounts
		debug("finding ", sessdata.email);
		if (accounts.find(sessdata.email) == accounts.end())
		{
			debug("account was not already in accounts map");
			appendEmailToManifest(sessdata.email);
			writeAccountFile(passci, sessdata);
			accounts.insert(make_pair(sessdata.email, sessdata));

			//inform the client of success
			res.set_content(" ", "text/plain");
		}
		else
		{
			debug("account was in accounts map already");

			//inform the client of failure
			res.set_content("", "text/plain");
		}
	});

	debug("Https server listening at https://localhost:1234");
	thread loginserverthread([&server]() {
		server.listen("localhost", 1234);
	});

	debug("login server listening at localhost:3002");
	serverLoop(socketS, from, &fromlen);

	loginserverthread.join();
	
	debug("program exiting\n");
	system("pause");
    return 0;
}

//make it forkable!