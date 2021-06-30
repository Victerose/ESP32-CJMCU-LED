#include <Adafruit_NeoPixel.h>
#define PIN 25
#define MAX_LED 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );

void hsv2rgb(uint16_t h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b){
    int hi = (h / 60) % 6;
    float f = h * 1.0 / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    switch (hi){
        case 0:
            *r = 255 * v;
            *g = 255 * t;
            *b = 255 * p;
            break;
        case 1:
            *r = 255 * q;
            *g = 255 * v;
            *b = 255 * p;
            break;
        case 2:
            *r = 255 * p;
            *g = 255 * v;
            *b = 255 * t;
            break;
        case 3:
            *r = 255 * p;
            *g = 255 * q;
            *b = 255 * v;
            break;
        case 4:
            *r = 255 * t;
            *g = 255 * p;
            *b = 255 * v;
            break;
        case 5:
            *r = 255 * v;
            *g = 255 * p;
            *b = 255 * q;
            break;
    }
}

void setup(){
    strip.begin();
    Serial.begin(115200);
}
                             
uint8_t r[8]={255,0,0,0,0,0,0,0},g[8]={255,0,0,0,0,0,0,0},b[8]={255,0,0,0,0,0,0,0};
uint16_t mem[8]={0,0,0,0,0,0,0,0};

void set_color(){
    for(uint8_t i=0;i<8;i++){
        //Serial.println(String(r[i]) + ", " + String(g[i]) + ", " + String(b[i]));
        strip.setPixelColor(i, r[i], g[i], b[i]);
    }
    strip.show();
}

void light_flow(){
    uint16_t h;
    for(uint8_t i=0;i<8;i++){
        h=(mem[0]+i*10)%360;
        hsv2rgb(h,1,1,r+i,g+i,b+i);
    }
    mem[0]+=1;
    Serial.println(mem[0]);
}

void loop(){
    light_flow();
    set_color();
    delay(100);                    
}
