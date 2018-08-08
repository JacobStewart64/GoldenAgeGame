#pragma once
#include <openssl/err.h>
#include <GoldenAge/debug.h>

#define MAXCLIENTS 1
#define MAXCHANNELS 2


enum LOOPMODE {
	LOGIN = 0,
	CHARACTER_SELECT,
	GAME,
	GAME_TO_CHARACTER_SELECT,
	RESET_DEVICE
};


struct LOOP_PARAM_PACK {
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_DIRECT3D9,
		irr::core::dimension2d<unsigned int>(800, 900), 16, false);
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::gui::IGUIEnvironment* env = device->getGUIEnvironment();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0,
		irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));
	ga::udp_com com;
	irr::scene::ITriangleSelector* selector = 0;
	std::string areamap[12][12] = {
		{ "./system/resources/3dart/terrain-heightmap.bmp" }
	};
	std::vector<client_toon> client_toons;
	std::vector<ga::toon> mytoons;
	ga::cryptinfo ci;
	ga::secretkey sk;
	ga::LoginEventReceiver* LoginReceiver;
	ga::CharacterSelectEventReceiver* CharacterSelectReceiver;
	ga::GameEventReceiver* GameReceiver;
	irr::IEventReceiver* current_receiver;
	irr::core::dimension2du screen_size = driver->getScreenSize();
	bool runloop = true;

	LOOP_PARAM_PACK() : com(MAXCLIENTS, MAXCHANNELS) {}
};


LOOPMODE loginScreenLoop(LOOP_PARAM_PACK& LOOP_PACK)
{
	debug("initializing data for login screen loop");
	LOOP_PACK.runloop = true;
	irr::video::ITexture* title = LOOP_PACK.driver->getTexture("./system/resources/textures/GoldenAgeTitle.png");
	irr::gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	httplib::SSLClient client("localhost", 1234, 10);
	ga::LoginEventReceiver receiver(LOOP_PACK);
	device->setEventReceiver(&receiver);
	receiver.setupGameServerMenu();
	receiver.resetStatusMessageTimeout(L"Hello!");
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
	debug("leaving login screen loop");
	return LOOPMODE::CHARACTER_SELECT;
}


void SET_WINDOW_CAPTION(const wchar_t* window_caption, LOOP_PARAM_PACK& LOOP_PACK)
{
	debug("setting window caption to: ", window_caption);
	device->setWindowCaption(window_caption);
	DEBUG([&LOOP_PACK]() {
		debug("load crypto strings");
		ERR_load_crypto_strings();

		debug("checking device");
		if (LOOP_PACK.device == 0)
		{
			debug("Failed to create device.");
			system("pause");
			exit(EXIT_FAILURE); // could not create selected driver.
		}

		debug("checking driver");
		if (LOOP_PACK.driver == 0)
		{
			debug("Failed to create video driver.");
			system("pause");
			exit(EXIT_FAILURE);
		}

		debug("checking environment");
		if (LOOP_PACK.env == 0)
		{
			debug("Failed to create gui environment.");
			system("pause");
			exit(EXIT_FAILURE);
		}

		debug("checking smgr");
		if (LOOP_PACK.smgr == 0)
		{
			debug("Failed to create scene manager!");
			system("pause");
			exit(EXIT_FAILURE);
		}
	});
}

LOOPMODE LOGIN_LOOP(LOOP_PARAM_PACK& LOOP_PACK, LOOPMODE LOOP_MODE)
{
	LOOP_PACK.com.register_receive([&LOOP_PACK](ENetEvent& e) {
		debug("My receive handler");
		ga::array_packet packet;
		packet.from_buf((const char*)e.packet->data, e.packet->dataLength);
		std::string type = packet.get();
		if (type == "f")
		{
			debug("received all my toongraphics for character select");
			unsigned int s = packet.getInt();
			for (int i = 0; i < s; ++i)
			{
				std::string name = packet.get();
				ga::toon t(name);
				std::string tgraph = packet.get();
				t.tg.from_string(tgraph);
				LOOP_PACK.mytoons.push_back(t);
			}
			init_toon_models(LOOP_PACK.mytoons, LOOP_PACK.client_toons);
			LOOP_PACK.CharacterSelectReceiver = new ga::CharacterSelectEventReceiver(LOOP_PACK);
			device->setEventReceiver(LOOP_PACK.CharacterSelectReceiver);
			LOOP_PACK.runloop = false;
		}
		else
		{
			debug("received unkown packet type");
		}
	});

	LOOP_PACK.com.register_connect([](ENetPeer* e) {
		debug("My connect handler");

	});

	LOOP_PACK.com.register_disconnect([](ENetPeer* e) {
		debug("My disconnect handler");
	});

	LOOP_PACK.com.register_delta([](unsigned int last_time)->unsigned int {
		unsigned int current_time = device->getTimer()->getTime();
		return current_time - last_time;
	});

	LOOP_PACK.com.register_tick([&LOOP_PACK](unsigned int delta_time) {
		//update toons
		device->run();
		LOOP_PACK.driver->beginScene(true, true, irr::video::SColor(0, 120, 102, 136));
		//draw the scene (a background with the toon over it, toon selector, toon select button, options button, appropriate texts)
		LOOP_PACK.driver->draw2DImage(LOOP_PACK.LoginReceiver->getBG(), irr::core::position2d<irr::s32>(0, 0),
			irr::core::rect<irr::s32>(0, 0, 800, 900), 0,
			irr::video::SColor(255, 255, 255, 255), false);

		LOOP_PACK.smgr->drawAll();

		LOOP_PACK.env->drawAll();

		LOOP_PACK.driver->endScene();
	});

	LOOP_PACK.com.connect("localhost", 3001, 60 * 1000 * 15); //timeout 15 minutes

	LOOPMODE LOOP_MODE_ = loginScreenLoop(LOOP_PACK);
	env->clear();
	return LOOP_MODE_;
}

LOOPMODE CHARACTER_SELECT_LOOP(LOOP_PARAM_PACK& LOOP_PACK)
{

}

LOOPMODE GAME_LOOP(LOOP_PARAM_PACK& LOOP_PACK, LOOPMODE LOOP_MODE)
{

}

LOOP_PARAM_PACK&& GAME_TO_CHARACTER_SELECT_PROC(LOOP_PARAM_PACK&& LOOP_PACK)
{

}

LOOP_PARAM_PACK&& RESET_DEVICE_PROC(LOOP_PARAM_PACK&& LOOP_PACK)
{

}