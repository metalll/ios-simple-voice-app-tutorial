 
#Building a simple voice app in iOS

In this tutorial you will learn how to use the Sinch SDK to make a voice call.  

##Start

 If you don't have an account with Sinch, sign up for one at [www.sinch.com/signup](http://www.sinch.com/signup). Set up a new application using the [Dashboard](http://www.sinch.com/dashboard/#/apps "Sinch Dashboard"), and take note of your application key and secret. Next:

*   Launch Xcode and create a new project (File&gt;New&gt;Project)
*   Select 'Single View Application' and click next
*   Name the project 'CallingApp' and save it

The easiest way to add the Sinch SDK is to use CocoaPods. In your Xcode project directory, create a _Podfile_ with the content below.

<div class="highlight highlight-objectivec"><pre>platform :ios, '7.0'
pod 'SinchRTC'</pre></div>

Now, open a terminal window in your Xcode project directory and type `pod install`. Remember to open the workspace in Xcode when using CocoaPods.

_Note: If you are new to CocoaPods  go to [cocoapods.org](http://cocoapods.org/) to learn how to install it._

##Login

To enable app to app calling, it is necessary to start the _SINClient_ with a username. In this example, you will let the user type in any username they want with no password.

First, create a new ViewController (File&gt;New&gt;File)

*   Select objective-c class and click next
*   Name the file _LoginViewController_
    Make sure UIViewController is the subclass.

Now, open **Main.Storyboard** and add 

*   A label with Username
*   Empty textbox
*   Button with the label Login
        <div style="text-align:center;">
![](readme_images/loginscreen.png)
        </div>

Set the custom class of the view to **LoginViewController**. 

Next, open the Assistant Editor (View&gt;Assitant Editor&gt;Show Assistant Editor). Create an outlet for the textfield by pressing control and dragging into **LoginViewController.h** in the Assistant Editor. Name the outlet username.

Next, create an action from the button by repeating the step from above, but choose action as connection. Name the method login.

_Note: Sometimes the assistant chooses .m files instead of .h make sure you are in the right one_

Open **LoginViewController.m** add `@synthesize userName;` below the [@implementation](https://github.com/implementation) line.

## Login action

When the user clicks on the login button, you want to present the CallScreenView. Add a new view to your storyboard with the following:

*   A label with User to call
*   Empty textbox
*   Button with the label Call

    Add a modal segue from the files owner of LoginViewController to the above CallscreenView and name it showCallScreen. The storyboard should now look like this:

![](readme_images/allscreensID.png)

Open **LoginViewController.m** and add the login method for the button.

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-(</span><span class="kt">IBAction</span><span class="p">)</span><span class="nf">login:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span> <span class="p">{</span>
    <span class="k">if</span> <span class="p">(</span><span class="n">username</span><span class="p">.</span><span class="n">text</span> <span class="o">!=</span> <span class="nb">nil</span> <span class="p">)</span> <span class="p">{</span>
    <span class="p">[</span><span class="nb">self</span> <span class="nl">performSegueWithIdentifier</span><span class="p">:</span><span class="s">@"showCallScreen"</span> <span class="nl">sender</span><span class="p">:</span><span class="n">username</span><span class="p">.</span><span class="n">text</span><span class="p">];</span>
    <span class="p">}</span>
<span class="p">}</span>
</pre>
            </div>

    The above code will perform the Segue if the username is not empty. To pass the username to the call screen, you need to implement prepareForSegue. Add the following code to **LoginViewController.m**:

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-(</span><span class="kt">void</span><span class="p">)</span><span class="nf">prepareForSegue:</span><span class="p">(</span><span class="bp">UIStoryboardSegue</span><span class="o">*</span><span class="p">)</span><span class="nv">segue</span> <span class="nf">sender:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span>
<span class="p">{</span>
    <span class="c1">// check that its the right segue</span>
    <span class="k">if</span> <span class="p">([</span><span class="n">segue</span><span class="p">.</span><span class="n">identifier</span> <span class="nl">isEqualToString</span><span class="p">:</span><span class="s">@"showCallScreen"</span><span class="p">])</span>     <span class="p">{</span>
    <span class="c1">// TODO, get the view controller for call screen</span>
    <span class="p">}</span>
<span class="p">}</span>
</pre>
            </div>

Now, compile and run the app, and you should now see the callscreen after pressing login.

##Adding a CallScreenViewController

Create a new ViewController and call it **CallScreenViewController**. This controller will handle all call activity in the app. Set the custom class of the view Callscreen to **CallScreenViewController** in interfacebuilder. Then, open the **CallScreenViewController.h** and add the following property:

            <div class="highlight highlight-objectivec">
    <pre><span class="cp">#import &lt;UIKit/UIKit.h&gt;</span>
<span class="k">@interface</span> <span class="nc">CallScreenViewController</span> : <span class="bp">UIViewController</span>
<span class="k">@property</span> <span class="bp">NSString</span><span class="o">*</span> <span class="n">username</span><span class="p">;</span>
<span class="k">@end</span>
</pre>
            </div>

Also add `@synthesize username` to the **CallScreenViewController.m** file. Go back to LoginViewController.m and add `#import "CallScreenViewController.h"` to your imports. Change `-(void)prepareForSegue:(UIStoryboardSegue*)segue sender:(id)sender` to:

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-(</span><span class="kt">void</span><span class="p">)</span><span class="nf">prepareForSegue:</span><span class="p">(</span><span class="bp">UIStoryboardSegue</span><span class="o">*</span><span class="p">)</span><span class="nv">segue</span> <span class="nf">sender:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span> <span class="p">{</span>
    <span class="c1">// Check that it's the right segue</span>
    <span class="k">if</span> <span class="p">([</span><span class="n">segue</span><span class="p">.</span><span class="n">identifier</span> <span class="nl">isEqualToString</span><span class="p">:</span><span class="s">@"showCallScreen"</span><span class="p">])</span> 
    <span class="p">{</span>
    <span class="c1">// Get destination viewController</span>
    <span class="n">CallScreenViewController</span> <span class="o">*</span><span class="n">vc</span> <span class="o">=</span> <span class="p">[</span><span class="n">segue</span> <span class="n">destinationViewController</span><span class="p">];</span>
    <span class="c1">// Set the username property of CallScreenViewController</span>
    <span class="n">vc</span><span class="p">.</span><span class="n">username</span> <span class="o">=</span> <span class="n">sender</span><span class="p">;</span>
    <span class="p">}</span>
<span class="p">}</span>
<span class="k">@end</span>
</pre>
            </div>

##Call screen implementation

The CallScreenViewController will be responsible for handling all events from the Sinch client. To enable this, open **CallScreenViewController.h** and add the following import:

            <div class="highlight highlight-objectivec">
    <pre><span class="cp">#import &lt;Sinch/Sinch.h&gt;</span>
</pre>
            </div>

The CallScreenViewController must implement the SINCallClientDelegate and SINCallDelegate protocol. SINCallClientDelegate protocol handles events such as incoming call notification, and the  SINCallDelegate protocol handles in call events like when a call is connected or ended. 

            <div class="highlight highlight-objectivec">
    <pre><span class="k">@interface</span> <span class="nc">CallScreenViewController</span> : 
<span class="bp">UIViewController</span> <span class="o">&lt;</span><span class="n">SINCallClientDelegate</span><span class="p">,</span> <span class="n">SINCallDelegate</span><span class="o">&gt;</span>
</pre>
            </div>

##Prepare to make a call

To make and receive calls you will need an instance of the Sinch client. Open **CallScreenViewController.m** and add the following instance variable:

            <div class="highlight highlight-objectivec">
    <pre><span class="k">@interface</span> <span class="nc">CallScreenViewController</span> <span class="p">()</span>
<span class="p">{</span>
    <span class="kt">id</span><span class="o">&lt;</span><span class="n">SINClient</span><span class="o">&gt;</span> <span class="n">_client</span><span class="p">;</span>
<span class="p">}</span>
<span class="k">@end</span>
</pre>
            </div>

Now add a method to start the client, make sure insert your application key and application secret here. When you are starting the client, everything necessary to handle calls are set up.  

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">initSinchClient</span> <span class="p">{</span>
    <span class="n">_client</span> <span class="o">=</span> <span class="p">[</span><span class="n">Sinch</span> <span class="nl">clientWithApplicationKey</span><span class="p">:</span><span class="s">@"your_key"</span>
    <span class="nl">applicationSecret</span><span class="p">:</span><span class="s">@"your_secret"</span>
    <span class="nl">environmentHost</span><span class="p">:</span><span class="s">@"sandbox.sinch.com"</span>
    <span class="nl">userId</span><span class="p">:</span><span class="nb">self</span><span class="p">.</span><span class="n">username</span><span class="p">];</span>
    <span class="n">_client</span><span class="p">.</span><span class="n">callClient</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span>
    <span class="p">[</span><span class="n">_client</span> <span class="nl">setSupportCalling</span><span class="p">:</span><span class="nb">YES</span><span class="p">];</span>
    <span class="p">[</span><span class="n">_client</span> <span class="n">start</span><span class="p">];</span>
    <span class="p">[</span><span class="n">_client</span> <span class="n">startListeningOnActiveConnection</span><span class="p">];</span>
<span class="p">}</span>
</pre>
            </div>

Replace the code in `viewDidLoad` with the following

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">viewDidLoad</span>
<span class="p">{</span>
    <span class="p">[</span><span class="nb">super</span> <span class="n">viewDidLoad</span><span class="p">];</span>
    <span class="p">[</span><span class="nb">self</span> <span class="n">initSinchClient</span><span class="p">];</span>
<span class="p">}</span>
</pre>
            </div>

##Make a call

Follow these steps to connect the user interface to enable the user to call:

*   Open **Main.storyboard** and select the call screen
*   Create an outlet for the textview call it remoteUsername
*   Create an action for the button that you call `callUser:`
        Don't forget to synthezise them in  your .m file.
        Your **CallScreenViewController.h** should now look like this:<div class="highlight highlight-objectivec">
    <pre><span class="cp">#import &lt;UIKit/UIKit.h&gt;</span>
<span class="cp">#import &lt;Sinch/Sinch.h&gt;</span>
<span class="k">@interface</span> <span class="nc">CallScreenViewController</span> : 
    <span class="bp">UIViewController</span> <span class="o">&lt;</span><span class="n">SINCallDelegate</span><span class="p">,</span> <span class="n">SINCallClientDelegate</span><span class="o">&gt;</span>
<span class="k">@property</span> <span class="bp">NSString</span><span class="o">*</span> <span class="n">username</span><span class="p">;</span>
<span class="k">@property</span> <span class="p">(</span><span class="k">strong</span><span class="p">,</span> <span class="k">nonatomic</span><span class="p">)</span> <span class="kt">IBOutlet</span> <span class="bp">UITextField</span> <span class="o">*</span><span class="n">remoteUsername</span><span class="p">;</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">IBAction</span><span class="p">)</span><span class="nf">callUser:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span><span class="p">;</span>
<span class="k">@end</span>
</pre>
            </div>

Open **CallScreenViewController.m** add an instance variable for a call:

            <div class="highlight highlight-objectivec">
    <pre><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span> <span class="n">_call</span><span class="p">;</span>
</pre>
            </div>

When user clicks call, set up a voice conversation in callUser:

            <div class="highlight highlight-objectivec">
<pre><span class="p">-</span><span class="p">(</span><span class="kt">IBAction</span><span class="p">)</span><span class="nf">callUser:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span> <span class="p">{</span>
    <span class="n">_call</span> <span class="o">=</span> <span class="p">[</span><span class="n">_client</span><span class="p">.</span><span class="n">callClient</span> <span class="nl">callUserWithId</span><span class="p">:</span><span class="nb">self</span><span class="p">.</span><span class="n">remoteUsername</span><span class="p">.</span><span class="n">text</span><span class="p">];</span> <span class="c1">// Create the call</span>
    <span class="n">_call</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span> <span class="c1">// Listen for events on self</span>
<span class="p">}</span>
</pre>
            </div>

##Answering the call

To answer a call you need to add code in didReceiveIncomingCall. In this example you will accept all calls, but in a normal application you would display a UI where the user could accept or reject a call. 

            <div class="highlight highlight-objectivec">
<pre><span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">client:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCallClient</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">client</span> <span class="nf">didReceiveIncomingCall:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// For now we are just going to answer calls, </span>
    <span class="c1">// in a normal app you would show in incoming call screen</span>
    <span class="n">call</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span>
    <span class="n">_call</span> <span class="o">=</span> <span class="n">call</span><span class="p">;</span>
    <span class="p">[</span><span class="n">_call</span> <span class="n">answer</span><span class="p">];</span>
<span class="p">}</span>
</pre>
            </div>

