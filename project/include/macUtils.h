//#include <AppKit/AppKit.h>
//#include <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

//@interface macUtils : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>
//@end


@interface MacUtils : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>

- (instancetype)init;

- (void)deliverWithTitle:(const char*)_title description:(const char*)_desc iconPath:(const char*)_iconPath;

@end