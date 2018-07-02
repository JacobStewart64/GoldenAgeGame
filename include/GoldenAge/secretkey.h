#pragma once
#include <vector>
#include <openssl/rand.h>

namespace ga {
	struct secretkey {
		unsigned char buffer[256];

		secretkey() {}

		secretkey(std::string established_key)
		{
			strcpy((char*)buffer, established_key.c_str());
		}

		void generate_random_bytes()
		{
			RAND_bytes(buffer, 255);
			buffer[255] = '\0';
		}

		std::string to_string() {
			return (char*)buffer;
		}
	};
};