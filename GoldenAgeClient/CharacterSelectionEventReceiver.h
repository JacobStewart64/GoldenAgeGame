#pragma once
#include <GoldenAge/debug.h>
#include <GoldenAge/toongraphics.h>
#include <GoldenAge/toon.h>
#include <irrlicht.h>
#include <unordered_map>

extern irr::gui::IGUIEnvironment* env;
extern irr::video::ITexture* bg;
extern irr::video::IVideoDriver* driver;
extern irr::scene::ISceneManager* smgr;

namespace ga {
	class CharacterSelectEventReceiver : public irr::IEventReceiver
	{
		//draw the toon!

		irr::gui::IGUIListBox* toon_select_box;
		irr::gui::IGUIButton* toon_select_button;
		irr::gui::IGUIButton* options_button;
		std::vector<toon>* toons;

	public:
		CharacterSelectEventReceiver() {}

		void setup()
		{
			smgr->addCameraSceneNode(0, irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));
			bg = driver->getTexture("./system/resources/textures/CharacterSelectBG.png");
			toon_select_box = env->addListBox(irr::core::rect<irr::s32>(450, 50, 750, 750), 0, 1, true);
			toon_select_button = env->addButton(irr::core::rect<irr::s32>(310, 700, 410, 750), 0, 1, L"Login", L"Login");
			options_button = env->addButton(irr::core::rect<irr::s32>(185, 700, 285, 750), 0, 2, L"Login", L"Login");
		}

		void init_toon_select_box(std::vector<toon>& toons)
		{
			//I have all the toon graphics, which represent the character's equipment models.
			//the character will be represented by something else I guess. There's a lot of little
			//id that I have to make to represent everything for the gameserver and client.
			this->toons = &toons;
			bool firsttime = true;
			for (toon& t : toons)
			{
				wchar_t buf[LOGINBUFFERMAX];
				mbstowcs(buf, t.name.c_str(), t.name.size());
				unsigned int i = toon_select_box->addItem(buf);
				if (firsttime)
				{
					toon_select_box->setSelected(buf);
					firsttime = false;
				}
			}
		}

		toon* getSelectedToon()
		{
			return &(*toons)[toon_select_box->getSelected()];
		}

		void openOptions()
		{

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
						//toon_select_button pushed
						//get id from selected
						//send packet to game server picking which one
					}
					else if (id == 2)
					{
						//options_button pushed
						openOptions();
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