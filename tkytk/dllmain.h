
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <Windows.h>
#include <vector>
#include <regex>
#include <string>
#include <map>
#include "lua-5.2.3/src/lua.hpp"
#include <tchar.h>

using namespace std;

lua_State *L;
//vector<masterRecord> RANDOMcore;
//vector<masterRecord> TALKcore;
extern "C" __declspec(dllexport) BOOL __cdecl load(HGLOBAL h, long len);
extern "C" __declspec(dllexport) BOOL __cdecl unload();
extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len);

static string w2s(wstring in,string cs="UTF-8");
static wstring s2w(string in,string cs="UTF-8");
