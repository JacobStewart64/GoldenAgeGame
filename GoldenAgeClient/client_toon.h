#pragma once
#include <GoldenAge/debug.h>
#include <GoldenAge/toon.h>
#include <irrlicht.h>

extern irr::scene::ISceneManager* smgr;
extern irr::IrrlichtDevice* device;

struct client_toon {
	ga::toon t;
	irr::scene::IAnimatedMeshSceneNode* node;

	client_toon(ga::toon& t) : t(t), node(0) {}

	void init_position()
	{
		node->setPosition(t.vpos);
	}

	void setup_graphics(const char* path_to_mesh, const char* path_to_texture = 0)
	{
		irr::scene::IAnimatedMesh* mesh = smgr->getMesh(path_to_mesh);
		if (!mesh)
		{
			debug("failed to load mesh!");
			device->drop();
			exit(EXIT_FAILURE);
		}
		node = smgr->addAnimatedMeshSceneNode(mesh);
		if (node)
		{
			node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			node->setScale(irr::core::vector3df(6));
			node->setPosition(node->getAbsolutePosition() - irr::core::vector3df(11, 14, 0));
			node->setRotation(irr::core::vector3df(0, -115, 0));
			//node->setMaterialTexture(0, driver->getTexture(path_to_texture));
		}
	}
};