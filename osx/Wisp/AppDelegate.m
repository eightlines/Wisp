#import "AppDelegate.h"

@implementation AppDelegate

@synthesize ble;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    ble = [[BLE alloc] init];
    [ble controlSetup:1];
    ble.delegate = self;
    [tabView selectFirstTabViewItem:nil];
}

-(void) bleDidConnect {
    NSLog(@"->Connected");
    
    btnConnect.title = @"Disconnect";
    [indConnect stopAnimation:self];
    
    sliderRed.enabled = true;
    sliderGreen.enabled = true;
    sliderBlue.enabled = true;
    sliderRate.enabled = true;
    sliderRate2.enabled = true;
    sliderPosition.enabled = true;
    sliderLeadCorona.enabled = true;
    sliderTailCorona.enabled = true;
    lights.enabled = true;
    ringType.enabled = true;
    direction.enabled = true;
    
    sliderRed.integerValue = 0;
    sliderGreen.integerValue = 0;
    sliderBlue.integerValue = 0;
    sliderRate.integerValue = 32;
    sliderRate2.integerValue = 32;
    sliderPosition.integerValue = 0;
    sliderLeadCorona.integerValue = 0;
    sliderTailCorona.integerValue = 0;
}

- (void)bleDidDisconnect {
    NSLog(@"->Disconnected");
    
    btnConnect.title = @"Connect";
    lblRSSI.stringValue = @"---";
    
    sliderRed.enabled = false;
    sliderGreen.enabled = false;
    sliderBlue.enabled = false;
    sliderRate.enabled = false;
    sliderRate2.enabled = false;
    sliderPosition.enabled = false;
    sliderLeadCorona.enabled = false;
    sliderTailCorona.enabled = false;
    lights.enabled = false;
    ringType.enabled = false;
    direction.enabled = false;
}

-(void) bleDidReceiveData:(unsigned char *)data length:(int)length {
//    NSLog(@"Length: %d", length);
    
    // parse data, all commands are in 3-byte
//    for (int i = 0; i < length; i+=3) {
//        NSLog(@"0x%02X, 0x%02X, 0x%02X", data[i], data[i+1], data[i+2]);
//    }
}

-(void) bleDidUpdateRSSI:(NSNumber *) rssi {
    lblRSSI.stringValue = rssi.stringValue;
}

- (IBAction)btnConnect:(id)sender {
    if (ble.activePeripheral)
        if(ble.activePeripheral.isConnected) {
            [[ble CM] cancelPeripheralConnection:[ble activePeripheral]];
            return;
        }
    
    if (ble.peripherals) ble.peripherals = nil;
    
    [ble findBLEPeripherals:2];
    [NSTimer scheduledTimerWithTimeInterval:(float)2.0 target:self selector:@selector(connectionTimer:) userInfo:nil repeats:NO];
    [indConnect startAnimation:self];
}

-(void) connectionTimer:(NSTimer *)timer {
    if (ble.peripherals.count > 0) {
        [ble connectPeripheral:[ble.peripherals objectAtIndex:0]];
    } else {
        [indConnect stopAnimation:self];
    }
}

-(IBAction)sendMode:(id)sender {
    NSTabViewItem * item = [tabView selectedTabViewItem];
    NSLog(@"%i", [[item identifier] intValue]);
}

-(IBAction)sendSlider:(id)sender { // RED
    UInt8 bId;
    int bValue;
    
    switch ([sender tag]) {
        case 0: // R
            bId = 0x02;
            bValue = (int)sliderRed.integerValue;
            break;
        case 1: // G
            bId = 0x03;
            bValue = (int)sliderGreen.integerValue;
            break;
        case 2: // B
            bId = 0x04;
            bValue = (int)sliderBlue.integerValue;
            break;
        case 3: // RATE
            bId = 0x05;
            bValue = (int)sliderRate.integerValue;
            break;
        case 4: // POSITION
            bId = 0x08;
            bValue = (int)sliderPosition.integerValue;
            break;
        case 5: // LEAD CORONA
            bId = 0x10;
            bValue = (int)sliderLeadCorona.integerValue;
            break;
        case 6: // TAIL CORONA
            bId = 0x11;
            bValue = (int)sliderTailCorona.integerValue;
            break;
        case 7: // RATE2
            bId = 0x05;
            bValue = (int)sliderRate2.integerValue;
            break;
    }

    NSLog(@"sendSlider:%i %i", (int)[sender tag], bValue); // DEBUGGING
    UInt8 buf[3] = {bId, bValue, bValue >> 8};
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendSegmentedControl:(id)sender {
    UInt8 bId;
    int bValue;
    
    switch ([sender tag]) {
        case 0:
            bId = 0x06;
            bValue = (int)lights.selectedSegment;
            break;
        case 1:
            bId = 0x07;
            bValue = (int)ringType.selectedSegment;
            break;
        case 2:
            bId = 0x09;
            bValue = (int)direction.selectedSegment;
            break;
    }

    NSLog(@"sendSegmentedControl:%i %i", (int)[sender tag], bValue); // DEBUGGING
    UInt8 buf[3] = {bId, bValue, bValue >> 8};
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

@end
