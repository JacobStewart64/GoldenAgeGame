#pragma once
#include <GoldenAge/toongraphics.h>
#include <string>

namespace ga {
	struct toon {
		std::string name;
		ga::toongraphics tg;

		toon(std::string& name)
		{
			this->name = name;
		}
	};
};