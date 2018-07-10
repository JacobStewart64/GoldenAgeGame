#pragma once
#include <GoldenAge/debug.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/toon.h>
#include <irrlicht.h>
#include <unordered_map>

extern irr::gui::IGUIEnvironment* env;
extern irr::video::ITexture* bg;
extern irr::video::IVideoDriver* driver;
extern irr::scene::ISceneManager* smgr;
extern unsigned int runloop;
extern ga::udp_com com;

namespace ga {
	class GameEventReceiver : public irr::IEventReceiver
	{

	public:
		GameEventReceiver()	{}

		void setup()
		{
			smgr->addCameraSceneNode(0, irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));
		}

		virtual bool OnEvent(const irr::SEvent& event)
		{
			if (event.EventType == irr::EET_GUI_EVENT)
			{
				debug("got a gui event");
				debug("getting id of gui element");
				irr::s32 id = event.GUIEvent.Caller->getID();
				debug("gui element id: ", id);
				debug("checking type of gui event");
				if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
				{
					debug("irr::gui::EGET_BUTTON_CLICKED");
					if (id == 1)
					{
			
					}
					else if (id == 2)
					{
	
					}
					else
					{
						debug("did you forget to add to the event receiver?");
					}
				}
			}
			return false;
		}
	};
}