#pragma once

#include <winsock2.h>
#include <stdio.h>

void initWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

unsigned int fillinsockaddr(sockaddr_in& addr, unsigned int port, const char* inet)
{
	unsigned int fromlen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(inet);
	return fromlen;
}

unsigned int initSocketAndSockAddr(SOCKET& s, sockaddr_in& local, unsigned int port, bool blocking = false)
{
	u_long iMode = 1;
	unsigned int fromlen = fillinsockaddr(local, port, "127.0.0.1");
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (!blocking)
	{
		if (ioctlsocket(s, FIONBIO, &iMode) != NO_ERROR)
			printf("ioctlsocket failed with error");
	}
	bind(s, (sockaddr*)&local, sizeof(local));
	return fromlen;
}