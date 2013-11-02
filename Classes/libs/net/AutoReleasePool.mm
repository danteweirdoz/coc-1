//
//  AutoReleasePool.m
//  Coc
//
//  Created by Hector on 13-10-27.
//
//
#import <UIKit/UIKit.h>

#include "AutoReleasePool.h"



AutoReleasePool::AutoReleasePool(){
	pool = [[NSAutoreleasePool alloc] init];
    
}

AutoReleasePool::~AutoReleasePool(){
    //	[pool drain];
	[(NSAutoreleasePool*)pool release];
}