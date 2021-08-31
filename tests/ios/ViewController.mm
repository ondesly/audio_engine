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

    if (!s_player) {
        s_player = std::make_unique<maw::player>();
    }
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample" ofType:@"mp3"];
    std::string path = [nsPath UTF8String];
    
    s_player->load(path);
    s_player->play(path);
}

@end
