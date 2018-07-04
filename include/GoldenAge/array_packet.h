#pragma once
#include <vector>


namespace ga {
	//hoping this makes creating new kinds of packets easy
	//variadic template buffer building from strings
	//overloaded dereference that returns vector<unsigned char> as char*
	//use the same pattern of arguments on both sides of communication
	//and the out_strings should all mirror the in strings.
	class array_packet {
		std::vector<unsigned char> packet;
		unsigned int index = 0;

		void zip_string(std::string str)
		{
			debug("packet add: ", str);
			packet.push_back(str.size());
			for (char c : str)
			{
				packet.push_back(c);
			}
		}

	public:
		//pass the size of all the args using the static helper
		//variadic args only strings
		array_packet() {}

		array_packet(unsigned int args_size)
		{
			debug("reserving ", args_size, " bytes");
			packet.reserve(args_size);
		}

		void from_buf(const char* buf, unsigned int size)
		{
			debug("reserving ", size, " bytes");
			packet.reserve(size);
			for (unsigned int i = 0; i < size; ++i)
			{
				packet.push_back(buf[i]);
			}
			debug("packet after from buffer: ", (const char*)&packet[0]);
		}

		//fill the packet with your string args, you can easily
		//retrieve them from the packet by calling get with the
		//same arguments where you receive this packet
		template<typename ...T>
		void fill(T ...args)
		{
			((void)zip_string(std::forward<T>(args)), ...);
		}

		std::string get()
		{
			std::string newstr;
			unsigned int length = index + packet[index] + 1;
			++index;
			for (; index < length; ++index)
			{
				newstr.push_back(packet[index]);
			}
			debug("got string: ", newstr);
			return newstr;
		}

		void add(std::string str)
		{
			debug("adding string: ", str);
			packet.push_back(str.size());
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
			int size = 0;
			((void)((size += args.size() + 1)), ...);
			return size;
		}

		//give packet buffer pointer
		const char* operator()()
		{
			return (const char*)&packet[0];
		}

		unsigned int size()
		{
			return packet.size();
		}
	};
};