// TimedMessageBoxTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../TimedMessageBox/TimedMessageBox.h"
// #include "../../lsMisc/CreateSimpleWindow.h"

using namespace std;

void errorExit(wstring message)
{
	MessageBox(NULL, message.c_str(), L"TimedMessageBoxTest", MB_ICONERROR);
	exit(1);
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	HMODULE hModule = LoadLibrary(_T("TimedMessageBox.dll"));
	if (!hModule)
	{
		errorExit(L"Module unloadable");
	}

	FNTimedMessageBox2 func2=NULL;
	func2 = (FNTimedMessageBox2)GetProcAddress(hModule, "fnTimedMessageBox2");
	if (!func2)
	{
		errorExit(L"function not found");
	}

	wstring result;
	{
		TIMEDMESSAGEBOX_PARAMS tp = { 0 };
		tp.size = sizeof(tp);
		tp.flags = TIMEDMESSAGEBOX_FLAGS_POSITION | TIMEDMESSAGEBOX_FLAGS_SHOWCMD | TIMEDMESSAGEBOX_FLAGS_TOPMOST;
		tp.hWndCenterParent = NULL;
		tp.position = TIMEDMESSAGEBOX_POSITION_BOTTOMRIGHT;
		tp.nShowCmd = SW_SHOWNOACTIVATE;

		wstring message;
		for (int i = 1; i < 10; ++i)
		{
			message += L"message";
			message += std::to_wstring(i);
			message += L"\r\n";
		}
		DWORD ret = func2(NULL, 10, L"title", message.c_str(), &tp);


		switch (ret)
		{
		case IDOK:
			result += L"OK";
			break;
		case IDCANCEL:
			result += L"Cancel";
			break;
		case IDRETRY:
			result += L"Retry";
			break;
		default:
			result += L"Unknown";
			break;
		}
		if (tp.nTimeout)
			result += L" (Timedout)";
	}

	// show result on top-right
	{
		TIMEDMESSAGEBOX_PARAMS tp = { 0 };
		tp.size = sizeof(tp);
		tp.flags = TIMEDMESSAGEBOX_FLAGS_POSITION | TIMEDMESSAGEBOX_FLAGS_SHOWCMD | TIMEDMESSAGEBOX_FLAGS_TOPMOST;
		tp.hWndCenterParent = NULL;
		tp.position = TIMEDMESSAGEBOX_POSITION_TOPRIGHT;
		tp.nShowCmd = SW_SHOWNOACTIVATE;

		DWORD ret = func2(NULL, 10, L"title", result.c_str(), &tp);
	}
	return 0;
}



