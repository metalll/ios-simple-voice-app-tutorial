//
//  LoginViewController.h
//  CallingApp
//
//  Created by christianjensen on 30/07/14.
//  Copyright (c) 2014 sinch. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CallScreenViewController.h"
@interface LoginViewController : UIViewController
@property (strong, nonatomic) IBOutlet UITextField *username;
- (IBAction)login:(id)sender;

@end
