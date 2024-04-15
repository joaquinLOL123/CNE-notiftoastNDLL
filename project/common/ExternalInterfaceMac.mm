#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif

#if defined(HX_MACOS)
#include "MacUtils.h"
#endif

#include <hx/CFFI.h>
#include <hx/CFFIPrime.h>

// Helpers
inline bool val_is_raw_object(value inVal) { return val_type(inVal)==valtObject; }
inline bool val_is_enum(value inVal) { return val_type(inVal)==valtEnum; }
inline bool val_is_class(value inVal) { return val_type(inVal)==valtClass; }

#if defined (HX_MACOS)
	static value notification_send (value title, value desc, value taskIconPath) {
		@autoreleasepool {
			MacUtils *macUtils = [[MacUtils alloc] init];
			[macUtils deliverWithTitle:val_string(title) description:val_string(desc) iconPath:val_string(taskIconPath)];
		}
		return alloc_int(1);
	}
	DEFINE_PRIME3 (notification_send);

	static value notification_delete () {
		return alloc_int(0);
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