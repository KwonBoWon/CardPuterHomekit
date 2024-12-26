/**
 * @author KwonBoWon
 */
#include "M5Cardputer.h"
#include "Adafruit_NeoPixel.h"
#include "HomeSpan.h"
#include "DEV_LED.h"

#define PIN 21
#define NUM_LEDS 1
Adafruit_NeoPixel rgb21 = Adafruit_NeoPixel(NUM_LEDS , PIN , NEO_RGB + NEO_KHZ800);
void setup() {
    Serial.begin(115200); // W로 wifi세팅

    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setTextFont(&fonts::Orbitron_Light_32);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.drawString("RGB LED",
                                   M5Cardputer.Display.width() / 2,
                                   M5Cardputer.Display.height() / 2);
                                   
    rgb21.begin();
    rgb21.setBrightness(255);

    homeSpan.setPairingCode("11122333"); // 홈킷에 입력
    homeSpan.setQRID("111-22-333");
    homeSpan.begin(Category::Bridges, "Cardputer Bridge");

    new SpanAccessory(); // Bridge
    new Service::AccessoryInformation();
        new Characteristic::Identify();

    new SpanAccessory(); // RGB LED                                     
        new Service::AccessoryInformation();    
            new Characteristic::Identify();               
            new Characteristic::Name("RGB LED"); 
        new DEV_RgbLED();
}

void loop() {
    homeSpan.poll();
    M5Cardputer.update();
    if (M5Cardputer.BtnA.wasPressed()) {
        M5Cardputer.Speaker.tone(8000, 20);
        M5Cardputer.Display.clear();
        M5Cardputer.Display.drawString("Pressed",
                                       M5Cardputer.Display.width() / 2,
                                       M5Cardputer.Display.height() / 2);
        rgb21.setPixelColor(0, rgb21.Color(255, 0, 0)); // Green light
        rgb21.show();
    }
    if (M5Cardputer.BtnA.wasReleased()) {
        M5Cardputer.Speaker.tone(8000, 20);
        M5Cardputer.Display.clear();
        M5Cardputer.Display.drawString("RGB LED",
                                       M5Cardputer.Display.width() / 2,
                                       M5Cardputer.Display.height() / 2);
        rgb21.setPixelColor(0, rgb21.Color(0, 0, 0)); // Black light(turn off)
        rgb21.show();
    }
}