#pragma once

#include "stdafx.h"
#include "LoginEventReceiver.h"
#include <GoldenAge/Debug.h>
#include <GoldenAge/winsockwrapper.h>

extern irr::IrrlichtDevice* device;
extern irr::video::IVideoDriver* driver;

namespace ga {
	void loginScreenLoop()
	{
		debug("initializing data for login screen loop");
		//ITexture* bg = driver->getTexture("./system/resources/textures/loginscreen.png");
		irr::video::ITexture* bg = driver->getTexture("./system/resources/textures/kewlbg.jpg");
		irr::video::ITexture* title = driver->getTexture("./system/resources/textures/GoldenAgeTitle.png");
		irr::gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		httplib::SSLClient client("localhost", 1234, 10);
		LoginEventReceiver receiver(client);
		device->setEventReceiver(&receiver);
		receiver.setupGameServerMenu();
		receiver.resetStatusMessageTimeout();
		debug("initializing data for login screen loop good");
		debug("entering login screen loop");
		while (device->run() && driver)
		{
			driver->beginScene(true, true, irr::video::SColor(0, 120, 102, 136));

			driver->draw2DImage(bg, irr::core::position2d<irr::s32>(0, 0),
				irr::core::rect<irr::s32>(0, 0, 800, 900), 0,
				irr::video::SColor(255, 255, 255, 255), false);

			driver->draw2DImage(title, irr::core::position2d<irr::s32>(50, 10),
				irr::core::rect<irr::s32>(0, 0, 700, 230), 0,
				irr::video::SColor(255, 255, 255, 255), true);

			env->drawAll();

			font->draw(L"email",
				irr::core::rect<irr::s32>(350, 330, 450, 370),
				irr::video::SColor(255, 0, 0, 0), true);

			font->draw(L"password",
				irr::core::rect<irr::s32>(350, 420, 450, 460),
				irr::video::SColor(255, 0, 0, 0), true);

			driver->endScene();
		}
		debug("leaving login screen loop");
	}

	void gameLoop()
	{
		debug("initializing data for game loop");
		initENet();
		ENetHost* client = makeClient(1, 2);
		debug("initializing data for game loop good");
		debug("entering game loop");
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
		debug("leaving game loop");
	}
}