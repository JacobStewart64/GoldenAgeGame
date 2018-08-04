// SharedTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <irrlicht.h>
#include <gtest/gtest.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/toon.h>
#include <fstream>
#include <thread>

TEST(UDP_COM, Doesnt_mess_up_toon_data)
{
	#define MAXCLIENTS 1000
	#define MAXCHANNELS 2
	ga::toon t(true);
	std::ofstream new_toon("./new_toon.txt");
	new_toon << t;
	new_toon.close();

	std::ifstream toon_file("./new_toon.txt");
	ga::toon toon;
	toon_file >> toon;
	toon_file.close();

	std::string type = "t";
	std::string type2;

	std::string out_toon_str;
	
	unsigned int grun = true;

	ga::udp_com com_out(MAXCLIENTS, MAXCHANNELS, 3001, true);

	com_out.register_connect([&com_out, &toon, &type](ENetPeer* peer) {
		ga::array_packet packet(ga::array_packet::get_args_size(type, toon.to_string()));
		packet.fill(type, toon.to_string());
		com_out.send(peer, packet(), packet.size(), ENET_PACKET_FLAG_RELIABLE);
	});

	#define MAXCLIENTS 1
	#define MAXCHANNELS 2
	ga::udp_com com_in(MAXCLIENTS, MAXCHANNELS);

	com_in.register_receive([&type2, &out_toon_str, &grun](ENetEvent& e) {
		ga::array_packet packet;
		packet.from_buf((const char*)e.packet->data, e.packet->dataLength);
		type2 = packet.get();
		out_toon_str = packet.get();
		grun = false;
	});

	std::thread thr([&com_out, &grun]() {
		com_out.listenLoop(grun, 0);
	});
	Sleep(15);
	com_in.connect("localhost", 3001, 60 * 1000 * 15);

	com_in.listenLoop(grun, 0);
	thr.join();

	EXPECT_EQ(type, type2) << "The type was not the same!";
	EXPECT_EQ(toon.to_string(), out_toon_str) << "ENET messed up the toon data!";
	ga::toon from_toon;
	from_toon.from_string(out_toon_str);
	EXPECT_EQ(toon.to_string(), from_toon.to_string()) << "Couldn't make a toon with the data!";
}

TEST(ArrayPacket, Doesnt_mess_up_toon_data)
{
	ga::toon t(true);
	std::ofstream new_toon("./new_toon.txt");
	new_toon << t;
	new_toon.close();

	std::ifstream toon_file("./new_toon.txt");
	ga::toon toon;
	toon_file >> toon;
	toon_file.close();

	std::string type = "t";
	ga::array_packet packet(ga::array_packet::get_args_size(type, toon.to_string()));
	packet.fill(type, toon.to_string());
	std::string newtype = packet.get();
	EXPECT_EQ(type, newtype) << "type did not come out right!";
	std::string part = packet.get();
	EXPECT_EQ(toon.to_string(), part) << "toon data did not come out of array packet properly!";

	ga::array_packet pack;
	pack.add(type);
	pack.add(toon.to_string());
	newtype = pack.get();
	part = pack.get();
	EXPECT_EQ(type, newtype) << "adding: type did not come out right!";
	EXPECT_EQ(toon.to_string(), part) << "adding: toon data did not come out of array packet properly!";
}

TEST(Toon_From_And_To_String, doesnt_mess_up_toon_data)
{
	ga::toon t(true);
	std::ofstream new_toon("./new_toon.txt");
	new_toon << t;
	new_toon.close();

	std::ifstream toon_file("./new_toon.txt");
	ga::toon toon;
	toon_file >> toon;
	toon_file.close();

	ga::toon from_toon;
	from_toon.from_string(toon.to_string());

	EXPECT_EQ(toon.to_string(), from_toon.to_string()) << "from string did not work!";
	EXPECT_EQ(toon.pos.col, from_toon.pos.col) << "col was equal";
	EXPECT_EQ(toon.pos.row, from_toon.pos.row) << "row was equal";
}

TEST(ToonLoad, doesnt_mess_up_toon_data)
{
	//load a toon twice and compare toons for equality
	ga::toon t(true);
	std::ofstream new_toon("./new_toon.txt");
	new_toon << t;
	new_toon.close();

	std::ifstream toon_file("./new_toon.txt");
	ga::toon toon;
	toon_file >> toon;
	toon_file.close();

	std::ifstream toon_file2("./new_toon.txt");
	ga::toon toon2;
	toon_file2 >> toon2;
	toon_file2.close();
	EXPECT_EQ(toon2.to_string(), toon.to_string()) << "same toon was not equal";
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
	system("pause");
	return ret;
}

