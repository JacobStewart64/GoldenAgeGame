#pragma once
#include <GoldenAge/debug.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/toon.h>
#include <irrlicht.h>
#include <unordered_map>


namespace ga {
	class GameEventReceiver : public irr::IEventReceiver
	{
		LOOP_PARAM_PACK& LOOP_PACK;

	public:
		GameEventReceiver(LOOP_PARAM_PACK& LOOP_PACK) : LOOP_PACK(LOOP_PACK) {}

		virtual bool OnEvent(const irr::SEvent& event)
		{
			if (event.EventType == irr::EET_GUI_EVENT)
			{
				int id = event.GUIEvent.Caller->getID();
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