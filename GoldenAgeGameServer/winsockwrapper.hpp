#pragma once

#include <winsock2.h>
#include <stdio.h>

void initWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

unsigned int initSocketAndSockAddr(SOCKET& s, sockaddr_in& local, unsigned int port)
{
	u_long iMode = 1;
	unsigned int fromlen = sizeof(local);
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (ioctlsocket(s, FIONBIO, &iMode) != NO_ERROR)
		printf("ioctlsocket failed with error");
	bind(s, (sockaddr*)&local, sizeof(local));
	return fromlen;
}