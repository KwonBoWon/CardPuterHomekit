#include <Adafruit_NeoPixel.h>

#define LED_PIN 21        // 데이터 핀
#define NUM_LEDS 1        // 제어할 LED 수 (1개)

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);  // 21pin(SK6812)

struct DEV_RgbLED : Service::LightBulb {
  SpanCharacteristic *power;  // 전원 상태
  SpanCharacteristic *H;      // 색상 (Hue)
  SpanCharacteristic *S;      // 채도 (Saturation)
  SpanCharacteristic *V;      // 밝기 (Brightness)

  DEV_RgbLED() : Service::LightBulb() {
    power = new Characteristic::On();
    H = new Characteristic::Hue(0);            // 초기값 0
    S = new Characteristic::Saturation(0);     // 초기값 0%
    V = new Characteristic::Brightness(100);   // 초기값 100%
    V->setRange(5, 100, 1);                    // 밝기 최소 5%, 최대 100%

    strip.begin();     // NeoPixel 초기화
    strip.show();      // LED를 초기화)
    Serial.println("Configuring SK6812 RGB LED with NeoPixel");
  }

  boolean update() {
    boolean p = power->getVal();  // 전원 상태
    float h = H->getVal<float>();
    float s = S->getVal<float>() / 100.0;  
    float v = V->getVal<float>() / 100.0;

    float r, g, b;
    LedPin::HSVtoRGB(h, s, v, &r, &g, &b);  // HSV를 RGB로 변환 (0~1 범위)

    uint8_t R = p * r * 255;  
    uint8_t G = p * g * 255;
    uint8_t B = p * b * 255;

    strip.setPixelColor(0, strip.Color(R, G, B));  
    strip.show();  

    char cBuf[128];
    sprintf(cBuf, "RGB=(%d,%d,%d)\n", R, G, B);
    Serial.print(cBuf);

    return true;
  }
};