Next, you will implement the protocol for the call delegate. The different states of a phone call are: inprogress (trying to connect), established, and ending. These events are quite useful as a developer, because this is where you can record start times, make changes UI to hangup calls, etc.

            <div class="highlight highlight-objectivec">
    <pre><span class="cp">#pragma mark - SINCallDelegate</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">callDidProgress:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// In this method you can play ringing tone and update ui to display progress of call.</span>
<span class="p">}</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">callDidEstablish:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// Called when a call connects.</span>
<span class="p">}</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">callDidEnd:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// Called when call finnished.</span>
<span class="p">}</span>
</pre>
            </div>

##Run the app

*   Compile and run on the device emulator
*   Log in as **A**
*   Compile and run on your iPhone*   Log in as **B**
*   On the iphone, type **A** in the 'to user' box,  and press 'call'.

(For testing, I usually turn on some music on the computer and step outside the room to and listen in the iPhone.)

##Hanging up the call

It's pretty nice to be able to have a way to end the call. Open up **Main.storyboard** and create an outlet called 'callButton' from the call button.

You will want to change the text of the call button when a call changes state. Open **CallScreenViewController.m** add  callButton to your @synthezise and change the delegate methods:

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">callDidEstablish:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// Change to hangup when the call is connected</span>
    <span class="p">[</span><span class="nb">self</span><span class="p">.</span><span class="n">callButton</span> <span class="nl">setTitle</span><span class="p">:</span><span class="s">@"Hang up"</span> <span class="nl">forState</span><span class="p">:</span><span class="n">UIControlStateNormal</span><span class="p">];</span>
