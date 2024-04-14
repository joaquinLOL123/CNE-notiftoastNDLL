#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "macUtils.h"

@implementation macUtils

namespace macUtils {
	/*
	int testNSLog(value str) {
                
        //I know, the example is super silly, but easy enough to
        //demonstrate the idea
        NSString *msg = [[NSString alloc] initWithCString:"Testing..."];  
		NSLog(msg);

		return 0;
	} 
	*/
	//https://gist.github.com/preslavrachev/3673663

	int deliver:(NSString title, NSString desc, NSString iconPath) {
		NSUserNotification *userNotification = [NSUserNotification new];
  		userNotification.title = title;
  		userNotification.informativeText = desc;

		if (iconPath != nil) {
        	NSImage *icon = [[NSImage alloc] initWithContentsOfFile:iconPath];
        	userNotification.contentImage = icon;
    	}

		NSUserNotificationCenter *center = [NSUserNotificationCenter defaultUserNotificationCenter];
  		center.delegate = self;
  		[center scheduleNotification:userNotification];

		return 0;
	} 
}