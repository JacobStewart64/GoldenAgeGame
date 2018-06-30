#pragma once
#include <string>
#include <iostream>

//if debug isn't defined, turns into a no-op
template <typename ...T>
void debug(std::string file, int line, T&& ...args)
{
	#ifdef _DEBUG
		unsigned int index = file.find_last_of('\\');
		file[index] = '/';
		index = file.find_last_of('\\', --index);
		std::string finalfile = file.substr(index);
		finalfile[0] = '/';
		std::cout << finalfile << ":" << line << " ";
		using expander = int[];
		(void)expander {
			0, (void(std::cout << std::forward<T>(args)), 0)...
		};
		std::cout << std::endl;
	#endif
}

//use this, it outputs the file and line the output was made from!
#define debug(...) debug(__FILE__, __LINE__, __VA_ARGS__)