#ifndef ENGINE_INCLUDE_H__
#define ENGINE_INCLUDE_H__

#pragma warning (disable : 4251)

#include <d3dx9.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <io.h>

#include "iostream"
using namespace std;

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
//DH_Fmode_commentLib_180702_Setting to Fmode
#pragma comment(lib, "fmodex_vc.lib")

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"
//DH_Fmode_Include_180702_Setting to Fmode
#include "fmod.h"
#include "fmod.hpp"

#endif // ENGINE_INCLUDE_H__


