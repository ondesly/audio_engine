//
//  ViewController.m
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include "maw/audio.h"

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample" ofType:@"mp3"];
    std::string path = [nsPath UTF8String];
    
    oo::audio audio{};
    audio.load(path);
    audio.play(path);
}

@end
