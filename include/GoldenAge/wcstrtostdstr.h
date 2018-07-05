#pragma once
#include <GoldenAge/debug.h>
#include <string>

std::string wcstrtostdstr16(const wchar_t* in)
{
	#ifdef _DEBUG
		unsigned int buffermax = wcslen(in);
		if (buffermax > 16)
		{
			debug("widestring input too big! no > 16");
			system("pause");
			exit(EXIT_FAILURE);
		}
	#endif
	char out[16];
	wcstombs(out, in, 16);
	return out;
}