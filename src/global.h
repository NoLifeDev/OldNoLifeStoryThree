//////////////////////////////////////////////////////////////////////////
// Copyright 2012 Peter Atechian (Retep998)                             //
//////////////////////////////////////////////////////////////////////////
// This file is part of NoLifeStory.                                    //
//                                                                      //
// NoLifeStory is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// NoLifeStory is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with NoLifeStory.  If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////

#pragma region Platform
#ifdef NLS_GLOBAL
#error "Global.h was included more than once"
#endif
#define NLS_GLOBAL

#ifdef _WIN32
#define NLS_WINDOWS
#else
#error "Unknown platform."
#endif

#ifdef _MSC_VER
#define NLS_MSVC
#if _MSC_VER >= 1700
#define NLS_CPP11
#else
#error "Please upgrade your Visual Studio"
#endif
#else
#error "Unknown compiler."
#endif

#define _CRT_SECURE_NO_WARNINGS 1
#pragma endregion

//Zlib
#include <zlib.h>

//SFML
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

//GLEW
#include <GL/glew.h>

//Bass
#include <bass.h>

//C Standard Library
//#include <cassert>
//#include <ccomplex>
//#include <cctype>
//#include <cerrno>
//#include <cfloat>
//#include <chrono>
//#include <ciso646>
//#include <climits>
//#include <clocale>
//#include <cmath>
//#include <csetjmp>
//#include <csignal>
//#include <cstdarg>
//#include <cstddef>
//#include <cstdint>
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <ctime>
//#include <cwchar>
//#include <cwctype>

//C++ Standard Library
#include <algorithm>
#include <allocators>
#include <array>
#include <atomic>
#include <bitset>
#include <codecvt>
#include <complex>
#include <condition_variable>
#include <deque>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <hash_map>
#include <hash_set>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <strstream>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <vector>
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace std::tr1;
using namespace std::tr2;
using namespace std::tr2::sys;

//Do some type checks
#ifdef SFML_ENDIAN_BIG
#error "Why are you using big endian???"
#endif
static_assert(sizeof(char) == 1, "Unsupported char size");
static_assert(sizeof(wchar_t) == 2, "Unsupported wchar_t size");

//And resources
#ifdef NLS_WINDOWS
#include "../res/Resource.h"
#endif

#define die() throw(273)

#include "AES.h"
#include "MapFile.h"
#include "Game.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Time.h"
#include "Crypto.h"
#include "Node.h"
#include "WZ.h"
#include "Img.h"
#include "Network.h"