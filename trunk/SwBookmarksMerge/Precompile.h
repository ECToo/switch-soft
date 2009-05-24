// Precompile.h
#pragma once


// C ISO
#include <cstdlib>
#include <cstring>
#include <cstdarg>


// C++ ISO
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <functional>
#include <iterator>
#include <hash_map>

using namespace std;
using namespace stdext;


// internal
#include "swStringStuff.h"
#include "swException.h"


typedef vector<char> BytesType;
typedef BytesType::iterator BytesTypeIt;
typedef pair<LazyString,LazyString> AttribType;

// ============================================================================
//  DeleteObject
// ============================================================================
struct DeleteObject
{
	template<typename T> void operator()(const T* ptr) const
	{
		delete ptr;
	}
};

// EOF