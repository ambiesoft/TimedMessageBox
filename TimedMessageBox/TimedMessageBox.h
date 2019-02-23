
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TIMEDMESSAGEBOX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TIMEDMESSAGEBOX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef TIMEDMESSAGEBOX_EXPORTS
#define TIMEDMESSAGEBOX_API __declspec(dllexport)
#else
#define TIMEDMESSAGEBOX_API __declspec(dllimport)
#endif

extern "C" {

enum TIMEDMESSAGEBOX_POSITION {
	TIMEDMESSAGEBOX_POSITION_DEFAULT,
	TIMEDMESSAGEBOX_POSITION_TOPLEFT,
	TIMEDMESSAGEBOX_POSITION_TOPRIGHT,
	TIMEDMESSAGEBOX_POSITION_BOTTOMLEFT,
	TIMEDMESSAGEBOX_POSITION_BOTTOMRIGHT,
	TIMEDMESSAGEBOX_POSITION_CENTERSCREEN,
	TIMEDMESSAGEBOX_POSITION_CENTERPARENT,
};

enum TIMEDMESSAGEBOX_RESULT {
	TIMEDMESSAGEBOX_RESULT_OK = 1 << 0,
	TIMEDMESSAGEBOX_RESULT_TIMEDOUT = 1 << 31,
};


enum TIMEDMESSAGEBOX_FLAGS
{
	TIMEDMESSAGEBOX_FLAGS_POSITION = 1 << 0,
	TIMEDMESSAGEBOX_FLAGS_LOGFONT = 1 << 1,
	TIMEDMESSAGEBOX_FLAGS_SHOWCMD = 1 << 2,
	TIMEDMESSAGEBOX_FLAGS_TOPMOST = 1 << 3,
};



struct TIMEDMESSAGEBOX_PARAMS
{
	int size;
	DWORD flags;
	HWND hWndCenterParent;
	TIMEDMESSAGEBOX_POSITION position;
	LOGFONT* pLogfont;
	int nShowCmd;
};

typedef int (*FNTimedMessageBox)(HWND hWnd,
								int sec,
								LPCWSTR pTitle,
								LPCWSTR Message); 
TIMEDMESSAGEBOX_API DWORD fnTimedMessageBox(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR Message);
										 

typedef DWORD(*FNTimedMessageBox2)(HWND hWnd,
								int sec,
								LPCWSTR pTitle,
								LPCWSTR Message,
								TIMEDMESSAGEBOX_PARAMS* pParams); 
TIMEDMESSAGEBOX_API DWORD fnTimedMessageBox2(HWND hWnd,
										  int sec,
										  LPCWSTR pTitle,
										  LPCWSTR Message,
										  TIMEDMESSAGEBOX_PARAMS* pParams);

} // extern "C"