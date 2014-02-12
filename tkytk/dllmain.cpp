// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "dllmain.h"


/*
int conv_enc(lua_State* L){
string inStr=luaL_checkstring(L,-3);
string inEnc=luaL_checkstring(L,-2);
string outEnc=luaL_checkstring(L,-1);
string outStr=w2s(s2w(inStr,inEnc),outEnc);
lua_pushstring(L, outStr.c_str());
return 1;
}
*/
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}




 BOOL __cdecl load(HGLOBAL h, long len){
	char* buf=new char[len+1];
	memcpy(buf,h,len);
	buf[len]=0;
	string param((char*)buf);
	GlobalFree(h);
	delete[] buf;

		L=luaL_newstate();
	 	luaL_openlibs(L);
//		lua_register(L,"encoding",conv_enc);
//		lua_register(L,"reqparse",reqparse);

		lua_getglobal(L,"package");
		lua_pushstring(L,(param+string("?.lua")).c_str());
		lua_setfield(L,-2,"path");
		lua_pop(L, 1);

		if(int r=luaL_loadfile(L, (param+string("index.lua")).c_str())){
			char hoge[100];
			sprintf(hoge,"(%d)",r);
			MessageBox(NULL,s2w((param+string("index.lua")+string(hoge))).c_str(),_T("起動に失敗"),MB_OK);
			return FALSE;
		}
		if(lua_pcall(L, 0, 0, 0)){
			MessageBox(NULL,_T("lua_proc"),_T("起動に失敗"),MB_OK);
			return FALSE;
		}



	BOOL ret;

		lua_getglobal(L,"load");
		lua_pushstring(L,param.c_str());
		lua_pcall(L, 1, 1, 0);
		ret=lua_toboolean(L, 1);
		lua_pop(L,1);
	return ret;
 }
 BOOL __cdecl unload(){
	BOOL ret;
		lua_getglobal(L,"unload");
		lua_pcall(L, 0, 1, 0);
		ret=lua_toboolean(L, 1);
		lua_pop(L,1);
		lua_close(L);

		return ret;
 }
 HGLOBAL __cdecl request(HGLOBAL h, long *len){
 	luaL_openlibs(L);

	char* buf=new char[*len+1];
	memcpy(buf,h,*len);
	buf[*len]=0;
	string param((char*)buf);
	GlobalFree(h);
	delete[] buf;

	lua_getglobal(L,"request");
	lua_pushstring(L,param.c_str());
	lua_pcall(L, 1, 1, 0);
	string ret=lua_tostring(L, 1);
	lua_pop(L,1);
	*len = ret.size()+1;
	buf=(char*)GlobalAlloc(GMEM_FIXED,*len);
	memcpy(buf,ret.c_str(),*len);
	((char*)buf)[*len-1]=0;
	return buf;
}

 
wstring s2w(string in,string cs){
	UINT csint;
	if(cs=="Shift_JIS"){
		csint=932;
	}else if(cs=="ISO-2022-JP"){
		csint=50221;
	}else if(cs=="EUC-JP"){
		csint=51932;
	}else if(cs=="UTF-8"){
		csint=65001;
	}else{
		csint=CP_ACP;
	}
	const int nSize = MultiByteToWideChar( csint, 0, (LPCSTR)in.c_str(), -1, NULL, 0 );
    TCHAR* buff = new TCHAR[ nSize +1 ];
    MultiByteToWideChar( csint, 0, (LPCSTR)in.c_str(), -1, (LPWSTR)buff, nSize );
	wstring ret(buff);
	delete[] buff;
	return ret;
}

string w2s(wstring in,string cs){
	UINT csint;
	if(cs=="Shift_JIS"){
		csint=932;
	}else if(cs=="ISO-2022-JP"){
		csint=50221;
	}else if(cs=="EUC-JP"){
		csint=51932;
	}else if(cs=="UTF-8"){
		csint=65001;
	}else{
		csint=CP_ACP;
	}
	const int nSize = WideCharToMultiByte( csint, 0, (LPCWSTR)in.c_str(), -1, NULL, 0, NULL, NULL );	 
    BYTE* buff = new BYTE[ nSize +1  ];
	WideCharToMultiByte( csint, 0, (LPCWSTR)in.c_str(), -1, (LPSTR)buff, nSize, NULL, NULL );
	string ret=string((char*)buff);
	delete[] buff;
	return ret;
}
