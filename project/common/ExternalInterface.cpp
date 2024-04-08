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
#include "Utils.h"


using namespace utils;

// Helpers
inline bool val_is_raw_object(value inVal) { return val_type(inVal)==valtObject; }
inline bool val_is_enum(value inVal) { return val_type(inVal)==valtEnum; }
inline bool val_is_class(value inVal) { return val_type(inVal)==valtClass; }

//vars
NOTIFYICONDATA m_NID;

// Allocating:
// alloc_null() == generate haxe null / or use val_null
// alloc_bool(bool) == generate haxe bool
// alloc_int(int) == generate haxe int
// alloc_float(double) == generate haxe float
// alloc_empty_object() == generate haxe anon obj
// alloc_string(const char *) == generate haxe string
// alloc_array(int) == generate haxe array with a size

// Getting:
// val_int(value) == get int from haxe object
// val_bool(value) == get bool from haxe object
// val_float(value) == get double from haxe object
// val_string(value) == get const char * from haxe object

// Checking
// val_is_null(value) == checks if the value is null
// val_is_int(value) == checks if the value is int
// val_is_bool(value) == checks if the value is bool
// val_is_float(value) == checks if the value is float
// val_is_string(value) == checks if the value is string
// val_is_function(value) == checks if the value is function
// val_is_array(value) == checks if the value is array
// val_is_abstract(value) == checks if the value is abstract? unsure
// val_is_kind(value, vkind) == checks if the value is kind? unsure
// val_is_number(value) == checks if the value is int or float
// val_is_object(value) == checks if the value is int or float
// val_is_raw_object(value) == checks if the value is an anon obj (Custom)
// val_is_enum(value) == checks if the value is enum (Custom)
// val_is_class(value) == checks if the value is class (Custom)

// Arrays:
// val_array_size(value) == get length of array
// val_array_i(value, int) == get item from index from array, you need to use the val_ functions to prepare the value
// val_array_set_i(value, int, value) == set item at index in array, you need to use alloc_ function to prepare the value
// val_array_set_size(value, int) == sets the length of the array
// val_array_push(value, value) == pushes a value to an array, you need to use the val_ functions to prepare the value
// alloc_array(int) == idk what the int does

// Functions:
// you need to use the val_ functions to prepare the values
// add o before the call function (val_ocall0) to call a object field, and add an int value for the field, unsure what this is for
// val_call0(value) == call haxe function with 0 args
// val_call1(value, value) == call haxe function with 1 args
// val_call2(value, value, value) == call haxe function with 2 args
// val_call3(value, value, value, value) == call haxe function with 3 args
// val_callN(value, value*, int) == calls haxe function with n args, based on the int and the array, Usage below

/*
int num_args = 4;
value *args = new value[num_args];
args[0] = alloc_bool(false);
args[1] = alloc_bool(true);
args[2] = alloc_int(5);
args[3] = alloc_int(15);
value result = val_callN(root->get(), args, num_args);
*/

// Objects
// idk it uses an int for the field i have no clue
// tho theres val_field_name(field) no clue how this is used
// setting a field: alloc_field(value, val_id(val_string(value)), value);
// setting a field with cstring: alloc_field(value, val_id(const char*), value);
// getting a field: val_field(value, val_id(val_string(value)));
// getting a field with cstring: val_field(value, val_id(const char*));
// TODO: finish this

// Misc:
// val_gc(value, c++ func) == sets a finalizer for a object, warning very limited, trying to allocate haxe memory crashes
// val_fun_nargs(value) == get total args of function, -1 for Reflect.makeVarArgs
// val_strlen(value) == get length of string

// Check https://github.com/HaxeFoundation/hxcpp/blob/master/src/hx/CFFI.cpp for the code, and more functions

// Defining functions for the ndll:
// the prefix isnt needed it can completely just be set_window_transparent if you want
// n can be from 0 to 15
// DEFINE_PRIMEn(function) == define function that returns
// DEFINE_PRIMEnv(function) == define void function

// FYI: if the type is bool or int or double (haxe float) then it does the conversion automatically

#if defined(HX_WINDOWS)
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
	return val_int(0);
}
DEFINE_PRIME3 (notification_send);

static value notification_delete () {
	return val_int(0);
}
DEFINE_PRIME0 (notification_delete);

#endif

// Examples
/*
static value ndllexample_sample_method (value inputValue) {
	int returnValue = val_int(inputValue);
	return alloc_int(returnValue);
}
DEFINE_PRIME1 (ndllexample_sample_method);

static void ndllexample_sample_method_void (value inputValue) {
	int returnValue = val_int(inputValue);
}
DEFINE_PRIME1v (ndllexample_sample_method_void);
*/



extern "C" void notification_main () {
	
	val_int(0); // Fix Neko init
	
}
DEFINE_ENTRY_POINT (notification_main);



extern "C" int notification_register_prims () { return 0; }