// GoldenAgeGameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//winsock server only! hooray!
#include "winsockwrapper.hpp" //why can't I put this in stdafx.h?
#include "../GoldenAgeLoginServer/Debug.hpp"
#include "../GoldenAgeLoginServer/cryptinfo.hpp"

void serverLoop()
{
	SOCKET socketS;
	struct sockaddr_in local;
	struct sockaddr_in from;
	initWinsock();
	int fromlen = initSocketAndSockAddr(socketS, local, 3001);

	while (true)
	{
		char buffer[1024];
		ZeroMemory(buffer, sizeof(buffer));
		if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
		{
			printf("Received message from %s: %s\n", inet_ntoa(from.sin_addr), buffer);
			if (buffer[0] == 'c')
			{
				//login messaage
				//setup ci and stuff for client
				//after client setup, inform server of w - who
			}
			else
			{
				sendto(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, fromlen);
			}
		}
	}
	closesocket(socketS);
}

int main()
{
	ERR_load_crypto_strings();
	debug("game server1 listening at localhost:3001");
	serverLoop();

	return 0;
}