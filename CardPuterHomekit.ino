/**
 * @author KwonBoWon
 */
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "Adafruit_NeoPixel.h"
#include "HomeSpan.h"
#include "DEV_LED.h"
#include "esp_log.h"

#define PIN 21
#define NUM_LEDS 1
Adafruit_NeoPixel rgb21 = Adafruit_NeoPixel(NUM_LEDS , PIN , NEO_RGB + NEO_KHZ800);

String cliBuffer = "";
String displayBuffer = "";

M5Canvas canvas(&M5Cardputer.Display);
String data = "> ";

static char logBuffer[512];

int customLogHandler(const char *format, va_list args) {
  vsnprintf(logBuffer, sizeof(logBuffer), format, args);  // 로그 데이터를 문자열로 변환
  Serial.print(logBuffer);                                // 시리얼 출력
  M5Cardputer.Display.clearDisplay();                                         // 디스플레이 화면 초기화
  M5Cardputer.Display.drawString(logBuffer, 10, 10);                   // 디스플레이 출력
  return 0;
}

void setup() {
    Serial.begin(115200); // W로 wifi세팅


    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    M5Cardputer.Display.setTextFont(&fonts::FreeSerifBold18pt7b);
    
    // outputrect
    M5Cardputer.Display.drawRect(0, 0, M5Cardputer.Display.width(),
                                 M5Cardputer.Display.height() - 28, GREEN);
    M5Cardputer.Display.setTextFont(&fonts::FreeSerifBold18pt7b);
    // inputrect
    M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 4,
                                 M5Cardputer.Display.width(), 4, GREEN);

    canvas.setTextFont(&fonts::FreeSerifBold18pt7b);
    canvas.setTextSize(0.5);

    canvas.createSprite(M5Cardputer.Display.width() - 8,
                        M5Cardputer.Display.height() - 36);
    canvas.setTextScroll(true);
    canvas.println("Welcome to CardputerHomekit");
    canvas.pushSprite(4, 4);
    
    M5Cardputer.Display.drawString(data, 4, M5Cardputer.Display.height() - 24);
    
    rgb21.begin();
    rgb21.setBrightness(255);

    homeSpan.setPairingCode("11122333"); // 홈킷에 입력
    homeSpan.setQRID("111-22-333");
    homeSpan.begin(Category::Bridges, "Cardputer Bridge");
    
    // creates a web log on the URL /HomeSpan-[DEVICE-ID].local:[TCP-PORT]/myLog
    homeSpan.enableWebLog(10,"pool.ntp.org","UTC","myLog");

    new SpanAccessory(); // Bridge
    new Service::AccessoryInformation();
        new Characteristic::Identify();

    new SpanAccessory(); // RGB LED                                     
        new Service::AccessoryInformation();    
            new Characteristic::Identify();               
            new Characteristic::Name("RGB LED"); 
        new DEV_RgbLED();

    esp_log_set_vprintf(customLogHandler);
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("Example", "This is a log message!");
}

void loop() {
    homeSpan.poll();
    M5Cardputer.update();

    if (M5Cardputer.BtnA.wasPressed()) {
        M5Cardputer.Speaker.tone(8000, 20);
        rgb21.setPixelColor(0, rgb21.Color(0, 0, 0)); // Light off
        rgb21.show();
    }
}