<span class="p">}</span>
<span class="p">-</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">callDidEnd:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINCall</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">call</span> <span class="p">{</span>
    <span class="c1">// Change to call again</span>
    <span class="p">[</span><span class="nb">self</span><span class="p">.</span><span class="n">callButton</span> <span class="nl">setTitle</span><span class="p">:</span><span class="s">@"Call"</span> <span class="nl">forState</span><span class="p">:</span><span class="n">UIControlStateNormal</span><span class="p">];</span>
    <span class="n">_call</span> <span class="o">=</span> <span class="nb">nil</span><span class="p">;</span>
<span class="p">}</span>
</pre>
            </div>

You also need to change the callUser function to hang up a call instead of dialing if a call is in progress:

            <div class="highlight highlight-objectivec">
<pre><span class="p">-</span> <span class="p">(</span><span class="kt">IBAction</span><span class="p">)</span><span class="nf">callUser:</span><span class="p">(</span><span class="kt">id</span><span class="p">)</span><span class="nv">sender</span> <span class="p">{</span>
    <span class="k">if</span> <span class="p">(</span><span class="n">_call</span> <span class="o">==</span> <span class="nb">nil</span><span class="p">)</span><span class="p">{</span>
        <span class="n">_call</span> <span class="o">=</span> <span class="p">[</span><span class="n">_client</span><span class="p">.</span><span class="n">callClient</span> <span class="nl">callUserWithId</span><span class="p">:</span><span class="nb">self</span><span class="p">.</span><span class="n">remoteUserId</span><span class="p">.</span><span class="n">text</span><span class="p">];</span>
      <span class="n">_call</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span>
    <span class="p">}</span>
    <span class="k">else</span><span class="p">{</span>
      <span class="p">[</span><span class="n">_call</span> <span class="n">hangup</span><span class="p">];</span>
      <span class="n">_call</span> <span class="o">=</span> <span class="nb">nil</span><span class="p">;</span>
    <span class="p">}</span>
<span class="p">}</span>
</pre>
            </div>

