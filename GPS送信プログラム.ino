/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Changed 2017.11.01 OpenWave inc, 
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 * 
 * Change DEVADDR to a unique address!
 * See http://thethingsnetwork.org/wiki/AddressSpace
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 * Required Library: 
 *    * https://github.com/matthijskooijman/arduino-lmic 
 *    * https://github.com/adafruit/DHT-sensor-library
 *    * https://github.com/adafruit/Adafruit_Sensor
 * 
 * Require Hardware:
 *    * LoRa Shield + Arduino
 *    * LoRa GPS Shield + Arduino 
 *    * LoRa Mini etc. 
 *    
 *    このサンプルは、The Things NetworkにABPで、DHT11の
 *    温度、湿度のデータをCayenneLPPのペイロードで送信します。
 *    
 *    2017.11.01 株式会社オープンウェーブ
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "CayenneLPP.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

float a;
float b;
float c;

void smartDelay(unsigned long ms);

SoftwareSerial gps(3,4);
TinyGPSPlus tinyGPS;
/*
 * 以下の、デバイスアドレス、ネットワークセッションキー、アプリケーションセッションキー
 * を、The Things Networkで取得した値に変更してください。
 */
// デバイスアドレス
static const u4_t DEVADDR = 0x26041C03;
// ネットワークセッションキー
static const PROGMEM u1_t NWKSKEY[16] = { 0x5E, 0x21, 0x2C, 0x46, 0x9B, 0x3B, 0x0B, 0x9F, 0x47, 0x0F, 0x8A, 0x19, 0xE0, 0x75, 0x4A, 0x2D };
// アプリケーションセッションキー
static const u1_t PROGMEM APPSKEY[16] = { 0xB7, 0xA8, 0xA0, 0x89, 0xEA, 0xBE, 0x87, 0x61, 0x06, 0xA5, 0xC8, 0xFF, 0x99, 0xEE, 0xD2, 0x25 };

// 以下の変数はOTAAでアクティベーションする際に、TTNから割り当てられる値となります。
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static uint8_t mydata[4];
static osjob_t initjob,sendjob,blinkjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 1;


// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};
void do_send(osjob_t* j){

    CayenneLPP lpp(51);                    // create a buffer of 51 bytes to store the payload

    lpp.reset();                           // clear the buffer
    lpp.addGPS(1,a,b,c);// channel 2, pressure
    lpp.addAnalogInput(2,c);
    
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println("OP_TXRXPEND, not sending");
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);
        Serial.println("Packet queued");
        Serial.println(LMIC.freq);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    Serial.println(ev);
    switch(ev) {

        case EV_TXCOMPLETE:
            Serial.println("EV_TXCOMPLETE (includes waiting for RX windows)");
            if(LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print("Data Received: ");
                Serial.write(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
                Serial.println();
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
         default:
            Serial.println("Other Event");
            break;
    }
}

void setup() {

    memset(mydata, 0x00, 4);
    
    Serial.begin(9600);
    gps.begin(9600);
    
    //delay(1000); 
    //bno.setExtCrystalUse(true);

    while(!Serial);
    
    Serial.println("Starting");
    
    delay(1000);

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    //LMIC_setClockError(MAX_CLOCK_ERROR * 1/100);
    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly 
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif
    
    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;
    
    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF10,14);

    // Start job
    do_send(&sendjob);
}

void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do{
    while(gps.available())
      tinyGPS.encode(gps.read());
    }while(millis()-start<ms);
  }

void loop() {
    a = tinyGPS.location.lat(); 
    b = tinyGPS.location.lng();
    c = tinyGPS.altitude.meters();
    os_runloop_once();
    smartDelay(1000);
}
