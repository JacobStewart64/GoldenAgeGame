// GoldenAge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "C:/Users/cool_/Documents/cpp-httplib/httplib.h" //<path to your httplib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

//?include last?
#include <Windows.h>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace std;

struct account {
	string email;
	string password;
	long key; //whatever we end up with... need to encrypt the udp packets at least

	account(string email, string password)
	{
		this->email = email;
		this->password = password;
	}
};

vector<account> accounts;

int main()
{
	//Load all accounts by looking at account manifest
	{
		vector<string> account_file_names; //avoid keeping these useless bytes around

		ifstream accountmanifest("./accountmanifest.txt");
		for (string line = ""; getline(accountmanifest, line);)
		{
			account_file_names.push_back(line);
		}
		accountmanifest.close();

		for (int i = 0; i < account_file_names.size(); ++i)
		{
			ifstream acc(account_file_names[i]);
			string e, p;
			acc >> e;
			acc >> p;
			accounts.push_back(account(e, p));
			acc.close();
		}
	}


	cout << "Creating the server, passing it the cert and private key." << endl;	
	httplib::SSLServer server(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);

	if (!server.is_valid()) {
		cout << "Error creating server, shutting down...\n\n\t";
		system("pause");
		return -1;
	}

	//REST API - AUTH A USER, ESTABLISH CRYPTO SHIT, TELL GAME SERVER,
	//GET TOLD BY GAME SERVER WHEN TO TELL CLIENT, TELL CLIENT
	//ALSO - CREATE ACCOUNT (MAKE FIRST)
	server.Post("/login", [](const httplib::Request& req, httplib::Response& res) {
		//login
		cout << "got a login post" << endl;
		cout << req.body << endl;
		char email[100];
		char pass[100];
		int i = 0;
		for (; i < 100; ++i)
		{
			if (req.body[i] == ' ')
			{
				email[i] = '\0';
				break;
			}
			else {
				email[i] = req.body[i];
			}
		}
		for (int j = 0; i < 200; ++i, ++j)
		{
			if (req.body[i] == '\0')
			{
				pass[j] = '\0';
				break;
			}
			else {
				pass[j] = req.body[i];
			}
		}

		string emailstr = email;
		string passstr = pass;

		cout << "search for file in ./Accounts" << endl;
		//search for file in accounts
		WIN32_FIND_DATA file;
		HANDLE search_handle = FindFirstFile(L"C:\\*", &file);
		if (search_handle)
		{
			do
			{
				char buf[100];
				wcstombs(buf, file.cFileName, 100);
				int len = strlen(buf);
				*(buf + (len - 4)) = '\0';
				if (strcmp(buf, emailstr.c_str()) == 0)
				{
					//open the file and compare the passwords
					ifstream passin("./Accounts/" + emailstr + ".txt");
					string inpass;
					passin >> inpass;
					if (inpass == passstr)
					{
						//calculate their crypto shit and, tell the game server
						//wait for it to tell you back, and then tell the client lol
						
					}
					else {
						//they got the password wrong

					}
					break;
				}
				std::wcout << file.cFileName << std::endl;
			} while (FindNextFile(search_handle, &file));
			FindClose(search_handle);

		}

		cout << "writing password to file ./Accounts/" + emailstr + ".txt" << endl;
		//create account file with password in it
		ofstream accountfile("./Accounts/" + emailstr + ".txt");
		accountfile << passstr;
		accountfile.close();

		//inform the client of success
		res.set_content(" ", "text/plain");
	});
	
	server.Post("/createaccount", [](const httplib::Request& req, httplib::Response& res) {
		//just create an account, button to login will be different
		cout << "got a createaccount post" << endl;
		cout << req.body << endl;
		//get the name and the password out
		char email[100];
		char pass[100];
		int i = 0;
		for (; i < 100; ++i)
		{
			if (req.body[i] == ' ')
			{
				email[i] = '\0';
				break;
			}
			else {
				email[i] = req.body[i];
			}
		}
		for (int j = 0; i < 200; ++i, ++j)
		{
			if (req.body[i] == '\0')
			{
				pass[j] = '\0';
				break;
			}
			else {
				pass[j] = req.body[i];
			}
		}

		string emailstr = email;
		string passstr = pass;

		cout << "updating manifest" << endl;
		//write email name to manifest
		ofstream manifest("./accountmanifest.txt", ios::app);
		manifest << emailstr;
		manifest.close();

		cout << "writing password to file ./Accounts/" + emailstr + ".txt" << endl;
		//create account file with password in it
		ofstream accountfile("./Accounts/" + emailstr + ".txt");
		accountfile << passstr;
		accountfile.close();

		//inform the client of success
		res.set_content(" ", "text/plain");
	});

	cout << "Https server listening at https://localhost:1234" << endl;
	server.listen("localhost", 1234);
    return 0;
}

