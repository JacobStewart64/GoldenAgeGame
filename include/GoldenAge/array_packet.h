#pragma once
#include <vector>
#include <GoldenAge/Debug.h>


namespace ga {
	//for dev only: highly not efficient
	//hoping this makes creating new kinds of packets easy
	//variadic template buffer building from strings
	//use the same pattern of arguments on both sides of the network
	//in == out.
	//designed to handle null chars and other anomalies.
	//thinking about memcpy c-string by hand in production? idk
	class array_packet {
		std::vector<unsigned char> packet;
		unsigned int index = 0;

	public:
		//pass the size of all the args using the static helper
		//variadic args only strings
		array_packet() {}

		array_packet(unsigned int args_size)
		{
			debug("reserving ", args_size, " bytes for array packet.");
			packet.reserve(args_size);
		}

		void from_buf(const char* buf, unsigned int size)
		{
			debug("reserving ", size, " bytes for array packet");
			packet.reserve(size);
			for (unsigned int i = 0; i < size; ++i)
			{
				packet.push_back(buf[i]);
			}
		}

		//fill the packet with your string args, you can easily
		//retrieve them from the packet by calling get with the
		//same arguments where you receive this packet
		template<typename ...T>
		void fill(T ...arg_pack)
		{
			debug("adding args to array packet");
			((debug(arg_pack), add(std::forward<T>(arg_pack))), ...);
		}

		std::string get()
		{
			std::string newstr;
			unsigned int length;
			unsigned char* lref = (unsigned char*)&length;
			unsigned int offset = (unsigned int)lref + 4;
			for (; (unsigned int)lref < offset; ++lref, ++index)
			{
				*lref = packet[index];
			}
			length += index;
			for (; index < length; ++index)
			{
				newstr.push_back(packet[index]);
			}
			debug("got string: ", newstr);
			return newstr;
		}

		unsigned int getInt()
		{
			unsigned int length;
			unsigned char* lref = (unsigned char*)&length;
			unsigned int offset = (unsigned int)lref + 4;
			for (; (unsigned int)lref < offset; ++lref, ++index)
			{
				*lref = packet[index];
			}
			return length;
		}

		void add(std::string str)
		{
			debug("adding string: ", str);
			unsigned int length = str.size();
			unsigned char* lref = (unsigned char*)&length;
			unsigned int offset = (unsigned int)lref + 4;
			for (; (unsigned int)lref < offset; ++lref)
			{
				packet.push_back(*lref);
			}
			for (char c : str)
			{
				packet.push_back(c);
			}
		}

		//helper to get the needed conversion to size of args + their null chars
		//only for strings
		template<typename ...T>
		static unsigned int get_args_size(T ...args)
		{
			debug("getting args size for array packet");
			int size = 0;
			((debug(size), size += args.size() + 4), ...);
			debug(size);
			return size;
		}

		//give packet buffer pointer
		void* operator()()
		{
			debug("getting packet:\n", std::string((char*)&packet[0], 0, packet.size()));
			return &packet[0];
		}

		unsigned int size()
		{
			debug("returning packet size: ", packet.size());
			return packet.size();
		}
	};
};