//
//  ViewController.m
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <memory>

#include <audio/engine.h>

#import "ViewController.h"

namespace {

    std::unique_ptr<oo::audio::engine> s_engine;

}

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
}

- (IBAction)playButtonTouch:(id)sender {
    if (!s_engine) {
        s_engine = std::make_unique<oo::audio::engine>();
    }
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample4" ofType:@"mp3"];
    
    s_engine->play([nsPath UTF8String]);
}

- (IBAction)stopButtonTouch:(id)sender {
    if (!s_engine) {
        s_engine = std::make_unique<oo::audio::engine>();
    }
    
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* nsPath = [bundle pathForResource:@"sample4" ofType:@"mp3"];
    
    s_engine->stop([nsPath UTF8String]);
}

- (IBAction)stopAllButtonTouch:(id)sender {
    if (!s_engine) {
        s_engine = std::make_unique<oo::audio::engine>();
    }
    
    s_engine->stop();
}

- (IBAction)termButtonTouch:(id)sender {
    s_engine = nullptr;
}

@end
