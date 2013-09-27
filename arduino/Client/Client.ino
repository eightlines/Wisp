#include <Streaming.h>
#include <SPI.h>

void setup() {
    Serial.begin(9600);
    Serial << "WISP Ready" << endl;
    
    BLE_setup();
    LPD8806_setup();
}

void loop() {
    BLE_loop();
    LPD8806_loop();
}
