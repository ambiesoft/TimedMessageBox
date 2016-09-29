// TimedMessageBox.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "TimedMessageBox.h"

HINSTANCE g_hModule;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			g_hModule = (HINSTANCE)hModule;
		}
		break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


#ifndef _In_
#define _In_
#endif

struct DialogParams {
	int nSec;
	LPCWSTR pTitle;
	LPCWSTR pMessage;

	DialogParams() {
		ZeroMemory(this, sizeof(this));
	}
};

#define countof(T) sizeof(T)/sizeof(T[0])

BOOL CALLBACK DlgProc(
  _In_ HWND   hDlg,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
	static UINT sTimerID;
	static DialogParams* spParams;
	static LPCWSTR spOK = L"OK";
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			spParams = (DialogParams*)lParam;
			SetWindowTextW(hDlg, spParams->pTitle);
			SetDlgItemTextW(hDlg, IDC_EDIT_MAIN, spParams->pMessage);

			sTimerID = SetTimer(hDlg, 1, 1000, NULL);
			return TRUE;
		}
		break;

		case WM_TIMER:
		{
			if(wParam != sTimerID)
				break;

			spParams->nSec--;
			if(spParams->nSec < 0)
			{
				KillTimer(hDlg, sTimerID);
				sTimerID=0;
				EndDialog(hDlg, IDOK);
				return 1;
			}

			WCHAR szT[256];
			wsprintfW(szT, L"%s (%d)", spOK, spParams->nSec);
			SetDlgItemTextW(hDlg, IDOK, szT);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
				{
					EndDialog(hDlg, IDOK);
					return 1;
				}
				break;

				case IDCANCEL:
				{
					EndDialog(hDlg, IDCANCEL);
					return 1;
				}
				break;
			}
			break;
		}
		break;
	}
	return 0;
}



// This is an example of an exported function.
TIMEDMESSAGEBOX_API int fnTimedMessageBox(int sec,
										  LPCWSTR pTitle,
										  LPCWSTR pMessage)
{
	DialogParams params;
	params.nSec = sec;
	params.pTitle = pTitle;
	params.pMessage = pMessage;
	return DialogBoxParamW(g_hModule,
		MAKEINTRESOURCEW(IDD_DIALOG_MAIN),
		NULL,
		DlgProc,
		(LPARAM)&params);
}


