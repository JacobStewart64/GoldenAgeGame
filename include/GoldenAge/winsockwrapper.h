#pragma once
#include <GoldenAge/Debug.h>
#include <enet/enet.h>
#include <enet/win32.h>

#define USEENETINTERNALBANDWIDTHALG 0

void initENet()
{
	debug("initializing ENet");
	if (enet_initialize() != 0)
	{
		debug("An error occurred while initializing ENet.\n");
		system("pause");
	}
	atexit(enet_deinitialize);
	debug("initializing ENet good");
}

ENetHost* makeServer(ENetAddress& address, unsigned short port, unsigned int max_clients, unsigned int max_channels)
{
	debug("making udp server on port ", port);
	address.host = ENET_HOST_ANY;
	address.port = port;
	debug("calling enet_host_create() with params:\ntype: server host: ", address.host, " port: ", address.port, "\nmax_clients: ", max_clients, " max_channels: ", max_channels);
	ENetHost* server = enet_host_create(&address /* the address to bind the server host to */,
		max_clients,
		max_channels,
		USEENETINTERNALBANDWIDTHALG,
		USEENETINTERNALBANDWIDTHALG);
	if (server == NULL)
	{
		debug("An error occurred while trying to create an ENet server host.\n");
		system("pause");
		exit(EXIT_FAILURE);
	}
	debug("making udp server good");
	return server;
}


ENetHost* makeClient(unsigned int max_outgoing_connections, unsigned int max_channels)
{
	debug("making udp client");
	debug("calling enet_host_create() with params:\ntype client, ", max_outgoing_connections, " max outgoing connections, ", max_channels, " max channels");
	ENetHost* client = enet_host_create(NULL,
		max_outgoing_connections, 
		max_channels,
		USEENETINTERNALBANDWIDTHALG,
		USEENETINTERNALBANDWIDTHALG
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