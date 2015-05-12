//
//  AdmobHelper.m
//  Tiên Cá
//
//  Created by Ho Duy Nhat Linh on 5/3/15.
//
//

#import <Foundation/Foundation.h>
#include "AdmobHelper.h"
#include "AppController.h"

void AdmobHelper::hideAd(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate hideAdmobBanner];
}

void AdmobHelper::showAd(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showAdmobBanner];
}

void AdmobHelper::showInterstitial(){
        
}
void AdmobHelper::rateApp(){
    
}
