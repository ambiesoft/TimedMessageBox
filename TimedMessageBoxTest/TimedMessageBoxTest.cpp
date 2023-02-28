// TimedMessageBoxTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../TimedMessageBox/TimedMessageBox.h"
#include "../../lsMisc/HighDPI.h"

using namespace std;

void errorExit(wstring message)
{
	MessageBox(NULL, message.c_str(), L"TimedMessageBoxTest", MB_ICONERROR);
	exit(1);
}

BOOL debugKocchi()
{
	HWND hWnd = NULL;
	const wchar_t APP_NAME[] = L"appname";
	HMODULE hModule = LoadLibrary(_T("TimedMessageBox.dll"));
	if (!hModule)
		return TRUE;

	FNTimedMessageBox func = NULL;
	func = (FNTimedMessageBox)GetProcAddress(hModule, "fnTimedMessageBox");
	if (!func)
		return TRUE;

	if (IDOK != func(hWnd, 300, APP_NAME, L"message"))
		return FALSE;
	return TRUE;
}

void testCount(FNTimedMessageBox2 func2, int count)
{
	wstring result;
	{
		TIMEDMESSAGEBOX_PARAMS tp = { 0 };
		tp.size = sizeof(tp);
		tp.flags = TIMEDMESSAGEBOX_FLAGS_POSITION |
			TIMEDMESSAGEBOX_FLAGS_SHOWCMD |
			TIMEDMESSAGEBOX_FLAGS_TOPMOST |
			TIMEDMESSAGEBOX_FLAGS_HIDECOPY |
			TIMEDMESSAGEBOX_FLAGS_HIDERETRY |
			TIMEDMESSAGEBOX_FLAGS_KEEPTOPMOST;
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
		DWORD ret = func2(NULL, count, L"title", message.c_str(), &tp);

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
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	Ambiesoft::InitHighDPISupport();

	debugKocchi();

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

	testCount(func2, 10);
	testCount(func2, -1);
	return 0;
}



