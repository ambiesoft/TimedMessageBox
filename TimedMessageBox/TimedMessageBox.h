
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

typedef int (*FNTimedMessageBox)(int sec,
										  LPCWSTR pTitle,
										  LPCWSTR Message); 
TIMEDMESSAGEBOX_API int fnTimedMessageBox(int sec,
										  LPCWSTR pTitle,
										  LPCWSTR Message);

} // extern "C"