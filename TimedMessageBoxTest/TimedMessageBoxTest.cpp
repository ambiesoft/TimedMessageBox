// TimedMessageBoxTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../TimedMessageBox/TimedMessageBox.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	HMODULE hModule = LoadLibrary("TimedMessageBox.dll");
	if(!hModule)
		return 1;

	
	FNTimedMessageBox func=NULL;
	func = (FNTimedMessageBox)GetProcAddress(hModule, "fnTimedMessageBox");
	if(!func)
		return 1;

	func(NULL, 10, L"title", L"message", MB_SYSTEMMODAL);




	FNTimedMessageBox2 func2=NULL;
	func2 = (FNTimedMessageBox2)GetProcAddress(hModule, "fnTimedMessageBox2");
	if(!func2)
		return 1;

	TIMEDMESSAGEBOX_PARAMS tp;
	tp.size = sizeof(tp);
	tp.hWndCenterParent = NULL;
	tp.position = TIMEDMESSAGEBOX_POSITION_BOTTOMRIGHT;

	func2(NULL, 10, L"title", L"message", MB_SYSTEMMODAL,&tp);


	return 0;
}



