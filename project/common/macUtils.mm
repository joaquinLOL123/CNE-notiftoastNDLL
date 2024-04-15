#import "MacUtils.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSWindow.h>

@implementation MacUtils

- (instancetype)init {
    self = [super init];
    if (self) {
        // Initialization code, if any
    }
    return self;
}

- (void)deliverWithTitle:(const char*)_title description:(const char*)_desc iconPath:(const char*)_iconPath {
	NSString *title = [NSString stringWithCString:_title encoding:NSUTF8StringEncoding];
	NSString *desc = [NSString stringWithCString:_desc encoding:NSUTF8StringEncoding];
	NSString *iconPath = [NSString stringWithCString:_iconPath encoding:NSUTF8StringEncoding];

	NSUserNotification *userNotification = [NSUserNotification new];
	userNotification.title = title;
	userNotification.informativeText = desc;

	if (iconPath != nil) {
		NSImage *icon = [[NSImage alloc] initWithContentsOfFile:iconPath];
		userNotification.contentImage = icon;
	}

	userNotification.soundName = NSUserNotificationDefaultSoundName;

	NSUserNotificationCenter *center = [NSUserNotificationCenter defaultUserNotificationCenter];
	center.delegate = self;
	[center deliverNotification:userNotification];
	//[center scheduleNotification:userNotification];

	//NSLog(@"notification: %@, notification center:%@", userNotification, center);

	/*func userNotificationCenter(_ center: UNUserNotificationCenter, willPresent notification: UNNotification,
                                withCompletionHandler completionHandler: @escaping (UNNotificationPresentationOptions) -> Void) {
        // Forground notifications.
        completionHandler([.alert, .sound])
    }*/
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
    return YES;
}
@end