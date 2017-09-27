// TimedMessageBoxTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../TimedMessageBox/TimedMessageBox.h"
// #include "../../lsMisc/CreateSimpleWindow.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: Place code here.
	HMODULE hModule = LoadLibrary(_T("TimedMessageBox.dll"));
	if(!hModule)
		return 1;

	
	//FNTimedMessageBox func=NULL;
	//func = (FNTimedMessageBox)GetProcAddress(hModule, "fnTimedMessageBox");
	//if(!func)
	//	return 1;

	//func(NULL, 10, L"title", L"message");






	FNTimedMessageBox2 func2=NULL;
	func2 = (FNTimedMessageBox2)GetProcAddress(hModule, "fnTimedMessageBox2");
	if(!func2)
		return 1;

	TIMEDMESSAGEBOX_PARAMS tp = {0};
	tp.size = sizeof(tp);
	tp.flags = TIMEDMESSAGEBOX_FLAGS_POSITION|TIMEDMESSAGEBOX_FLAGS_SHOWCMD|TIMEDMESSAGEBOX_FLAGS_TOPMOST;
	tp.hWndCenterParent = NULL;
	tp.position = TIMEDMESSAGEBOX_POSITION_BOTTOMRIGHT;
	tp.nShowCmd = SW_SHOWNOACTIVATE;

	func2(NULL, 10, L"title", L"message", &tp);


	return 0;
}



