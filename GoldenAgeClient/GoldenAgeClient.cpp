#include "stdafx.h"
#include "LoginEventReceiver.h"
#include "CharacterSelectionEventReceiver.h"
#include "login_screen_loop.h"
#include "GameEventReceiver.h"
#include "client_toon.h"
#include "TOPLEVELFLOW.h"
#include <openssl/err.h>
#include <GoldenAge/debug.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/cryptinfo.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/secretkey.h>
#include <GoldenAge/toon.h>


enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};


void load_area(ga::world_position pos, std::string** areamap,
	irr::scene::ITriangleSelector* selector, LOOP_PARAM_PACK& LOOP_PACK)
{
	// add terrain scene node
	debug("opening file: ", (int)pos.row, " ", (int)pos.col, " ", areamap[pos.row][pos.col].c_str());
	irr::scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(areamap[pos.row][pos.col].c_str());
	// create triangle selector for the terrain 
	selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0,
		LOOP_PACK.driver->getTexture("./system/resources/3dart/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,
		LOOP_PACK.driver->getTexture("./system/resources/3dart/detailmap3.jpg"));

	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
}

void add_ground_collision(irr::scene::IAnimatedMeshSceneNode* node, irr::scene::ITriangleSelector* selector)
{
	if (selector)
	{
		irr::scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, node, irr::core::vector3df(30, 50, 30),
			irr::core::vector3df(0, -10, 0), irr::core::vector3df(0, 30, 0));
		node->addAnimator(anim);
		anim->drop();  // And likewise, drop the animator when we're done referring to it.
	}
}

void init_toon_models(std::vector<ga::toon>& toons, std::vector<client_toon>& client_toons)
{
	if (toons.size() > 0)
	{
		client_toon ct(toons[0]);
		debug("world position of first toon! ", (int)toons[0].pos.row, " ", (int)toons[0].pos.col);
		ct.t.tg = toons[0].tg;
		ct.setup_graphics("./system/resources/3dart/human_male.blend.ms3d");
		client_toons.push_back(ct);
	}
	for (int i = 1; i < toons.size(); ++i)
	{
		client_toon ct(toons[i]);
		ct.t.tg = toons[i].tg;
		ct.setup_graphics("./system/resources/3dart/human_male.blend.ms3d");
		ct.node->setVisible(false);
		client_toons.push_back(ct);
	}
}


int main()
{
	LOOP_PARAM_PACK LOOP_PACK;
	LOOPMODE LOOP_MODE = LOOPMODE::LOGIN;
	while (true)
	{
		if (LOOP_MODE == LOOPMODE::LOGIN || LOOP_MODE == LOOPMODE::GAME_TO_CHARACTER_SELECT)
		{
			SET_WINDOW_CAPTION(L"Golden Age Login", LOOP_PACK);
			LOOP_MODE = LOGIN_LOOP(LOOP_PACK, LOOP_MODE);
		}
		else if (LOOP_MODE == LOOPMODE::CHARACTER_SELECT)
		{
			SET_WINDOW_CAPTION(L"Golden Age Character Select", LOOP_PACK);
			LOOP_MODE = CHARACTER_SELECT_LOOP(LOOP_PACK);
		}
		else if (LOOP_MODE == LOOPMODE::GAME || LOOP_MODE == LOOPMODE::RESET_DEVICE)
		{
			SET_WINDOW_CAPTION(L"Golden Age - [toon name] - [area] - [server]", LOOP_PACK);
			LOOP_MODE = GAME_LOOP(LOOP_PACK, LOOP_MODE);
		}
		else
		{
			debug("unknown loop mode FAILURE!");
			DEBUG([]() {
				system("pause");
			});
			exit(EXIT_FAILURE);
		}

		if (LOOP_MODE == LOOPMODE::GAME_TO_CHARACTER_SELECT)
		{
			LOOP_PACK = GAME_TO_CHARACTER_SELECT_PROC(std::move(LOOP_PACK));
		}
		else if (LOOP_MODE == LOOPMODE::RESET_DEVICE)
		{
			LOOP_PACK = RESET_DEVICE_PROC(std::move(LOOP_PACK));
		}
		else
		{
			debug("No top level proc called!");
		}
	}
	
	//start the game loop
	irr::u32 millis = device->getTimer()->getTime();
	//get the screen size data
	LOOP_PACK.screen_size = LOOP_PACK.driver->getScreenSize();

	LOOP_PACK.runloop = true;
	LOOP_PACK.com.listenLoop(LOOP_PACK.runloop, millis);
	LOOP_PACK.runloop = true;
	LOOP_PACK.env->clear();

	//register game packet handlers
	LOOP_PACK.com.register_receive([](ENetEvent& e) {
		debug("My game receive handler");
		ga::array_packet packet;
		packet.from_buf((const char*)e.packet->data, e.packet->dataLength);
		std::string type = packet.get();
		if (type == "d")
		{

		}
		else
		{

		}
	});

	LOOP_PACK.com.register_tick([&LOOP_PACK](unsigned int delta_time) {
		//update toons
		device->run();
		LOOP_PACK.driver->beginScene(true, true, irr::video::SColor(0, 120, 102, 136));

		smgr->drawAll();

		env->drawAll();

		LOOP_PACK.driver->endScene();
	});

	//load area from toon world_position
	load_area(LOOP_PACK.client_toons[0].t.pos);
	debug(LOOP_PACK.client_toons[0].node->getPosition().X, " ", LOOP_PACK.client_toons[0].node->getPosition().Y, " ", LOOP_PACK.client_toons[0].node->getPosition().Z);
	LOOP_PACK.client_toons[0].init_position();
	debug(LOOP_PACK.client_toons[0].node->getPosition().X, " ", LOOP_PACK.client_toons[0].node->getPosition().Y, " ", LOOP_PACK.client_toons[0].node->getPosition().Z);
	add_ground_collision(LOOP_PACK.client_toons[0].node, LOOP_PACK.selector);

	//setup camera to follow client 0
	LOOP_PACK.camera->setPosition(LOOP_PACK.client_toons[0].node->getPosition());
	LOOP_PACK.client_toons[0].node->addChild(LOOP_PACK.camera);
	LOOP_PACK.camera->setPosition(irr::core::vector3df(0, 100, 0));
	LOOP_PACK.camera->updateAbsolutePosition();
	LOOP_PACK.camera->setTarget(LOOP_PACK.client_toons[0].node->getPosition());

	//start the gameloop, probs get a bunch of packets about stuff spawning
	millis = device->getTimer()->getTime();
	LOOP_PACK.com.listenLoop(LOOP_PACK.runloop, millis);


	debug("exiting program");
	system("pause");
	return 0;
}