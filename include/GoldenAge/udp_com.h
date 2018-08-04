#pragma once
#include <GoldenAge/debug.h>
#include <enet/enet.h>
#include <enet/win32.h>
#include <vector>
#include <string>
#include <functional>

#define USEENETINTERNALBANDWIDTHALG 0

//these definitions allow lambda capture - does not convert to lpfunction
typedef std::function<void(ENetEvent&)> efunc;
typedef std::function<void(ENetPeer*)> pfunc;
typedef std::function<unsigned int(unsigned int)> dfunc;
typedef std::function<void(unsigned int)> tfunc;

namespace ga {
	class udp_com {
		ENetEvent e;
		ENetHost* host;
		efunc handler_receive = nullptr;
		pfunc handler_connect = nullptr;
		pfunc handler_disconnect = nullptr;
		tfunc handler_tick = nullptr;
		dfunc handler_delta = nullptr;
		std::vector<ENetPeer*> peers;

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

		ENetHost* makeServer(unsigned short port, unsigned int max_clients, unsigned int max_channels)
		{
			debug("making udp server on port ", port);
			ENetAddress address;
			address.host = ENET_HOST_ANY;
			address.port = port;
			debug("calling enet_host_create() with params:\ntype: server host: ", address.host, " port: ", address.port, "\nmax_clients: ", max_clients, " max_channels: ", max_channels);
			ENetHost* server = enet_host_create(&address, max_clients, max_channels,
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
			ENetHost* client = enet_host_create(NULL, max_outgoing_connections, max_channels,
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

		void warnMessageHandlers()
		{
			debug("checking message handlers...");
			if (!handler_receive)
			{
				debug("WARNING: receive handler not registered!");
				handler_receive = [](ENetEvent& e) { debug("WARNING: receive handler not registered and received a message"); };
			}
			if (!handler_connect)
			{
				debug("WARNING: connect handler not registered!");
				handler_connect = [](ENetPeer* e) { debug("WARNING: connect handler not registered and got a connection event"); };
			}
			if (!handler_disconnect)
			{
				debug("WARNING: disconnect handler not registered!");
				handler_disconnect = [](ENetPeer* e) { debug("WARNING: disconnect handler not registered and got a disconnect event"); };
			}
			if (!handler_tick)
			{
				debug("WARNING: tick handler not registered!");
				handler_tick = [](unsigned int delta_time) {};
			}
			if (!handler_delta)
			{
				debug("WARNING: delta handler not registered! Time will stand still, delta_time won't move anything.");
				handler_delta = [](unsigned int last_time)->unsigned int { return 0; };
			}
		}

	public:
		udp_com(unsigned int max_clients, unsigned int max_channels, unsigned short port = 0, bool server = false)
		{
			initENet();
			if (server)
			{
				debug("you have chosen to make a udp server");
				host = makeServer(port, max_clients, max_channels);
			}
			else {
				debug("you have chosen to make a udp client");
				host = makeClient(max_clients, max_channels);
			}
		}

		ENetPeer* getPeer()
		{
			return peers.back();
		}

		void connect(std::string domain, unsigned short port, unsigned int timeout)
		{
			ENetAddress address;
			enet_address_set_host(&address, domain.c_str());
			address.port = port;
			debug("connecting to ", domain, ":", port);
			peers.push_back(enet_host_connect(host, &address, 2, 0));
			if (peers.back() == NULL)
			{
				debug("No available peers for initiating an ENet connection.\n");
				system("pause");
				exit(EXIT_FAILURE);
			}
			if (enet_host_service(host, &e, timeout) > 0 && e.type == ENET_EVENT_TYPE_CONNECT)
			{
				debug("Connection to ", domain, ":", port, " succeeded.");
			}
			else
			{
				debug("nulling peer data");
				enet_peer_reset(peers.back());
				debug("Connection to ", domain, ":", port, " failed.");
			}
		}

		void send(ENetPeer* peer, const char* packet, unsigned int size, enum _ENetPacketFlag ENETPACKETFLAG)
		{
			debug("sending packet");
			ENetPacket* pack = enet_packet_create(packet, size, ENETPACKETFLAG);
			debug("the packet we are sending\n", pack->data);
			enet_peer_send(peer, 0, pack);
			debug("flushing host! (packet sent - prob don't rely on this happening)");
			enet_host_flush(host);
		}

		void register_receive(efunc handler_receive)
		{
			debug("registering receive handler");
			this->handler_receive = handler_receive;
		}

		void register_connect(pfunc handler_connect)
		{
			debug("registering connect handler");
			this->handler_connect = handler_connect;
		}

		void register_disconnect(pfunc handler_disconnect)
		{
			debug("registering disconnect handler");
			this->handler_disconnect = handler_disconnect;
		}

		void register_tick(tfunc handler_tick)
		{
			debug("registering tick handler");
			this->handler_tick = handler_tick;
		}

		void register_delta(dfunc handler_delta)
		{
			debug("registering delta handler");
			this->handler_delta = handler_delta;
		}

		void listenLoop(unsigned int& runloop, unsigned int last_time)
		{
			warnMessageHandlers();
			debug("entering udp listen loop");
			while (runloop > 0)
			{
				ENetEvent e;

				while (enet_host_service(host, &e, 0) > 0)
				{
					switch (e.type)
					{
					case _ENetEventType::ENET_EVENT_TYPE_CONNECT:
						debug("A new client connected from ", e.peer->address.host, ":", e.peer->address.port);
						debug("handling connect");
						handler_connect(e.peer);
						break;
					case _ENetEventType::ENET_EVENT_TYPE_RECEIVE:
						debug("A packet of length ", e.packet->dataLength, " containing ", e.packet->data, " was received from ", e.peer->data, " on channel ", e.peer->data, ".\n");
						debug("handling packet");
						handler_receive(e);
						debug("packet handled destroying packet");
						enet_packet_destroy(e.packet);
						break;
					case _ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
						debug(e.peer->data, " disconnected");
						debug("handling disconnect");
						handler_disconnect(e.peer);
						debug("null peer->data");
						e.peer->data = NULL;
					}
				}
				unsigned int delta_time = handler_delta(last_time);
				last_time += delta_time;
				handler_tick(delta_time);
			}
			debug("leaving udp listen loop");
		}
	};
};