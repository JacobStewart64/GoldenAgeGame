#pragma once
#include "stdafx.h"
#include "Sleeper.h"
#include "CharacterSelectionEventReceiver.h"
#include "Validator.h"
#include <GoldenAge/debug.h>
#include <GoldenAge/cryptinfo.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/wcstrtostdstr.h>
#include <GoldenAge/secretkey.h>
#include <functional>


namespace ga {
	class LoginEventReceiver : public irr::IEventReceiver, private Validator
	{
		irr::gui::IGUIStaticText* statusmsg = LOOP_PACK.env->addStaticText(L"Hello!", irr::core::rect<irr::s32>(150, 290, 650, 320), false, false, 0, -1, false);
		irr::gui::IGUIListBox* gameserver = LOOP_PACK.env->addListBox(irr::core::rect<irr::s32>(300, 210, 500, 290), 0, 1);
		irr::gui::IGUIEditBox* email = LOOP_PACK.env->addEditBox(L"", irr::core::rect<irr::s32>(300, 320, 500, 390), true, 0, 1);
		irr::gui::IGUIEditBox* password = LOOP_PACK.env->addEditBox(L"", irr::core::rect<irr::s32>(300, 410, 500, 480), true, 0, 2);
		irr::gui::IGUIButton* login = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(350, 500, 450, 530), 0, 1, L"Login", L"Login");
		irr::gui::IGUIButton* createaccount = LOOP_PACK.env->addButton(irr::core::rect<irr::s32>(350, 540, 450, 570), 0, 2, L"Make Account", L"Make Account");
		std::unordered_map<unsigned int, std::string> id_to_game_server_name_map;
		httplib::SSLClient* client;
		std::vector<std::thread> threads;
		std::vector<ga::sleeper*> sleepers;
		irr::video::ITexture* bg = LOOP_PACK.driver->getTexture("./system/resources/textures/kewlbg.jpg");
		LOOP_PARAM_PACK& LOOP_PACK;

		void onRequestSuccess(std::shared_ptr<httplib::Response>& res,
			std::function<void(std::shared_ptr<httplib::Response>& res)> handle_success)
		{
			debug("response body: ", res->body);
			if (res->body[0] == '\0')
			{
				debug("request failure");
				resetStatusMessageTimeout(L"Request failure!");
			}
			else
			{
				handle_success(res);
			}
		}

	public:
		LoginEventReceiver(LOOP_PARAM_PACK& LOOP_PACK) : LOOP_PACK(LOOP_PACK)
		{
			debug("constructing LoginEventReceiver");
			debug("setting alignment of statusmsg, email, and password");
			statusmsg->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
			email->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
			password->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
			debug("constructing LoginEventReceiver good");
		}

		virtual bool OnEvent(const irr::SEvent& event)
		{
			if (event.EventType == irr::EET_GUI_EVENT)
			{
				irr::s32 id = event.GUIEvent.Caller->getID();
				if (event.GUIEvent.EventType == irr::gui::EGET_EDITBOX_CHANGED)
				{
					debug("irr::gui::EGET_EDITBOX_CHANGED");
					if (id == 1)
					{
						validateInput(email);
					}
					else if (id == 2)
					{
						validateInput(password);
					}
					else
					{
						debug("Unknown GUI element event");
					}
				}
				else if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
				{
					debug("irr::gui::EGET_BUTTON_CLICKED");
					if (id == 1)
					{
						sendLoginPost();
					}
					else if (id == 2)
					{
						sendCreateAccountPost();
						sendLoginPost();
					}
					else
					{
						debug("did you forget to add to the event receiver?");
					}
				}
			}
			return false;
		}

		std::string getGameServerNameFromSelected()
		{
			debug("getting id of selected game server");
			unsigned int id = gameserver->getSelected();
			debug("id: ", id);
			debug("looking in game server map with id");
			if (id_to_game_server_name_map.find(id) != id_to_game_server_name_map.end())
			{
				std::string selected_game_server_name = id_to_game_server_name_map.at(id);
				debug("found string: ", selected_game_server_name);
				return selected_game_server_name;
			}
			debug("bad game server id, no game server name at id");
			debug("what are you doing? Exiting...");
			DEBUG([]() {
				system("pause");
			});
			exit(EXIT_FAILURE);
		}

		void setupGameServerMenu()
		{
			debug("setting up game server menu");
			debug("opening file ", GAMESERVERINILOCATION);
			std::ifstream gameserverini(GAMESERVERINILOCATION);
			debug("adding first game server name to list");
			std::string line; getline(gameserverini, line);
			wchar_t buf[LOGINBUFFERMAX];
			mbstowcs(buf, line.c_str(), line.size());
			unsigned int id = gameserver->addItem(buf);
			debug("entry id ", id);
			debug("insert ", line, " at ", id, "in the gserver_id to game server name map");
			id_to_game_server_name_map.insert(std::make_pair(id, line));
			debug("insert good");
			debug("set the first item selected");
			gameserver->setSelected(buf);
			debug("set first item selected good");
			for (std::string line; getline(gameserverini, line);)
			{
				debug("making entry in map for ", line);
				wchar_t buf[LOGINBUFFERMAX];
				mbstowcs(buf, line.c_str(), line.size());
				irr::u32 id = gameserver->addItem(buf);
				debug("entry id ", id);
				debug("insert ", line, " at ", id, "in the gserver_id to game server name map");
				id_to_game_server_name_map.insert(make_pair(id, line));
				debug("insert good");
				debug("make entry good");
			}
			debug("close file ", GAMESERVERINILOCATION);
			gameserverini.close();
			debug("game server menu setup good");
		}

