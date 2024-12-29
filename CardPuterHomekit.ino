/**
 * @author KwonBoWon
 */
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "Adafruit_NeoPixel.h"
#include "HomeSpan.h"
#include "DEV_LED.h"
// TODO 펑션키로 방향키 변경
#define PIN 21
#define NUM_LEDS 1
Adafruit_NeoPixel rgb21 = Adafruit_NeoPixel(NUM_LEDS , PIN , NEO_RGB + NEO_KHZ800);
boolean isSwitching = false;
int currentProc = 0;
int currentSelect = 0;
char displayBuffer[5][20];
String data = "> ";

void removeLastChar(char* buffer) {
    int length = strlen(buffer);
    if (length > 0) {
        buffer[length - 1] = '\0';
    }
}
void setHomeUI(){
    snprintf(displayBuffer[0], 20, "CardPuterHomekit");
    snprintf(displayBuffer[1], 20, "WiFi");
    snprintf(displayBuffer[2], 20, "PairingCode");
    snprintf(displayBuffer[3], 20, "Device");
    snprintf(displayBuffer[4], 20, "Setting");
}
void setWiFiUI(){
    snprintf(displayBuffer[0], 20, "Back");
    snprintf(displayBuffer[1], 20, "SSID");
    snprintf(displayBuffer[2], 20, "");
    snprintf(displayBuffer[3], 20, "PASSWORD");
    snprintf(displayBuffer[4], 20, "");
}
void setPairingUI(){
    snprintf(displayBuffer[0], 20, "Back");
    snprintf(displayBuffer[1], 20, "ParinigCode(8numbers)");
    snprintf(displayBuffer[2], 20, "");
    snprintf(displayBuffer[3], 20, "QRID(8numbers)");
    snprintf(displayBuffer[4], 20, "");
}
void drawDisplay(){
    M5Cardputer.Display.clearDisplay();
    M5Cardputer.Display.setCursor(0, 0);
    char pre;
    for(int i=0; i < 5; i++){
        pre = (i==currentSelect) ? '>' : '_';
        M5Cardputer.Display.printf("%c%s\n", pre, displayBuffer[i]);
    }
}
void setWifi(){
    homeSpan.setWifiCredentials(displayBuffer[2], displayBuffer[4]);
    Serial.println(displayBuffer[2]);
    Serial.println(displayBuffer[4]);
}
void setPairing(){
    homeSpan.setPairingCode(displayBuffer[2]);
    Serial.println(displayBuffer[2]);
}
void setQRID(){
    homeSpan.setQRID(displayBuffer[4]);
    Serial.println(displayBuffer[4]);
}
void getDevice(){
    homeSpan.statusString(HS_STATUS s);
}
void homeLoop(){
    if (M5Cardputer.Keyboard.isChange()) {
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (M5Cardputer.Keyboard.isPressed()) {
            if(status.fn && M5Cardputer.Keyboard.isKeyPressed(';')){
                Serial.println("UP");
                currentSelect = currentSelect>0 ? currentSelect-1 : currentSelect;                
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(status.fn && M5Cardputer.Keyboard.isKeyPressed('.')){
                Serial.println("DOWN");
                currentSelect = currentSelect<4 ? currentSelect+1 : currentSelect;
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(status.enter){
                Serial.println("ENTER");
                currentProc = currentSelect;
                isSwitching = true;
                currentSelect = 0;
                
            }
        }
    }
}
void wifiLoop(){
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            if(status.fn && M5Cardputer.Keyboard.isKeyPressed(';')){
                Serial.println("UP");
                currentSelect = currentSelect>0 ? currentSelect-1 : currentSelect;                
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(status.fn && M5Cardputer.Keyboard.isKeyPressed('.')){
                Serial.println("DOWN");
                currentSelect = currentSelect<4 ? currentSelect+1 : currentSelect;
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(status.enter){
                Serial.println("ENTER");
                if (currentSelect == 0){
                    currentProc = 0;
                    isSwitching = true;
                    currentSelect = 0;
                }
                else if(currentSelect == 2){
                    
                }
                else if(currentSelect == 4){
                    setWifi();
                }
            }
            else if (status.del) {
                if (currentSelect == 2){
                    removeLastChar(displayBuffer[2]);
                    drawDisplay();
                }
                else if(currentSelect == 4){
                    removeLastChar(displayBuffer[4]);
                    drawDisplay();
                }
            }
            else{
                for (auto i : status.word) {
                    if (currentSelect == 2){
                        snprintf(displayBuffer[2] + strlen(displayBuffer[2]), 20, "%c", i);
                        drawDisplay();
                    }
                    else if(currentSelect == 4){
                        snprintf(displayBuffer[4] + strlen(displayBuffer[4]), 20, "%c", i);
                        drawDisplay();
                    }
                }
            }
        }
    }
}
void paringLoop(){
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            if(M5Cardputer.Keyboard.isKeyPressed(';')){
                Serial.println("UP");
                currentSelect = currentSelect>0 ? currentSelect-1 : currentSelect;                
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(M5Cardputer.Keyboard.isKeyPressed('.')){
                Serial.println("DOWN");
                currentSelect = currentSelect<4 ? currentSelect+1 : currentSelect;
                Serial.println(currentSelect);
                drawDisplay();
            }
            else if(status.enter){
                Serial.println("ENTER");
                if (currentSelect == 0){
                    currentProc = 0;
                    isSwitching = true;
                    currentSelect = 0;
                }
                else if(currentSelect == 2){
                    setPairing();
                }
                else if(currentSelect == 4){
                    setQRID();
                }
            }
            else if (status.del) {
                if (currentSelect == 2){
                    removeLastChar(displayBuffer[2]);
                    drawDisplay();
                }
                else if(currentSelect == 4){
                    removeLastChar(displayBuffer[4]);
                    drawDisplay();
                }
            }
            else{
                for (auto i : status.word) {
                    if('0'<= i && i <= '9'){
                        if (currentSelect == 2){
                            snprintf(displayBuffer[2] + strlen(displayBuffer[2]), 20, "%c", i);
                            drawDisplay();
                        }
                        else if(currentSelect == 4){
                            snprintf(displayBuffer[4] + strlen(displayBuffer[4]), 20, "%c", i);
                            drawDisplay();
                        }
                    }
                }
            }
        }
    }
}

void setup() {
    Serial.begin(115200); // W로 wifi세팅

    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    M5Cardputer.Display.setTextColor(TFT_GREEN);
    M5Cardputer.Display.setTextFont(&fonts::FreeSans24pt7b); //FreeSerifBold18pt7b
    
    rgb21.begin();
    rgb21.setBrightness(255);
    
    //homeSpan.setPairingCode("11122333"); // 홈킷에 입력
    //homeSpan.setQRID("111-22-333");
    homeSpan.begin(Category::Bridges, "Cardputer Bridge");
    
    // creates a web log on the URL /HomeSpan-[DEVICE-ID].local:[TCP-PORT]/myLog
    //homeSpan.enableWebLog(10,"pool.ntp.org","UTC","myLog");

    new SpanAccessory(); // Bridge
    new Service::AccessoryInformation();
        new Characteristic::Identify();

    new SpanAccessory(); // RGB LED                                     
        new Service::AccessoryInformation();    
            new Characteristic::Identify();               
            new Characteristic::Name("RGB LED"); 
        new DEV_RgbLED();
    
    setHomeUI();
    drawDisplay();
    drawDisplay();
}

void loop() {
    homeSpan.poll();
    M5Cardputer.update();
    if(isSwitching){
        isSwitching = false;
        switch(currentProc){
            case 0: // Main menum
                setHomeUI();
                drawDisplay();
                Serial.println("HomeUI");
                break;
            case 1: // Set Wifi
                setWiFiUI();
                drawDisplay();
                Serial.println("WiFiUI");
                break;
            case 2: // Set paringcode
                setPairingUI();
                drawDisplay();
                Serial.println("ParingUI");
                break;
            case 3: // Setting
                break;
            default:
                break;
        }
    }
    switch(currentProc){
        case 0:
            homeLoop();
            break;
        case 1:
            wifiLoop();
            break;
        case 2:
            paringLoop();
            break;
        case 3:
            break;
        default:
            break;
    }
    
    if (M5Cardputer.BtnA.wasPressed()) {
        M5Cardputer.Speaker.tone(8000, 20);
        rgb21.setPixelColor(0, rgb21.Color(0, 0, 0)); // Light off
        rgb21.show();
    }
}