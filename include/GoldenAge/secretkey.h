#pragma once
#include <vector>
#include <openssl/rand.h>

namespace ga {
	struct secretkey {
		unsigned char buffer[256];

		secretkey() {}

		secretkey(std::string& established_key)
		{
			from_string(established_key);
		}

		void generate_random_bytes()
		{
			RAND_bytes(buffer, 256);
		}

		std::string to_string() {
			std::string newstr;
			for (int i = 0; i < 256; ++i)
			{
				newstr.push_back(buffer[i]);
			}
			return newstr;
		}

		void from_string(std::string& sk)
		{
			for (int i = 0; i < 256; ++i)
			{
				buffer[i] = sk[i];
			}			
		}
	};
};