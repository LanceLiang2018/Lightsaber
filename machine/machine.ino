#include "Adafruit_NeoPixel.h"

#define PIN 8
#define LEDS 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

class MColor {
public:
  uint16_t r, g, b;
  
  MColor() {
    r = 0; g = 0; b = 0;
  }

  void set(uint16_t mr, uint16_t mg, uint16_t mb) {
    r = mr; g = mg; b = mb;
  }
};

uint16_t grey[LEDS] = {};
MColor colors[LEDS] = {};

uint32_t COLORS[7] = {
  strip.Color(255, 0, 0),     //红
  strip.Color(255, 165, 0),   //橙
  strip.Color(255, 255, 0),   //黄
  strip.Color(0, 0, 255),     //绿
  strip.Color(0, 127, 255),   //青
  strip.Color(0, 0, 255),     //蓝
  strip.Color(139, 0, 255),   //紫
};

uint16_t NCOLORS[7][3] = {
//  {255, 0, 0},     //红
  {0, 0, 0},     //红
  {255, 165, 0},   //橙
  {255, 255, 0},   //黄
  {0, 0, 255},     //绿
  {0, 127, 255},   //青
  {0, 0, 255},     //蓝
  {139, 0, 255},   //紫
};

uint32_t get_color(MColor mcolor, uint16_t mgrey) {
  uint32_t fin = strip.Color(mcolor.r * mgrey / 255, mcolor.g * mgrey / 255, mcolor.b * mgrey / 255);
  return fin;
}

//当前切换的颜色模式
uint16_t cnt_color = 0;
void set_color() {
  for (uint8_t i=0; i<LEDS; i++) {
    colors[i].set(NCOLORS[cnt_color][0], NCOLORS[cnt_color][1], NCOLORS[cnt_color][2]);
  }
}

//当前切换的闪烁模式
uint16_t cnt_grey = 0;
void set_grey() {
  for (uint8_t i=0; i<LEDS; i++) {
    grey[i] = 255;
  }
}

void setup() {

  for (uint8_t i=0; i<LEDS; i++) {
//    uint16_t col = 255 / LEDS * i;
//    colors[i].r = col; colors[i].g = col; colors[i].b = col;
    grey[i] = 255;
  }
  
  strip.begin();
  strip.show();
}

void loop() {
  set_color();
  set_grey();

  for (uint8_t i=0; i<LEDS; i++)
  {
    uint32_t fin = get_color(colors[i], grey[i]);
    strip.setPixelColor(i, fin);
  }
  strip.show();

  uint16_t t = 0;

  // 在这里检测按键
  while (t < 100) {
    delay(1);
  }
}
