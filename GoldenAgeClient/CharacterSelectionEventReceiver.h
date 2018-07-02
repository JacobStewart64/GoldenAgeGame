#pragma once
#include <GoldenAge/debug.h>
#include <GoldenAge/toongraphics.h>
#include <irrlicht.h>
#include <unordered_map>

extern irr::gui::IGUIEnvironment* env;

namespace ga {

	class CharacterSelectEventReceiver : public irr::IEventReceiver
	{
		//draw the toon!
		irr::gui::IGUIListBox* toon_select_box = env->addListBox(irr::core::rect<irr::s32>(300, 210, 500, 290), 0, 1);
		irr::gui::IGUIButton* toon_select_button = env->addButton(irr::core::rect<irr::s32>(350, 500, 450, 530), 0, 1, L"Login", L"Login");
		irr::gui::IGUIButton* options_button = env->addButton(irr::core::rect<irr::s32>(350, 500, 450, 530), 0, 2, L"Login", L"Login");
		std::unordered_map<std::string, ga::toongraphics> toon_name_to_graphics;
		std::unordered_map<unsigned int, std::string> toon_id_to_name;

	public:
		CharacterSelectEventReceiver(ga::toongraphics& toon_graphics)
		{
			//initialize map of uint to toongraphic

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

		void setupGameServerMenu()
		{

		}

		void getSelectedToon(std::string& str)
		{
			debug("getting id of selected toon");
			irr::u32 id = toon_select_box->getSelected();
			debug("id: ", id);
			debug("looking in toon map with id");
			str = toon_id_to_name.at(id);
			debug("found string: ", str);
		}

		void openOptions()
		{

		}
	};

}