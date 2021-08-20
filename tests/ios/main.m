//
//  main.m
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char *argv[]) {
    NSString *appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}