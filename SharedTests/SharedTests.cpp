// SharedTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <irrlicht.h>
#include <gtest/gtest.h>
#include <GoldenAge/array_packet.h>
#include <GoldenAge/udp_com.h>
#include <GoldenAge/toon.h>
#include <GoldenAge/secretkey.h>
#include <GoldenAge/cryptinfo.h>
#include <fstream>
#include <thread>

TEST(cryptinfo, decrypt_and_encrypt)
{
	ga::cryptinfo ci;
	ci.fillkeyiv();
	ga::secretkey sk;
	sk.generate_random_bytes();
	std::string skstr = sk.to_string();
	std::string encrypt_out;
	ci.encrypt(skstr, encrypt_out);
	std::string plaintext;
	ci.decrypt(encrypt_out, plaintext);
	EXPECT_EQ(skstr, plaintext) << "encrypt decrypt altered the string!";
}

TEST(Secret_Key, from_and_to_string_doesnt_mess_it_up)
{
	ga::secretkey sk;
	sk.generate_random_bytes();
	ga::secretkey sk2;
	std::string tmp = sk.to_string();
	sk2.from_string(tmp);
	EXPECT_EQ(sk.to_string(), sk2.to_string()) << "sk strings were not equal";
}

TEST(Secret_Key, getting_data_out_of_http_packet_doesnt_mess_it_up)
{
	std::string c = "c";
	ga::cryptinfo ci;
	ci.fillkeyiv();
	ga::secretkey sk;
	sk.generate_random_bytes();
	std::string data = c + " " + ci.keyToString() + " " + ci.ivToString() + " " + sk.to_string() + " ";
	debug(sk.to_string().size());
	std::string type(data.substr(0, 1), 0, 1);
	std::string key(data.substr(2, 32), 0, 32);
	debug("key: ", key, "see space?");
	debug("key size: ", key.size());
	std::string iv(data.substr(35, 16), 0, 16);
	debug("iv: ", iv, "see space?");
	debug("iv size: ", iv.size());
	std::string skstr(data.substr(52, 256), 0, 256);
	debug("skstr: ", skstr, "see space?");
	debug("skstr size: ", skstr.size());
	ci.keyFromString(key);
	debug("passed key");
	ci.ivFromString(iv);
	debug("passed iv");
	sk.from_string(skstr);
	debug("passed sk");
	EXPECT_EQ(sk.to_string(), skstr) << "sk strings were not equal!";
	EXPECT_EQ(type + " " + key + " " + iv + " " + skstr + " ", data) << "new data not equal old data!";
	EXPECT_EQ(ci.keyToString(), key) << "keys were not equal!";
	EXPECT_EQ(ci.ivToString(), iv) << "iv were not equal!";
}

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

