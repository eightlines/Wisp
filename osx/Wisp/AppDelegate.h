#import <Cocoa/Cocoa.h>
#import "BLE.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, BLEDelegate>
{
    IBOutlet NSTextField *lblRSSI;
    IBOutlet NSSlider *sliderRed;
    IBOutlet NSSlider *sliderGreen;
    IBOutlet NSSlider *sliderBlue;
    IBOutlet NSSlider *sliderRate;
    IBOutlet NSSlider *sliderRate2;
    IBOutlet NSSlider *sliderPosition;
    IBOutlet NSSlider *sliderLeadCorona;
    IBOutlet NSSlider *sliderTailCorona;
    IBOutlet NSButton *btnConnect;
    IBOutlet NSProgressIndicator *indConnect;
    IBOutlet NSSegmentedControl *lights;
    IBOutlet NSSegmentedControl *ringType;
    IBOutlet NSSegmentedControl *direction;
    IBOutlet NSTabView *tabView;
    IBOutlet NSTabViewItem *itemBlink;
    IBOutlet NSTabViewItem *itemParticle;
}

@property (assign) IBOutlet NSWindow *window;
@property (strong, nonatomic) BLE *ble;

@end