## Problems

If you are running into problems, there are a couple of easy things you can check:

*   Make sure application key and secret are correct*   Do you have an provisioning profile set?
*   If it still doesn't work, don't hesitate to email me at [christian@sinch.com](mailto:christian@sinch.com) to ask questions.

## Add logging

To help you determine what is wrong, you can add logging on the _SINClient_. In  **CallScreenViewController.h** add the following protocol SINClientDelegate:

            <div class="highlight highlight-objectivec">
    <pre><span class="k">@interface</span> <span class="nc">CallScreenViewController</span> : <span class="bp">UIViewController</span> <span class="o">&lt;</span><span class="n">SINClientDelegate</span><span class="p">,</span> <span class="n">SINCallClientDelegate</span><span class="p">,</span> <span class="n">SINCallDelegate</span><span class="o">&gt;</span>
</pre>
            </div>

Also add the follwing methods to **CallScreenViewController.h**:

            <div class="highlight highlight-objectivec">
    <pre><span class="cp">#pragma mark - SINClientDelegate</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">clientDidStart:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINClient</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">client</span> <span class="p">{</span>
    <span class="n">NSLog</span><span class="p">(</span><span class="s">@"Sinch client started (version: %@)"</span><span class="p">,</span> <span class="p">[</span><span class="n">Sinch</span> <span class="n">version</span><span class="p">]);</span>
