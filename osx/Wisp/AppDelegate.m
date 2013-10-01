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

-(IBAction)sendRed:(id)sender { // RED
    //NSLog([sender gelabel]);
    UInt8 buf[3] = {0x02, 0x00, 0x00};
    
    buf[1] = sliderRed.integerValue;
    buf[2] = (int)sliderRed.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendGreen:(id)sender { // GREEN
    UInt8 buf[3] = {0x03, 0x00, 0x00};
    
    buf[1] = sliderGreen.integerValue;
    buf[2] = (int)sliderGreen.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendBlue:(id)sender { // BLUE
    UInt8 buf[3] = {0x04, 0x00, 0x00};
    
    buf[1] = sliderBlue.integerValue;
    buf[2] = (int)sliderBlue.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendRate:(id)sender { // RATE
    UInt8 buf[3] = {0x05, 0x00, 0x00};
    
    buf[1] = sliderRate.integerValue;
    buf[2] = (int)sliderRate.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendLightState:(id)sender { // LIGHTS ON/OFF
    NSInteger selIndex = lights.selectedSegment;
    
    UInt8 buf[3] = {0x06, 0x00, 0x00};
    
    buf[1] = selIndex;
    buf[2] = (int)selIndex >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendRingState:(id)sender { // RING FADE/SNAP
    NSInteger selIndex = ringType.selectedSegment;
    
    UInt8 buf[3] = {0x07, 0x00, 0x00};
    
    buf[1] = selIndex;
    buf[2] = (int)selIndex >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendPosition:(id)sender { // PARTICLE START POSITION
    UInt8 buf[3] = {0x08, 0x00, 0x00};
    
    buf[1] = sliderPosition.integerValue;
    buf[2] = (int)sliderPosition.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendDirection:(id)sender { // PARTICLE DIRECTION
    NSInteger selIndex = direction.selectedSegment;
    
    UInt8 buf[3] = {0x09, 0x00, 0x00};
    
    buf[1] = selIndex;
    buf[2] = (int)selIndex >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendLeadCorona:(id)sender { // PARTICLE LEAD CORONA
    UInt8 buf[3] = {0x10, 0x00, 0x00};
    
    buf[1] = sliderLeadCorona.integerValue;
    buf[2] = (int)sliderLeadCorona.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

-(IBAction)sendTailCorona:(id)sender { // PARTICLE LEAD CORONA
    UInt8 buf[3] = {0x11, 0x00, 0x00};
    
    buf[1] = sliderTailCorona.integerValue;
    buf[2] = (int)sliderTailCorona.integerValue >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [ble write:data];
}

@end