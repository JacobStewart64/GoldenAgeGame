// GoldenAge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../GoldenAgeGameServer/winsockwrapper.hpp"


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

struct cryptinfo {
	uint8_t key[32];
	uint8_t iv[AES_BLOCK_SIZE];
	EVP_CIPHER_CTX *ectx;
	EVP_CIPHER_CTX *dctx;

private:
	friend std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e);
	friend std::istream& operator>>(std::istream &os, cryptinfo  &e);
};

std::ostream& operator<<(std::ostream &os, const  cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os << e.key[i];
	}
	os << "\n";
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
	{
		os << e.iv[i];
	}
	os << "\n";

	return os;
}

std::istream& operator>>(std::istream &os, cryptinfo  &e)
{
	for (int i = 0; i < 32; i++)
	{
		os >> e.key[i];
	}
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
	{
		os >> e.iv[i];
	}
	return os;
}

struct sessiondata {
	string email;
	string password;
	string gameservername;
	cryptinfo ci;

	bool loggedin = false;

	void fillinsd(string email, string password, string gameservername)
	{
		this->email = email;
		this->password = password;
		this->gameservername = gameservername;
	}
};


void createNewRandKeyIV(cryptinfo& ci)
{
	debug("creating new key and iv for a cryptinfo");
	RAND_bytes(ci.key, sizeof(ci.key));
	RAND_bytes(ci.iv, sizeof(ci.iv));
}

void handleErrors(void)
{
	unsigned long errCode;
	debug("An error has occured in encryption/decryption");
	while (errCode = ERR_get_error())
		debug(ERR_error_string(errCode, NULL));		
	abort();
}

void getNewEncryptParams(cryptinfo& ci)
{
	debug("getting new encryption params");
	debug("making new ctx, pass me a unfilled one");
	if (!(ci.ectx = EVP_CIPHER_CTX_new())) handleErrors();
	debug("initializing the encryption operation");
	if (1 != EVP_EncryptInit_ex(ci.ectx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
	debug("initialize key and IV", "\nbefore: ", ci.key, " ", ci.iv);
	if (1 != EVP_EncryptInit_ex(ci.ectx, NULL, NULL, ci.key, ci.iv)) handleErrors();
	debug("read key and IV", "\nafter: ", ci.key, " ", ci.iv);
}

int encrypt(cryptinfo& ci, unsigned char* plaintext, int plaintext_len,
	unsigned char* ciphertext, unsigned char* aad = 0,	int aad_len = 0)
{
	debug("encrypting string ", plaintext);
	int len;
	int ciphertext_len;
	debug("writing encrypted bytes to ciphertext");
	if (1 != EVP_EncryptUpdate(ci.ectx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
	ciphertext_len = len;
	debug("writing final bytes to ciphertext");
	if (1 != EVP_EncryptFinal_ex(ci.ectx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;
	ciphertext[ciphertext_len] = '\0';
	debug("encrypted string ", ciphertext);
	debug("free ctx obj");
	EVP_CIPHER_CTX_free(ci.ectx);
	debug("return ciphertext_len");
	return ciphertext_len;
}

void getNewDecryptParams(cryptinfo& ci)
{
	debug("getting new decryption params");
	debug("making new ctx, pass me a unfilled one");
	if (!(ci.dctx = EVP_CIPHER_CTX_new())) handleErrors();
	debug("initializing the decryption operation");
	if (!EVP_DecryptInit_ex(ci.dctx, EVP_aes_256_cbc(), NULL, NULL, NULL)) handleErrors();
	debug("setting dctx key and iv \nbefore ", ci.key, " ", ci.iv);
	if (!EVP_DecryptInit_ex(ci.dctx, NULL, NULL, ci.key, ci.iv)) handleErrors();
}

void fillcryptinfo(cryptinfo& ci)
{
	getNewEncryptParams(ci);
	getNewDecryptParams(ci);
}

int decrypt(cryptinfo& ci, unsigned char *ciphertext, int ciphertext_len,
	unsigned char *plaintext, unsigned char *aad = 0, int aad_len = 0)
{
	debug("decrypting string ", ciphertext);
	int len;
	int plaintext_len;
	int ret;
	debug("writing unencrypted bytes to plaintext");
	if (!EVP_DecryptUpdate(ci.dctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;
	debug("writing final bytes to plaintext");
	ret = EVP_DecryptFinal_ex(ci.dctx, plaintext + len, &len);
	plaintext_len += len;
	*(plaintext + plaintext_len) = '\0';
	debug("freeing ctx obj");
	EVP_CIPHER_CTX_free(ci.dctx);
	if (ret == 1)
	{
		debug("decrypt success\ndecrypted string: ", plaintext);
		debug("returning plain text len");
		return plaintext_len;
	}
	else
	{
		debug("there may have been an error check != -1");
		return -1;
	}
}

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
	unsigned char encryptedpassword[64];
	getNewEncryptParams(ci);
	encrypt(ci, (unsigned char*)data.password.c_str(), data.password.size(), encryptedpassword);
	debug("writing password [",
		encryptedpassword,
		"] to file ./Accounts/",
		data.email,
		".txt");
	ofstream accountfile("./Accounts/" + data.email + ".txt");
	
	accountfile << encryptedpassword << "\n";
	accountfile << data.gameservername << "\n";

	data.password = (const char*)encryptedpassword;

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
	//fillcryptinfo(passci);


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
			unsigned char decryptdata[64];
			getNewDecryptParams(passci);
			decrypt(passci, (unsigned char*)sessd.password.c_str(), sessd.password.size(),  decryptdata);
			string dpass = (const char*)decryptdata;
			if ((sessdata.password == dpass) && (!sessd.loggedin))
			{
				sessd.loggedin = true;
				debug("password matched");
				cryptinfo ci;
				createNewRandKeyIV(ci);
				debug("filled crypt info ", ci);
				//send cryptinfo over in body
				//send 127.0.0.1:3000 - this is game server info 
				stringstream ss;
				ss << ci << "\n";
				gameserver& gs = gameservermap.at(sessd.gameservername);
				ss << gs << "\n";
				ss << sessd.email << "\n";
				debug("is this the correct data?", ss.str());
				sockaddr_in gameserv;
				fillinsockaddr(gameserv, 3001, "127.0.0.1");
				sendto(socketS, ss.str().c_str(), ss.str().size(), 0, (SOCKADDR*)&gameserv, sizeof(gameserv));

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