<span class="p">}</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">clientDidStop:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINClient</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">client</span> <span class="p">{</span>
    <span class="n">NSLog</span><span class="p">(</span><span class="s">@"Sinch client stopped"</span><span class="p">);</span>
<span class="p">}</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">clientDidFail:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINClient</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">client</span> <span class="nf">error:</span><span class="p">(</span><span class="bp">NSError</span><span class="o">*</span><span class="p">)</span><span class="nv">error</span> <span class="p">{</span>
    <span class="n">NSLog</span><span class="p">(</span><span class="s">@"Error: %@"</span><span class="p">,</span> <span class="n">error</span><span class="p">.</span><span class="n">localizedDescription</span><span class="p">);</span>
<span class="p">}</span>
<span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">client:</span><span class="p">(</span><span class="kt">id</span><span class="o">&lt;</span><span class="n">SINClient</span><span class="o">&gt;</span><span class="p">)</span><span class="nv">client</span>
    <span class="nf">logMessage:</span><span class="p">(</span><span class="bp">NSString</span> <span class="o">*</span><span class="p">)</span><span class="nv">message</span>
    <span class="nf">area:</span><span class="p">(</span><span class="bp">NSString</span> <span class="o">*</span><span class="p">)</span><span class="nv">area</span>
    <span class="nf">severity:</span><span class="p">(</span><span class="n">SINLogSeverity</span><span class="p">)</span><span class="nv">severity</span>
    <span class="nf">timestamp:</span><span class="p">(</span><span class="bp">NSDate</span> <span class="o">*</span><span class="p">)</span><span class="nv">timestamp</span> <span class="p">{</span>
    <span class="c1">// If you want all messages remove the if statement</span>
    <span class="k">if</span> <span class="p">(</span><span class="n">severity</span> <span class="o">==</span> <span class="n">SINLogSeverityCritical</span><span class="p">)</span> <span class="p">{</span>
    <span class="n">NSLog</span><span class="p">(</span><span class="s">@"%@"</span><span class="p">,</span> <span class="n">message</span><span class="p">);</span>
    <span class="p">}</span>
<span class="p">}</span>
</pre>
            </div>

Set the delegate of the SINClient in `-(void)initSinchClientWithUserId:` as follows:

            <div class="highlight highlight-objectivec">
    <pre><span class="p">-</span> <span class="p">(</span><span class="kt">void</span><span class="p">)</span><span class="nf">initSinchClientWithUserId:</span><span class="p">(</span><span class="bp">NSString</span><span class="o">*</span><span class="p">)</span><span class="nv">userId</span> <span class="p">{</span>
    <span class="n">_client</span> <span class="o">=</span> <span class="p">[</span><span class="n">Sinch</span> <span class="nl">clientWithApplicationKey</span><span class="p">:</span><span class="s">@"your key"</span>
    <span class="nl">applicationSecret</span><span class="p">:</span><span class="s">@"your secret"</span>
    <span class="nl">environmentHost</span><span class="p">:</span><span class="s">@"sandbox.sinch.com"</span>
    <span class="nl">userId</span><span class="p">:</span><span class="nb">self</span><span class="p">.</span><span class="n">userName</span><span class="p">];</span>
    <span class="c1">// Add logging by setting delegate to self</span>
    <span class="n">_client</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span>
    <span class="p">[</span><span class="n">_client</span> <span class="nl">setSupportCalling</span><span class="p">:</span><span class="nb">YES</span><span class="p">];</span>
    <span class="n">_client</span><span class="p">.</span><span class="n">callClient</span><span class="p">.</span><span class="n">delegate</span> <span class="o">=</span> <span class="nb">self</span><span class="p">;</span>
    <span class="p">[</span><span class="n">_client</span> <span class="n">start</span><span class="p">];</span>
    <span class="p">[</span><span class="n">_client</span> <span class="n">startListeningOnActiveConnection</span><span class="p">];</span>
<span class="p">}</span>
</pre>
            </div>
        </div>
