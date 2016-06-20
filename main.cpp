/*
FLV Player by Tejashwi Kalp Taru.
Originated from FLV Player by RagDOG coded in Assembly.
All credit goes to RagDOG.
*/

#include "defines.h"

BOOL CALLBACK DlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
void Dispatcher(HWND hWnd,WPARAM wParam);


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	InitCommonControls();
	hIns=hInstance;
	// Loading ATL DLL...
	hDLL=LoadLibrary(szDll);
	if(hDLL)
	{
		*(FARPROC*)&AtlAxWinInit=GetProcAddress(hDLL,szAtlAxWinInit);
		if(AtlAxWinInit)
			AtlAxWinInit();
	}
	else
		FreeLibrary(hDLL);
	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DLGPROC(DlgProc),0);
}

BOOL CALLBACK DlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			

			// Load all HTML texts into buffer...
			LoadIntoBuffer(szHeader,IDR_TEXTFILE1,133);

			GetCurrentDirectory(sizeof CurrDir,CurrDir);
			lstrcat(CurrDir,szBksl);
			lstrcpy(path,CurrDir);
			lstrcat(path,szPlayer);
			lstrcpy(pathflv,CurrDir);
			lstrcat(pathflv,szflvplayer);
			return true;
		case WM_COMMAND:
			Dispatcher(hWnd,wParam);
			return true;
		case WM_CLOSE:
			DeleteFile(path);
	        DeleteFile(pathflv);
			EndDialog(hWnd,0);
			return true;
	}
	return false;
}

void Dispatcher(HWND hWnd,WPARAM wParam)
{
	OPENFILENAME ofn;
	switch((LOWORD(wParam)))
	{
		case IDC_BUTTON1:
			{
				ZeroMemory(&ofn,sizeof ofn);
				ofn.lStructSize = sizeof ofn;
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = "FLV Videos\0*.flv\0\0";
				ofn.lpstrFile = FileBuffer;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
				if(GetOpenFileName(&ofn))
				{
					SetDlgItemText(hWnd,IDC_EDIT1,FileBuffer);
					WriteHTML(hWnd);
					DestroyWindow(play);
					Play(hWnd);
				}
			}
			break;
	}

}
