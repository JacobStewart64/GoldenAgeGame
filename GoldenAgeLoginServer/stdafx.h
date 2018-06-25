// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// TODO: reference additional headers your program requires here
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "C:/Users/cool_/Documents/cpp-httplib/httplib.h" //<path to your httplib.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <stdarg.h>
#include "Debug.hpp"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <thread>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
