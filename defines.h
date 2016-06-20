#include <windows.h>
#include "resource.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")

// Flash Player hexed up....
#include "flvplayer.h"

char szDll[]           = "atl.dll";
char szAtlAxWinInit[]  = "AtlAxWinInit";
char szAtlAxWin[]      = "AtlAxWin";
char szPlayer[]        = "player.html";
char szflvplayer[]     = "flvplayer.swf";
char szBksl[]          = "\\";
char szHeader[512]     = {0};
char CurrDir[MAX_PATH] = {0};
char path[MAX_PATH]    = {0};
char pathflv[MAX_PATH] = {0};
char FileBuffer[260]   = {0};
char lpOut[MAX_PATH]   = {0};
DWORD lBytesWriten;
HWND play;
HINSTANCE hIns;

// DLL Loading vars
HMODULE hDLL;
int(__cdecl* AtlAxWinInit) ();


// Function to load Text files from resource
void LoadFileInResource(int name, int type, DWORD& size, const char*& data)
{
    HMODULE handle = GetModuleHandle(NULL);
    HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name),MAKEINTRESOURCE(type));
    HGLOBAL rcData = LoadResource(handle, rc);
    size = SizeofResource(handle, rc);
    data = static_cast<const char*>(LockResource(rcData));
}

void LoadIntoBuffer(char buffer[],int name, int type)
{
	DWORD size = 0;
	const char* data = NULL;
	LoadFileInResource(name,type, size, data);
	// The text stored in the resource might not be NULL terminated.
	memcpy(buffer, data, size);
	buffer[size] = 0; // NULL terminator
}

void WritePlayer(HWND hWnd)
{
	HANDLE hPlayer;
	hPlayer=CreateFile(pathflv,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	if(hPlayer != INVALID_HANDLE_VALUE)
	{
		CancelIo(hPlayer);
		WriteFile(hPlayer,player,33477,&lBytesWriten,0);
		CloseHandle(hPlayer);
	}

}

void WriteHTML(HWND hWnd)
{
	HANDLE handle;
	DeleteFile(path);
	handle=CreateFile(path,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	if(handle != INVALID_HANDLE_VALUE)
	{
		CancelIo(handle); // Cancel I/O from same handle...


		wsprintf(lpOut,szHeader,FileBuffer);
		int len=lstrlen(lpOut);

		WriteFile(handle,lpOut,len,&lBytesWriten,0);

		CloseHandle(handle); // Close the handle

		WritePlayer(hWnd); // Now write the player from header file flvplayer.h
	}
}

void Play(HWND hWnd)
{
	play=CreateWindowEx(0,szAtlAxWin,path,WS_CHILD | WS_VISIBLE,0,0,435,299,hWnd,NULL,hIns,NULL);
}