		template<typename ...T, unsigned int post_max_buf_len = LOGINBUFFERMAX>
		void makePost(const char* location,
			std::function<void(std::shared_ptr<httplib::Response>& res)> response_handler,
			T&& ...args_pack) //a bunch of strings to be concatenated
		{
			debug("login post at ", location);
			std::string post_str = ((args_pack.size() >= post_max_buf_len ?
				(debug("post str arg >= ", post_max_buf_len, ", FATALITY!!!"),
					system("pause"), exit(EXIT_FAILURE)) : std::forward<T>(args_pack)) += ...)
			debug("post data string: ", post_str);
			std::shared_ptr<httplib::Response> res = client->Post(location, post_str, "text/plain");
			debug("handling response");
			response_handler(res);
		}

		void sendLoginPost()
		{
			std::string email_str = wcstrtostdstr16(email->getText());
			std::string password_str = wcstrtostdstr16(password->getText());
			std::string game_server_name = getGameServerNameFromSelected();
			makePost("/login",
				[this, &email_str](std::shared_ptr<httplib::Response>& res) {
				onRequestSuccess(res, [this, &email_str](std::shared_ptr<httplib::Response>& res) {
					debug("request success");
					this->resetStatusMessageTimeout(L"Request Success!");
					std::string type(res->body.substr(0, 1), 0, 1);
					debug("type: ", type);
					std::string key(res->body.substr(2, 32), 0, 32);
					debug("key: ", key);
					std::string iv(res->body.substr(35, 16), 0, 16);
					debug("iv: ", iv, " remember to change this arg every time.\
								a no network solution is awesome but I don't think it is 100% secure.\
								send new iv from gameserver every response (Initialization Vector\
								change = same string, different encrypt)");
					std::string sk_str(res->body.substr(52, 256), 0, 256);
					debug("sk_str: ", sk_str);
					LOOP_PACK.ci.keyFromString(key);
					LOOP_PACK.ci.ivFromString(iv);
					LOOP_PACK.sk.from_string(sk_str);
					debug("established session!");
					debug("client ci for this session: ", LOOP_PACK.ci);
					debug("client secret key for this session: ", LOOP_PACK.sk.to_string());
					debug("connecting to the game server...");

					LOOP_PACK.CharacterSelectReceiver = new CharacterSelectEventReceiver(LOOP_PACK);
					device->setEventReceiver(LOOP_PACK.CharacterSelectReceiver);
					jointhreads();

					std::string typ("a");
					ga::array_packet pack(
						ga::array_packet::get_args_size(
							typ,
							email_str,
							LOOP_PACK.sk.to_string()
						));
					pack.fill(typ, email_str, LOOP_PACK.sk.to_string());
					LOOP_PACK.com.send(LOOP_PACK.com.getPeer(), pack(), pack.size(), ENET_PACKET_FLAG_RELIABLE);
				});
			},
			email,
			password,
			game_server_name);	
		}

		void sendCreateAccountPost()
		{
			std::shared_ptr<httplib::Response> res;
			makePost("/createaccount", [this](std::shared_ptr<httplib::Response>& res) {
				onRequestSuccess(res, [this](std::shared_ptr<httplib::Response>& res) {
					debug("create account success");
					resetStatusMessageTimeout(L"Create Account Success!");
				});
			},
			wcstrtostdstr16(email->getText()),
			wcstrtostdstr16(password->getText()),
			getGameServerNameFromSelected());
		}

		void resetStatusMessageTimeout(const wchar_t* status_msg)
		{
			debug("pushing status timeout thread onto thread array");
			statusmsg->setText(status_msg);
			ga::sleeper* sleep = new ga::sleeper();
			threads.push_back(std::thread([this, sleep]() {
				sleep->sleepFor(std::chrono::milliseconds(3000));
				statusmsg->setText(L"");
			}));
			sleepers.push_back(sleep);
			debug("pushing status timeout thread good");
		}

		void jointhreads()
		{
			debug("joining threads");
			for (int i = 0; i < threads.size(); ++i)
			{
				sleepers[i]->wake();
				if (threads[i].joinable())
				{
					threads[i].join();
				}
				delete sleepers[i];
			}
			debug("joining threads good");
		}

		irr::video::ITexture* getBG()
		{
			debug("got bg");
			return bg;
		}
	};
}