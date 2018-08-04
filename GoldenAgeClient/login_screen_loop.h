#pragma once

#include "stdafx.h"
#include "LoginEventReceiver.h"
#include "CharacterSelectionEventReceiver.h"
#include <GoldenAge/debug.h>
#include <GoldenAge/udp_com.h>

extern irr::IrrlichtDevice* device;
extern irr::video::IVideoDriver* driver;

namespace ga {
	void loginScreenLoop(unsigned int& runloop, CharacterSelectEventReceiver& rec)
	{
		debug("initializing data for login screen loop");
		irr::video::ITexture* title = driver->getTexture("./system/resources/textures/GoldenAgeTitle.png");
		irr::gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		httplib::SSLClient client("localhost", 1234, 10);
		LoginEventReceiver receiver(client, &runloop, rec);
		device->setEventReceiver(&receiver);
		receiver.setupGameServerMenu();
		receiver.resetStatusMessageTimeout();
		debug("initializing data for login screen loop good");
		debug("entering login screen loop");
		while (device->run() && driver && runloop > 0)
		{
			driver->beginScene(true, true, irr::video::SColor(0, 120, 102, 136));

			driver->draw2DImage(receiver.getBG(), irr::core::position2d<irr::s32>(0, 0),
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
		runloop = 1;
		debug("leaving login screen loop");
	}
}