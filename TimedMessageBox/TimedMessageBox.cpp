// TimedMessageBox.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "TimedMessageBox.h"

using namespace Ambiesoft;

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



struct DialogParams {
	int nSec;
	LPCWSTR pTitle;
	LPCWSTR pMessage;
	int nDialogResult;
	bool timedout;
	TIMEDMESSAGEBOX_PARAMS* pTimedParams;
	DialogParams() {
		ZeroMemory(this, sizeof(*this));
	}
};

#define countof(T) sizeof(T)/sizeof(T[0])


INT_PTR CALLBACK DlgProc(
  HWND   hDlg,
  UINT   uMsg,
  WPARAM wParam,
  LPARAM lParam
)
{
	static UINT_PTR sTimerID;
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

			PostMessage(hDlg, WM_APP_AFTERINIT, 0,0);

			
			if(spParams->pTimedParams && 
				(spParams->pTimedParams->nShowCmd==SW_SHOWNA || spParams->pTimedParams->nShowCmd==SW_SHOWNOACTIVATE))
			{
				SetWindowLong(hDlg, GWL_EXSTYLE, 
					GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
			}

			// Font size =* 1.2
			HFONT hFontEdit = (HFONT)SendDlgItemMessage(hDlg, IDC_EDIT_MAIN, WM_GETFONT, 0, 0);
			if (hFontEdit)
			{
				LOGFONT logFont = { 0 };
				if (0 != GetObject(hFontEdit, sizeof(logFont), &logFont))
				{
					logFont.lfHeight = (LONG)((logFont.lfHeight * 100 * 1.2) / 100);
					logFont.lfWidth = (LONG)((logFont.lfWidth * 100 * 1.2) / 100);
					HFONT hNewFont = CreateFontIndirect(&logFont);
					if (hNewFont)
					{
						SendDlgItemMessage(hDlg, IDC_EDIT_MAIN, WM_SETFONT, (WPARAM)hNewFont, 0);
					}
				}
			}

			ShowWindow(GetDlgItem(hDlg, IDC_BUTTON_COPY),
				(spParams->pTimedParams && (spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_HIDECOPY))
				? SW_HIDE : SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_BUTTON_KEEP),
				(spParams->pTimedParams && (spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_HIDEKEEP))
				? SW_HIDE : SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDRETRY),
				(spParams->pTimedParams && (spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_HIDERETRY))
				? SW_HIDE : SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDOK),
				(spParams->pTimedParams && (spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_HIDEOK))
				? SW_HIDE : SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDCANCEL),
				(spParams->pTimedParams && (spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_HIDECANCEL))
				? SW_HIDE : SW_SHOW);

			return TRUE;
		}
		break;

		case WM_APP_AFTERINIT:
		{
			//SetWindowLong(hDlg, GWL_EXSTYLE, (GetWindowLong(hDlg, GWL_EXSTYLE)&~(WS_EX_TOPMOST | WS_EX_NOACTIVATE)));
			if(spParams->pTimedParams)
			{
				if((spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_POSITION)!=0)
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
				
				//int nShowCmd = SW_SHOW;
				//if(spParams->pTimedParams)
				//{
				//	if(spParams->pTimedParams->flags  & TIMEDMESSAGEBOX_FLAGS_LOGFONT)
				//	{
				//	}

				//	if(spParams->pTimedParams->flags  & TIMEDMESSAGEBOX_FLAGS_SHOWCMD)
				//	{
				//		nShowCmd = spParams->pTimedParams->nShowCmd;
				//	}
				//}
				//ShowWindow(hDlg,nShowCmd);

				if(spParams->pTimedParams->flags & TIMEDMESSAGEBOX_FLAGS_TOPMOST)
				{
					SetWindowPos(hDlg,
						HWND_TOPMOST,
						0,0,0,0,
						SWP_NOMOVE|SWP_NOSIZE);
				}

			}
			else
			{
				CenterWindow(hDlg,NULL);
				ShowWindow(hDlg, SW_SHOW);
			}
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
				spParams->nDialogResult = IDOK;
				spParams->timedout = true;
				// DestroyWindow(hDlg);
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
				case IDC_BUTTON_COPY:
				{
					SetClipboardText(hDlg, spParams->pMessage);
				}
				break;

				case IDC_BUTTON_KEEP:
				{
					KillTimer(hDlg, sTimerID);
					sTimerID = 0;
					SetDlgItemTextW(hDlg, IDOK, spOK);
				}
				break;

				case IDOK:
				{
					spParams->nDialogResult = IDOK;
					spParams->timedout = false;
					EndDialog(hDlg, IDOK);
					return TRUE;
				}
				break;

				case IDCANCEL:
				{
					spParams->nDialogResult = IDCANCEL;
					spParams->timedout = false;
					EndDialog(hDlg, IDCANCEL);
					return TRUE;
				}
				break;

				case IDRETRY:
				{
					spParams->nDialogResult = IDRETRY;
					spParams->timedout = false;
					EndDialog(hDlg, IDRETRY);
					return TRUE;
				}
				break;
			}
			break;
		}
		break;

		//case WM_DESTROY:
		//{
		//	PostQuitMessage(0);
		//}
		//break;
	}
	return 0;
}



// This is an example of an exported function.
TIMEDMESSAGEBOX_API DWORD fnTimedMessageBox(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR pMessage)

{
	return fnTimedMessageBox2(hWnd,sec,pTitle,pMessage,NULL);
}

TIMEDMESSAGEBOX_API DWORD fnTimedMessageBox2(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR pMessage,
										  TIMEDMESSAGEBOX_PARAMS* pParams)
{
	DialogParams params;
	params.nSec = sec;
	params.pTitle = pTitle;
	params.pMessage = pMessage;
	params.timedout = false;
	params.pTimedParams=pParams;

	DWORD dret = DialogBoxParamW(
		g_hModule,
		MAKEINTRESOURCEW(IDD_DIALOG_MAIN),
		hWnd,
		DlgProc,
		(LPARAM)&params);

	pParams->nTimeout = !!params.timedout;
	return dret;
}


