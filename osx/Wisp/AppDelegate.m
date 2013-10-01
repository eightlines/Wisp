#import "AppDelegate.h"

@implementation AppDelegate

@synthesize ble;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    ble = [[BLE alloc] init];
    [ble controlSetup:1];
    ble.delegate = self;
}

-(void) bleDidConnect {
    NSLog(@"->Connected");
    
    btnConnect.title = @"Disconnect";
    [indConnect stopAnimation:self];
    
    sliderRed.enabled = true;
    sliderGreen.enabled = true;
    sliderBlue.enabled = true;
    sliderRate.enabled = true;
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
    sliderPosition.enabled = false;
    sliderLeadCorona.enabled = false;
    sliderTailCorona.enabled = false;
    lights.enabled = false;
    ringType.enabled = false;
    direction.enabled = false;
}

-(void) bleDidReceiveData:(unsigned char *)data length:(int)length {
    NSLog(@"Length: %d", length);
    
    // parse data, all commands are in 3-byte
    for (int i = 0; i < length; i+=3) {
        NSLog(@"0x%02X, 0x%02X, 0x%02X", data[i], data[i+1], data[i+2]);
        
        if (data[i] == 0x0A) {
//            if (data[i+1] == 0x01)
//                lblDigitalIn.stringValue = @"HIGH";
//            else
//                lblDigitalIn.stringValue = @"LOW";
        } else if (data[i] == 0x0B) {
            UInt16 Value;
            Value = data[i+2] | data[i+1] << 8;
//            lblAnalogIn.stringValue = [NSString stringWithFormat:@"%d", Value];
        }
    }
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
    //UInt8 buf[3] = {0x01, 0x00, 0x00};
    NSTabViewItem * item = [tabView selectedTabViewItem];
    NSLog(@"%i", [[item identifier] intValue]);
    
//    if (swDigitalOut.selectedSegment == 0)
//        buf[1] = 0x01;
//    else
//        buf[1] = 0x00;
    
//    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
//    [ble write:data];
}

-(IBAction)sendSlider:(id)sender { // RED
    NSLog(@"sendSlider:%i", (int)[sender tag]); // DEBUGGING

    UInt8 bId;
    int bValue;
    
    switch ([sender tag]) {
        case 0:
            bId = 0x01;
            bValue = (int)sliderRed.integerValue;
            break;
        case 1:
            bId = 0x02;
            bValue = (int)sliderGreen.integerValue;
            break;
        case 2:
            bId = 0x03;
            bValue = (int)sliderBlue.integerValue;
            break;
        case 3:
            bId = 0x05;
            bValue = (int)sliderRate.integerValue;
            break;
        case 4:
            bId = 0x08;
            bValue = (int)sliderPosition.integerValue;
            break;
        case 5:
            bId = 0x10;
            bValue = (int)sliderLeadCorona.integerValue;
            break;
        case 6:
            bId = 0x11;
            bValue = (int)sliderTailCorona.integerValue;
            break;
    }

    UInt8 buf[3] = {bId, bValue, bValue >> 8};
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendSegmentedControl:(id)sender {
    NSLog(@"sendSegmentedControl:%i", (int)[sender tag]); // DEBUGGING
    
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

    UInt8 buf[3] = {bId, bValue, bValue >> 8};
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

@end
