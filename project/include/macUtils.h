#import <Cocoa/Cocoa.h>

@interface macUtils : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>
@end

namespace macUtils {

	/*
	int testNSLog();
	*/
	//https://gist.github.com/preslavrachev/3673663

	void deliver(NSString title, NSString desc, NSString iconPath);
}
