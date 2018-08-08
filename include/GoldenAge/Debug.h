#pragma once
#include <string>
#include <iostream>
#include <functional>

void logFileAndLine(char* file, int line)
{
	unsigned int index = strlen(file);
	for (int i = index - 1; i >= 0; --i)
	{
		if (file[i] == '\\')
		{
			index = i;
			break;
		}
	}
	file[index] = '/';
	for (int i = index - 1; i >= 0; --i)
	{
		if (file[i] == '\\')
		{
			index = i;
			break;
		}
	}
	file[index] = '/';
	std::cout << &file[index] << ":" << line << " ";
}

void DEBUG(std::function<void()> cb)
{
	#ifdef _DEBUG
		cb();
	#endif
}

#define DEBUG(x)   DEBUG((x))

//if debug isn't defined, turns into a no-op
template <typename ...T>
void debug(const char* file, int line, T&& ...arg_pack)
{
	#ifdef _DEBUG
		logFileAndLine(file, line);
		((std::cout << std::forward<T>(arg_pack)), ...);
		std::cout << std::endl;
	#endif
}

//use this, it outputs the file and line the output was made from!
#define debug(...) debug(__FILE__, __LINE__, __VA_ARGS__)