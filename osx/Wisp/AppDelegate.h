#import <Cocoa/Cocoa.h>
#import "BLE.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, BLEDelegate>
{
    IBOutlet NSTextField *lblRSSI;
    IBOutlet NSSlider *sliderRed;
    IBOutlet NSSlider *sliderGreen;
    IBOutlet NSSlider *sliderBlue;
    IBOutlet NSSlider *sliderRate;
    IBOutlet NSButton *btnConnect;
    IBOutlet NSProgressIndicator *indConnect;
    IBOutlet NSSegmentedControl *lights;
    IBOutlet NSSegmentedControl *ringType;
    IBOutlet NSTabView *tabView;
    IBOutlet NSTabViewItem *itemBlink;
    IBOutlet NSTabViewItem *itemParticle;
    
//    IBOutlet NSTextField *lblAnalogIn;
//    IBOutlet NSSegmentedControl *swDigitalOut;
//    IBOutlet NSTextField *lblDigitalIn;
//    IBOutlet NSButton *btnAnalogIn;
}

@property (assign) IBOutlet NSWindow *window;
@property (strong, nonatomic) BLE *ble;

@end
