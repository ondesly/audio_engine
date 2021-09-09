//
//  ViewController.m
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <memory>

#include <maw/player.h>

#import "ViewController.h"

namespace {

    std::unique_ptr<maw::player> s_player;

}

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
}

- (IBAction)playButtonTouch:(id)sender {
    if (!s_player) {
        s_player = std::make_unique<maw::player>();
    }
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample4" ofType:@"mp3"];
    
    s_player->play([nsPath UTF8String]);
}

- (IBAction)stopButtonTouch:(id)sender {
    if (!s_player) {
        s_player = std::make_unique<maw::player>();
    }
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample4" ofType:@"mp3"];
    
    s_player->stop([nsPath UTF8String]);
}

- (IBAction)stopAllButtonTouch:(id)sender {
    if (!s_player) {
        s_player = std::make_unique<maw::player>();
    }
    
    s_player->stop();
}

- (IBAction)termButtonTouch:(id)sender {
    s_player = nullptr;
}

@end
