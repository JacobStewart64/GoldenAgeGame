#pragma once
#include "client_toon.h"
#include "TOPLEVELFLOW.h"
#include "Validator.h"
#include <GoldenAge/debug.h>
#include <GoldenAge/toon.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/wcstrtostdstr.h>
#include <unordered_map>


namespace ga {
	class CharacterSelectEventReceiver : public irr::IEventReceiver, private Validator
	{
		irr::gui::IGUIListBox* toon_select_box;
		irr::gui::IGUIButton* toon_select_button;
		irr::gui::IGUIButton* options_button;
		irr::gui::IGUIButton* create_character_button;
		irr::gui::IGUIButton* submit_character_button;
		irr::gui::IGUIButton* cancel_creation_button;
		irr::gui::IGUIEditBox* create_toon_name;
		irr::video::ITexture* bg;
		LOOP_PARAM_PACK& LOOP_PACK;

		void init_toon_select_box()
		{
			wchar_t buf[LOGINBUFFERMAX];
			if (LOOP_PACK.mytoons.size() > 0)
			{
				mbstowcs(buf, LOOP_PACK.mytoons[0].name.c_str(), LOOP_PACK.mytoons[0].name.size());
				toon_select_box->addItem(buf);
				toon_select_box->setSelected(buf);
			}
			for (int i = 1; i < LOOP_PACK.mytoons.size(); ++i)
			{
				mbstowcs(buf, LOOP_PACK.mytoons[i].name.c_str(), LOOP_PACK.mytoons[i].name.size());
				toon_select_box->addItem(buf);
			}
		}

	public:
		CharacterSelectEventReceiver(LOOP_PARAM_PACK& LOOP_PACK) : LOOP_PACK(LOOP_PACK)
		{
			bg = LOOP_PACK.driver->getTexture("./system/resources/textures/CharacterSelectBG.png");
			toon_select_box = LOOP_PACK.env->addListBox(irr::core::rect<irr::s32>(450, 50, 750, 750), 0, 1, true);
			toon_select_button = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(310, 700, 410, 750), 0, 1, L"Login", L"Login");
			options_button = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(185, 700, 285, 750), 0, 2, L"Options", L"Options");
			create_character_button = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(320, 5, 400, 45), 0, 3, L"Create Character", L"Create Character");
			submit_character_button = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(310, 745, 410, 795), 0, 4, L"Create Character", L"Create Character");
			submit_character_button->setVisible(false);
			cancel_creation_button = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(185, 745, 285, 795), 0, 5, L"Cancel", L"Cancel");
			cancel_creation_button->setVisible(false);
			create_toon_name = LOOP_PACK.env->addEditBox(L"", irr::core::rect<irr::s32>(185, 700, 285, 740), true, 0, 1);
			create_toon_name->setVisible(false);
		}

		void finishCreateToon(std::string& name)
		{
			LOOP_PACK.mytoons.back().name = name;
			toon_select_box->clear();
			init_toon_select_box();

			submit_character_button->setVisible(false);
			cancel_creation_button->setVisible(false);
			create_toon_name->setVisible(false);

			toon_select_box->setVisible(true);
			toon_select_button->setVisible(true);
			options_button->setVisible(true);
			create_character_button->setVisible(true);
		}

		toon* getSelectedToon()
		{
			return &LOOP_PACK.mytoons[toon_select_box->getSelected()];
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
						std::string ptype("b");
						std::string selected(std::to_string(toon_select_box->getSelected()));
						ga::array_packet pack(ga::array_packet::get_args_size(ptype, selected));
						pack.fill(ptype, selected);
						LOOP_PACK.com.send(LOOP_PACK.com.getPeer(), pack(), pack.size(), ENET_PACKET_FLAG_RELIABLE);
						LOOP_PACK.GameReceiver = new ga::GameEventReceiver(LOOP_PACK);
						device->setEventReceiver(LOOP_PACK.GameReceiver);
						LOOP_PACK.runloop = false;
					}
					else if (id == 2)
					{
						//options_button pushed
						openOptions();
					}
					else if (id == 3)
					{
						//1st - turn off all the non-create-toon gui stuff
						toon_select_box->setVisible(false);
						toon_select_button->setVisible(false);
						options_button->setVisible(false);
						create_character_button->setVisible(false);
						//2nd - turn on all the create-toon gui stuff
						submit_character_button->setVisible(true);
						cancel_creation_button->setVisible(true);
						create_toon_name->setVisible(true);
						//3rd - create a default toon, make him a client_toon, display him graphically in the toon spot!
						//client toons is still in its state where the first client toon does not represent the player
						//it will be erased! We don't have the name yet so use a dummy empty string name. Fill in the name on submit. Add client toons at back name to listbox.
						ga::toon toon(true);
						LOOP_PACK.mytoons.push_back(toon);
						client_toon ct(toon);
						int ret = toon_select_box->getSelected();
						if (ret != -1)
							LOOP_PACK.client_toons[ret].node->setVisible(false);
						ct.setup_graphics("./system/resources/3dart/human_male.blend.ms3d");
						LOOP_PACK.client_toons.push_back(ct);
					}
					else if (id == 4)
					{
						//submit created toon
						//for now this is a g packet send and do simple gui things
						std::string type("g");
						std::string name = wcstrtostdstr16(create_toon_name->getText());
						finishCreateToon(name);
						ga::array_packet packet(ga::array_packet::get_args_size(type, name));
						packet.fill(type, name);
						LOOP_PACK.com.send(LOOP_PACK.com.getPeer(), packet(), packet.size(), ENET_PACKET_FLAG_RELIABLE);
						//sent packet "create toon" with name info!
					}
					else if (id == 5)
					{
						//cancel toon create
						LOOP_PACK.client_toons.pop_back();
						LOOP_PACK.mytoons.pop_back();

						submit_character_button->setVisible(false);
						cancel_creation_button->setVisible(false);
						create_toon_name->setVisible(false);

						toon_select_box->setVisible(true);
						toon_select_button->setVisible(true);
						options_button->setVisible(true);
						create_character_button->setVisible(true);
					}
					else
					{
						debug("did you forget to add to the event receiver?");
					}
				}
				else if (event.GUIEvent.EventType == irr::gui::EGET_EDITBOX_CHANGED)
				{
					debug("irr::gui::EGET_EDITBOX_CHANGED");
					if (id == 1)
					{
						debug("checking if toon name string is too long");
						validateInput(create_toon_name);
						debug("toon name string good");
					}
					else
					{
						debug("did you forget to add to the event receiver?");
					}
				}
			}
			return false;
		}

		irr::video::ITexture* getBG()
		{
			return bg;
		}
	};
}