#pragma once

#include <enet/win32.h>
#include <enet/enet.h>
#include <GoldenAge/Debug.hpp>

void initENet()
{
	debug("initializing ENet");
	if (enet_initialize() != 0)
	{
		debug("An error occurred while initializing ENet.\n");
		system("pause");
	}
	atexit(enet_deinitialize);
	debug("initialize enet good");
}

ENetHost* makeServer(ENetAddress& address, unsigned short port)
{
	debug("make udp server on port ", port);
	address.host = ENET_HOST_ANY;
	address.port = port;
	ENetHost* server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 10,000 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		debug("An error occurred while trying to create an ENet server host.\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
	debug("making udp server good");
	return server;
}


ENetHost* makeClient()
{
	debug("making udp client");
	ENetHost* client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0,
		0
	);

	if (client == NULL)
	{
		debug("An error occurred while trying to create an ENet client host.\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
	debug("making udp client good");
	return client;
}