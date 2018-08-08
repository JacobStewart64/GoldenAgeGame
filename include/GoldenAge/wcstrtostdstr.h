#pragma once
#include <GoldenAge/debug.h>
#include <string>

std::string wcstrtostdstr16(const wchar_t* in)
{
	#ifdef _DEBUG
		unsigned int buffermax = wcslen(in);
		if (buffermax >= LOGINBUFFERMAX)
		{
			debug("widestring input too big! no >= 16");
			system("pause");
			exit(EXIT_FAILURE);
		}
	#endif
	char out[LOGINBUFFERMAX];
	wcstombs(out, in, LOGINBUFFERMAX);
	return out;
}