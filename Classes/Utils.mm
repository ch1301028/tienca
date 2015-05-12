//
//  Utils.m
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 5/3/15.
//
//

#import <Foundation/Foundation.h>
#include "Utils.h"

#include "AppController.h"

string Utils::getUniqueId(){
    NSString *udid = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    std::string temp([udid UTF8String]);
    return temp;
}

void Utils::openSms(int amount){
    std::string udid = Utils::getUniqueId();
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate openSms : amount: udid];
    
}