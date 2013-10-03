void BLE_setup() {
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(LSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.begin();

    ble_begin();
  
    Serial << "BLE Initialized" << endl;
}

void BLE_loop() {
    while (ble_available()) {
        byte data0 = ble_read();
        byte data1 = ble_read();
        byte data2 = ble_read();
        
        if (data0 == 0x01) { // Change Mode
            style = (data1 == 0) ? 2 : 4;
        } else if (data0 == 0x02) { // R
            rgb[0] = data1;
        } else if (data0 == 0x03) { // G
            rgb[1] = data1;
        } else if (data0 == 0x04) { // B
            rgb[2] = data1;
        } else if (data0 == 0x05) { // RATE
            rate = data1;
        } else if (data0 == 0x06) { // LIGHTS ON/OFF
            style = (data1 == 0) ? 0 : 1;
        } else if (data0 == 0x07) { // RING FADE/SNAP
            style = (data1 == 0) ? 3 : 2;
        } else if (data0 == 0x08) { // POSITION
            pos = data1;
        } else if (data0 == 0x09) { // PARTICLE DIRECTION
            dir = data1;
        } else if (data0 == 0x10) { // PARTICLE LEAD CORONA
            coronaL = data1;
        } else if (data0 == 0x11) { // PARTICLE TAIL CORONA
            coronaT = data1;
        }
    }

    if (!ble_connected()) {
    }
    
    ble_do_events();
}
