#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif

#if defined(HX_WINDOWS)
#include <windows.h>
#include <winuser.h>
#include <dwmapi.h>
#include <strsafe.h>
#include <shellapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Dwmapi")
#pragma comment(lib, "Shell32.lib")
using namespace std;
#endif


#include <hx/CFFI.h>
#include <hx/CFFIPrime.h>

// Helpers
inline bool val_is_raw_object(value inVal) { return val_type(inVal)==valtObject; }
inline bool val_is_enum(value inVal) { return val_type(inVal)==valtEnum; }
inline bool val_is_class(value inVal) { return val_type(inVal)==valtClass; }

#if defined(HX_WINDOWS)
	NOTIFYICONDATA m_NID;

	static value notification_send (value title, value desc, value taskIconPath) {
		HWND hWnd = GetActiveWindow();

		m_NID.cbSize = sizeof(m_NID);
		m_NID.hWnd = hWnd;
		m_NID.uFlags = NIF_ICON;

		LPCSTR lIcon = val_string(taskIconPath);

		m_NID.hIcon = (HICON)LoadImage(NULL, lIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		Shell_NotifyIcon(NIM_ADD, &m_NID);

		m_NID.uVersion = NOTIFYICON_VERSION_4;
		Shell_NotifyIcon(NIM_SETVERSION, &m_NID);

		m_NID.uFlags = NIF_INFO;
		m_NID.dwInfoFlags = NIIF_INFO | NIIF_LARGE_ICON;
		m_NID.uTimeout = 1000;

		LPCSTR lTitle = val_string(title);
		LPCSTR lDesc = val_string(desc);

		if (StringCchCopy(m_NID.szInfoTitle, sizeof(m_NID.szInfoTitle), lTitle) != S_OK)
			return FALSE;

		if (StringCchCopy(m_NID.szInfo, sizeof(m_NID.szInfo), lDesc) != S_OK)
			return FALSE;

		return alloc_int(Shell_NotifyIcon(NIM_MODIFY, &m_NID));
	}
	DEFINE_PRIME3 (notification_send);

	//literally just modified code from VS Dave and Bambi that adds icons and ofcourse making it work as an NDLL
	//https://github.com/BemboLikePizza/VsDave/blob/main/source/PlatformUtil.hx
	//(I am linking a fork of the mod's repo due to the actual mod repo being down)

	static value notification_delete () {
		return alloc_int(Shell_NotifyIcon(NIM_DELETE, &m_NID));
	}
	DEFINE_PRIME0 (notification_delete);
#else
	static value notification_send (value title, value desc, value taskIconPath) {
		return alloc_int(0);
	}
	DEFINE_PRIME3 (notification_send);

	static value notification_delete () {
		return alloc_int(0);
	}
	DEFINE_PRIME0 (notification_delete);
#endif


extern "C" void notification_main () {

	val_int(0); // Fix Neko init

}
DEFINE_ENTRY_POINT (notification_main);



extern "C" int notification_register_prims () { return 0; }