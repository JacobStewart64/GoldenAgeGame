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

		void zip_string(std::string& str)
		{
			for (char c : str)
			{
				packet.push_back(c);
			}
			packet.push_back('\0');
		}

		void unzip_string(std::string& out_str, unsigned int& start_len)
		{
			for (; packet[start_len] != '\0'; ++start_len)
			{
				out_str.push_back(packet[start_len]);
			}
			out_str.push_back('\0');
		}

	public:
		//pass the size of all the args using the static helper
		//variadic args only strings
		array_packet(unsigned int args_size)
		{
			packet.reserve(args_size);
		}

		//fill the packet with your string args, you can easily
		//retrieve them from the packet by calling get with the
		//same arguments where you receive this packet
		template<typename ...T>
		void fill(T ...args)
		{
			((void)zip_string(args), ...);
		}

		template<typename ...T>
		void get(unsigned int len = 0, T ...args)
		{
			
			((void)unzip_string(args, len), ...);
		}

		//helper to get the needed conversion to size of args + their null chars
		//only for strings
		template<typename ...T>
		static unsigned int get_args_size(T ...args)
		{
			int size = 0;
			((void)(size += std::forward<T>(args).size()), ...);
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