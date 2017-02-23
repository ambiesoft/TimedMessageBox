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
	UINT uType;
	TIMEDMESSAGEBOX_PARAMS* pTimedParams;
	DialogParams() {
		ZeroMemory(this, sizeof(*this));
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

			if(spParams->pTimedParams)
			{
				switch(spParams->pTimedParams->position)
				{
	
				case TIMEDMESSAGEBOX_POSITION_TOPLEFT:
					MoveWindowCommon(hDlg, MOVEWINDOW_TOPLEFT);
					break;

				case TIMEDMESSAGEBOX_POSITION_TOPRIGHT:
					MoveWindowCommon(hDlg, MOVEWINDOW_TOPRIGHT);
					break;

				case TIMEDMESSAGEBOX_POSITION_BOTTOMLEFT:
					MoveWindowCommon(hDlg, MOVEWINDOW_BOTTOMLEFT);
					break;

				case TIMEDMESSAGEBOX_POSITION_BOTTOMRIGHT:
					MoveWindowCommon(hDlg, MOVEWINDOW_BOTTOMRIGHT);
					break;

				case TIMEDMESSAGEBOX_POSITION_CENTERSCREEN:
					CenterWindow(hDlg,NULL);
					break;

				case TIMEDMESSAGEBOX_POSITION_CENTERPARENT:
					CenterWindow(hDlg, spParams->pTimedParams->hWndCenterParent);
					break;
				}
			}
			else
			{
				CenterWindow(hDlg,NULL);
			}
			if(spParams->uType & MB_SYSTEMMODAL)
			{
				SetWindowPos(hDlg,
					HWND_TOPMOST,
					0,0,0,0,
					SWP_NOMOVE|SWP_NOSIZE);
			}
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
TIMEDMESSAGEBOX_API int fnTimedMessageBox(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR pMessage,
										  UINT uType)
{
	return fnTimedMessageBox2(hWnd,sec,pTitle,pMessage,uType,NULL);
}

TIMEDMESSAGEBOX_API int fnTimedMessageBox2(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR pMessage,
										  UINT uType,
										  TIMEDMESSAGEBOX_PARAMS* pParams)
{
	DialogParams params;
	params.nSec = sec;
	params.pTitle = pTitle;
	params.pMessage = pMessage;
	params.uType = uType;
	params.pTimedParams=pParams;
	return DialogBoxParamW(g_hModule,
		MAKEINTRESOURCEW(IDD_DIALOG_MAIN),
		hWnd,
		DlgProc,
		(LPARAM)&params);
}


