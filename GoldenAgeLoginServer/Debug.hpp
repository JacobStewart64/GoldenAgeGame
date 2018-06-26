#pragma once

#include <string>
#include <iostream>

using namespace std;

//if debug isn't defined, turns into a no-op
template <typename ...T>
void debug(string file, int line, T&& ...args)
{
	#ifdef _DEBUG
		unsigned int index = file.find_last_of('\\');
		file[index] = '/';
		index = file.find_last_of('\\', --index);
		string finalfile = file.substr(index);
		finalfile[0] = '/';
		cout << finalfile << ":" << line << " ";
		using expander = int[];
		(void)expander {
			0, (void(cout << std::forward<T>(args)), 0)...
		};
		cout << endl;
	#endif
}

//use this, it outputs the file and line the output was made from!
#define debug(...) debug(__FILE__, __LINE__, __VA_ARGS__);