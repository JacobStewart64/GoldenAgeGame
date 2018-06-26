// GoldenAgeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace httplib;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice* device = createDevice(EDT_DIRECT3D9, dimension2d<u32>(800, 900), 16, false);
IVideoDriver* driver = device->getVideoDriver();;
IGUIEnvironment* env = device->getGUIEnvironment();
u32 millis = device->getTimer()->getTime();

class LoginEventReceiver : public IEventReceiver
{

public:
	IGUIStaticText* statusmsg = env->addStaticText(L"Hello!", rect<s32>(275, 290, 325, 320), false, false, 0, -1, false);
	IGUIListBox* gameserver = env->addListBox(rect<s32>(300, 210, 500, 290), 0, 1);
	unordered_map<u32, std::string> gsitemmap;
	IGUIEditBox* email = env->addEditBox(L"", rect<s32>(300, 320, 500, 390), true, 0, 1);
	IGUIEditBox* password = env->addEditBox(L"", rect<s32>(300, 410, 500, 480), true, 0, 2);
	IGUIButton* login = env->addButton(rect<s32>(350, 500, 450, 530), 0, 1, L"Login", L"Login");
	IGUIButton* createaccount = env->addButton(rect<s32>(350, 540, 450, 570), 0, 2, L"Make Account", L"Make Account");
	bool bdrawemailtext = true;
	bool bdrawpasstext = true;
	bool bsendlogin = false;
	bool bsendcreateaccount = false;
	bool bshowstatus = true;

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch (event.GUIEvent.EventType)
			{
			case EGET_EDITBOX_CHANGED:
				if (id == 1)
				{
					if (strcmp((char*)email->getText(), (char*)L"") == 0)
					{
						bdrawemailtext = true;
					}
					else
					{
						bdrawemailtext = false;
					}
				}
				else if (id == 2)
				{
					if (strcmp((char*)password->getText(), (char*)L"") == 0)
					{
						bdrawpasstext = true;
					}
					else
					{
						bdrawpasstext = false;
					}
				}
				break;

			case EGET_BUTTON_CLICKED:
				if (id == 1)
				{
					//login button clicked
					bsendlogin = true;
				}
				else if (id == 2)
				{
					//create account button clicked
					bsendcreateaccount = true;
				}
				break;
			}
		}
		return false;
	}

	std::string getGameServerStringFromSelected()
	{
		return gsitemmap.at(gameserver->getSelected());
	}

	void setupGameServerMenu()
	{
		//read them from game server.ini
		debug("opening file ", GAMESERVERINILOCATION);
		ifstream gameserverini(GAMESERVERINILOCATION);
		bool firsttime = true;
		for (std::string line; getline(gameserverini, line);)
		{
			debug(line);
			wchar_t buf[LOGINBUFFERMAX];
			mbstowcs(buf, line.c_str(), line.size());
			wcout << buf << endl;
			if (firsttime)
			{
				gsitemmap.insert(make_pair(gameserver->addItem(buf), line));
				//set focus
				gameserver->setSelected(buf);
				firsttime = false;
			}
			else
			{
				gsitemmap.insert(make_pair(gameserver->addItem(buf), line));
			}
		}
	}

	bool shouldSendLogin()
	{
		return bsendlogin;
	}

	bool shouldSendCreateAccount()
	{
		return bsendcreateaccount;
	}

	void resetSendLogin()
	{
		bsendlogin = false;
	}

	void resetSendCreateAccount()
	{
		bsendcreateaccount = false;
	}
};

shared_ptr<httplib::Response> makeLoginPost(const char* location, SSLClient& client, LoginEventReceiver& receiver)
{
	char emailspacepassword[LOGINBUFFERMAX + LOGINBUFFERMAX + LOGINBUFFERMAX + 3];
	wcstombs(emailspacepassword, receiver.email->getText(), LOGINBUFFERMAX);
	int len = strlen(emailspacepassword);
	*(emailspacepassword + len++) = ' ';
	wcstombs(emailspacepassword + len, receiver.password->getText(), LOGINBUFFERMAX);
	len = strlen(emailspacepassword);
	*(emailspacepassword + len++) = ' ';
	std::string str = receiver.getGameServerStringFromSelected().c_str();
	memcpy(emailspacepassword + len, str.c_str(), str.size()+1);
	return client.Post(location, emailspacepassword, "text/plain");
}

void jointhreads(vector<thread>& threads)
{
	for (thread& t : threads)
	{
		t.join();
	}
}

int main()
{
	device->setWindowCaption(L"Golden Age Login");
	
	vector<thread> threads;

	if (device == 0)
	{
		debug("Failed to create device.");
		system("pause");
		return 1; // could not create selected driver.
	}

	if (driver == 0)
	{
		debug("Failed to create video driver.");
		system("pause");
		return 1;
	}

	if (env == 0)
	{
		debug("Failed to create gui environment.");
		system("pause");
		return 1;
	}

	{
		//ITexture* bg = driver->getTexture("./system/resources/textures/loginscreen.png");
		ITexture* bg = driver->getTexture("./system/resources/textures/kewlbg.jpg");
		IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		SSLClient client("localhost", 1234, 10);
		LoginEventReceiver receiver;
		device->setEventReceiver(&receiver);
		receiver.setupGameServerMenu();
		threads.push_back(thread([&receiver]() {
			Sleep(5000);
			receiver.statusmsg->setText(L"");
		}));
		while (device->run() && driver)
		{
			driver->beginScene(true, true, video::SColor(0, 120, 102, 136));

			//draw bg image
			driver->draw2DImage(bg, core::position2d<s32>(0, 0),
				rect<s32>(0, 0, 800, 900), 0,
				SColor(255, 255, 255, 255), false);

			//draw gui
			env->drawAll();

			//draw 2d text
			// draw email text
			font->draw(L"email",
				rect<s32>(350, 330, 450, 370),
				SColor(255, 0, 0, 0));


			// draw password text
			font->draw(L"password",
				rect<s32>(350, 420, 450, 460),
				SColor(255, 0, 0, 0));

			if (receiver.shouldSendLogin())
			{
				auto res = makeLoginPost("/login", client, receiver);
				debug("response body: ", res->body);
				if (res->body[0] == '\0')
				{
					debug("login failure");
					receiver.statusmsg->setText(L"Login failure!");
					threads.push_back(thread([&receiver]() {
						Sleep(5000);
						receiver.statusmsg->setText(L"");
					}));
				}
				else {
					debug("login success");
					receiver.statusmsg->setText(L"Login Success!");
				}
				//this body needs to let me send packets at my game server
				receiver.resetSendLogin();
			}
			
			if (receiver.shouldSendCreateAccount())
			{
				auto res = makeLoginPost("/createaccount", client, receiver);
				debug("response body: ", res->body);
				if (res->body[0] == '\0')
				{
					debug("create account failure");
					receiver.statusmsg->setText(L"Create Account Failure!");
					threads.push_back(thread([&receiver]() {
						Sleep(5000);
						receiver.statusmsg->setText(L"");
					}));
				}
				else
				{
					receiver.statusmsg->setText(L"Create Account Success!");
					threads.push_back(thread([&receiver]() {
						Sleep(5000);
						receiver.statusmsg->setText(L"");
					}));
				}
				receiver.resetSendCreateAccount();
			}

			driver->endScene();
		}
	}

	debug("exiting program\n\n\t ");
	jointhreads(threads); //you have to do this or it will crash?????? I'm bad at threads, need to get gud lol
	//device->drop(); wth do i not have to do this for??? and when do i need to  lol?/?
	system("pause");
	return 0;
}