/**
 * @author KwonBoWon
 */
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "Adafruit_NeoPixel.h"
#include "HomeSpan.h"
#include "DEV_LED.h"
// TODO 장치 컨트롤 버튼 임의로 지정해서 사용, 사용자 정의함수
#define PIN 21
#define NUM_LEDS 1
Adafruit_NeoPixel rgb21 = Adafruit_NeoPixel(NUM_LEDS , PIN , NEO_RGB + NEO_KHZ800);
boolean isSwitching = false;
int currentProc = 0;
int currentSelect = 0;
char displayBuffer[6][30];
char statusBuffer[30];

void removeLastChar(char* buffer) {
    int length = strlen(buffer);
    if (length > 0) {
        buffer[length - 1] = '\0';
    }
}
void setHomeUI(){
    snprintf(displayBuffer[0], 30, "CardPuterHomekit");
    snprintf(displayBuffer[1], 30, "SET WiFi");
    snprintf(displayBuffer[2], 30, "SET PairingCode");
    snprintf(displayBuffer[3], 30, "");
    snprintf(displayBuffer[4], 30, "SETTING");
}
void setTutorialUI(){
    snprintf(displayBuffer[0], 30, "Back");
    snprintf(displayBuffer[1], 30, "1. Use Fn+arrows and Enter");
    snprintf(displayBuffer[2], 30, "2. Set WiFi and Pairing Code");
    snprintf(displayBuffer[3], 30, "3. Add Accessory");
    snprintf(displayBuffer[4], 30, "4. Enter Pairing Code");
}
void setWiFiUI(){
    snprintf(displayBuffer[0], 30, "Back");
    snprintf(displayBuffer[1], 30, "SSID");
    snprintf(displayBuffer[2], 30, "");
    snprintf(displayBuffer[3], 30, "PASSWORD");
    snprintf(displayBuffer[4], 30, "");
}
void setPairingUI(){
    snprintf(displayBuffer[0], 30, "Back");
    snprintf(displayBuffer[1], 30, "ParinigCode(8numbers)");
    snprintf(displayBuffer[2], 30, "");
    snprintf(displayBuffer[3], 30, "QRID(8numbers)");
    snprintf(displayBuffer[4], 30, "");
}
void setWebLogUI(){
    snprintf(displayBuffer[0], 30, "Back");
    snprintf(displayBuffer[1], 30, "RECONNECT WiFi");
    snprintf(displayBuffer[2], 30, "DELETE WiFi DATA");
    snprintf(displayBuffer[3], 30, "DELETE HomeKit DATA");
    snprintf(displayBuffer[4], 30, "ERASE ALL STORED DATA");
}
void setSettingUI(){
    snprintf(displayBuffer[0], 30, "Back");
    snprintf(displayBuffer[1], 30, "RECONNECT WiFi");
    snprintf(displayBuffer[2], 30, "DELETE WiFi DATA");
    snprintf(displayBuffer[3], 30, "DELETE HomeKit DATA");
    snprintf(displayBuffer[4], 30, "ERASE ALL STORED DATA");
}

void drawDisplay(){
    M5Cardputer.Display.clearDisplay();
    M5Cardputer.Display.setCursor(0, 0);
    char pre;
    for(int i=0; i < 5; i++){ // selectable
        pre = (i==currentSelect) ? '>' : '_';
        M5Cardputer.Display.printf("%c%s\n", pre, displayBuffer[i]);
    }
    M5Cardputer.Display.printf("*%s\n", pre, displayBuffer[5]); // status
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
void statusUpdate(HS_STATUS status){
    Serial.printf("\n*** HOMESPAN STATUS CHANGE: %s\n",homeSpan.statusString(status));
    snprintf(displayBuffer[5], 30, homeSpan.statusString(status));
    drawDisplay();
}
void setup() {
    Serial.begin(115200); // W로 wifi세팅

    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    M5Cardputer.Display.setTextColor(TFT_GREEN);
    M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b); // 
    
    rgb21.begin();
    rgb21.setBrightness(255);
    
    homeSpan.setStatusCallback(statusUpdate);   // set callback function
    homeSpan.begin(Category::Bridges, "Cardputer Bridge");
    
    // creates a web log on the URL /HomeSpan-[DEVICE-ID].local:[TCP-PORT]/Log
    //homeSpan.enableWebLog(10,"pool.ntp.org","UTC","Log");

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
            case 2: // Set Wifi
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