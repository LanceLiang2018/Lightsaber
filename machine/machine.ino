#include "Adafruit_NeoPixel.h"
#include "stdio.h"
#include "stdlib.h"

#define PIN 8
#define LEDS 30
// 切换颜色
#define KEY1 9
// 切换闪烁方式
#define KEY2 10

// 颜色模式种数
#define NUM_COLOR 9
// 闪烁模式种数
#define NUM_GREY 4

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

uint16_t NCOLORS[8][3] = {
  {255, 0, 0},     //红
//  {0, 0, 0},     //红
  {255, 165, 0},   //橙
  {255, 255, 0},   //黄
  {0, 0, 255},     //绿
  {0, 127, 255},   //青
  {0, 0, 255},     //蓝
  {139, 0, 255},   //紫
  {255, 255, 255}, //白
};

uint32_t get_color(MColor mcolor, uint16_t mgrey) {
  uint32_t fin = strip.Color(mcolor.r * mgrey / 255, mcolor.g * mgrey / 255, mcolor.b * mgrey / 255);
  return fin;
}

//当前切换的颜色模式
uint16_t cnt_color = 0;
uint16_t timer_color = 0;
uint16_t tmp_color = 0;
void set_color() {
  if (cnt_color < 8) {
    for (uint8_t i=0; i<LEDS; i++) {
      colors[i].set(NCOLORS[cnt_color][0], NCOLORS[cnt_color][1], NCOLORS[cnt_color][2]);
    }
  }
  if (cnt_color == 8) {
    uint16_t target;
    if (tmp_color != NUM_COLOR)
      target = tmp_color + 1;
    else
      target = 0;
    MColor pre = MColor();
    pre.set(NCOLORS[tmp_color][0], NCOLORS[tmp_color][1], NCOLORS[tmp_color][2]);
    MColor next = MColor();
    next.set(NCOLORS[target][0], NCOLORS[target][1], NCOLORS[target][2]);
    for (uint8_t i=0; i<LEDS; i++) {
      colors[i].r += (next.r - pre.r) / 100 + 0;
      colors[i].g += (next.g - pre.g) / 100 + 0;
      colors[i].b += (next.b - pre.b) / 100 + 0;
    }
    char buf[32] = "";
    sprintf(buf, "%d -> %d", tmp_color, target);
    Serial.println(buf);
    sprintf(buf, "%d, %d, %d", colors[0].r, colors[0].g, colors[0].b);
    Serial.println(buf);
    
    if (abs(colors[0].r - next.r) <= 30 && abs(colors[0].g - next.g) <= 30 && abs(colors[0].b - next.b) <= 30) {
      tmp_color++;
      if (tmp_color >= NUM_COLOR)
          tmp_color = 0;
      char buf[32] = "";
      sprintf(buf, "Switch to: %d", tmp_color);
      Serial.println(buf);
      for (uint8_t i=0; i<LEDS; i++) {
        colors[i].set(NCOLORS[tmp_color][0], NCOLORS[tmp_color][1], NCOLORS[tmp_color][2]);
      }
    }
  }
}

//当前切换的闪烁模式
uint16_t cnt_grey = 4;
uint16_t timer_grey = 0;
uint16_t tmp_grey = 0;
bool first_grey = true;
void set_grey() {
  // 常亮
  if (cnt_grey == 0) {
    for (uint8_t i=0; i<LEDS; i++) {
      grey[i] = 255;
    }
  }
  // 闪烁
  if (cnt_grey == 1) {
    if (timer_grey >= 6) {
      for (uint8_t i=0; i<LEDS; i++) {
        if (tmp_grey == 0)
          grey[i] = 255;
        else
          grey[i] = 0;
      }
      timer_grey = 0;
      tmp_grey = !tmp_grey;
    }
    timer_grey++;
  }
  // 闪烁(fast)
  if (cnt_grey == 2) {
    if (timer_grey >= 1) {
      for (uint8_t i=0; i<LEDS; i++) {
        if (tmp_grey == 0)
          grey[i] = 255;
        else
          grey[i] = 0;
      }
      timer_grey = 0;
      tmp_grey = !tmp_grey;
    }
    timer_grey++;
  }
  // 流动
  if (cnt_grey == 3) {
    if (first_grey) {
      for (uint8_t i=0; i<LEDS; i++) {
//        grey[i] = 255;
          if (i % 5 == 0)
            grey[i] = 0;
          else
            grey[i] = 255;
      }
//      grey[0] = 0;
      first_grey = false;
    }
    uint16_t tmp = grey[LEDS-1];
    for (uint8_t i=LEDS-1; i>0; i--) {
      grey[i] = grey[i-1];
    }
    grey[0] = tmp;
  }
  // 呼吸
  if (cnt_grey == 4) {
    if (first_grey) {
      tmp_grey = 0;
      timer_grey = 0;
      for (uint8_t i=0; i<LEDS; i++) {
        if (tmp_grey == 0)
          grey[i] = 0;
      }
      first_grey = false;
    }
    if (tmp_grey == 0) {
      if (timer_grey == 15) {
        timer_grey = 15;
        tmp_grey = !tmp_grey;
      }
      for (uint8_t i=0; i<LEDS; i++) {
        grey[i] = 255 * timer_grey / 15;
      }
      timer_grey++;
    }
    if (tmp_grey != 0) {
      if (timer_grey == 0) {
        timer_grey = 0;
        tmp_grey = !tmp_grey;
      }
      for (uint8_t i=0; i<LEDS; i++) {
        grey[i] = 255 * timer_grey / 15;
      }
      timer_grey--;
    }
  }
}

void setup() {

  for (uint8_t i=0; i<LEDS; i++) {
//    uint16_t col = 255 / LEDS * i;
//    colors[i].r = col; colors[i].g = col; colors[i].b = col;
    grey[i] = 255;
  }

  //使能内部上拉电阻
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  
  strip.begin();
  strip.show();

  Serial.begin(9600);
}

void loop() {
//  char buf[32] = "";
//  sprintf(buf, "KEY1: %d; KEY2: %d", digitalRead(KEY1), digitalRead(KEY2));
//  Serial.println(buf);
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
    if (digitalRead(KEY1) == 0) {
      delay(20);    // 按键消除抖动
      if (digitalRead(KEY1) == 0) {
        cnt_color++;
        if (cnt_color >= NUM_COLOR)
          cnt_color = 0;
        // 保持到按键抬起
        while (digitalRead(KEY1) == 0);
      }
    }
    if (digitalRead(KEY2) == 0) {
      delay(20);    // 按键消除抖动
      if (digitalRead(KEY2) == 0) {
        cnt_grey++;
        if (cnt_grey >= NUM_GREY)
          cnt_grey = 0;
        // 保持到按键抬起
        while (digitalRead(KEY2) == 0);
      }
    }
    delay(1);
    t++;
  }
}
