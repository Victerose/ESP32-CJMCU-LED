#include <Adafruit_NeoPixel.h>
#define PIN 25
#define MAX_LED 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );

void hsv2rgb(uint16_t h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b){
    int hi = (h / 60) % 6;  // in which section of the color hexcone
    float f = h * 1.0 / 60 - hi;

    float rgb_max = v;
    float rgb_min = v*(1-s);  //float p = v * (1 - s);
    float rgb_mid;
    if(hi%2==0){
        rgb_mid = f*(rgb_max-rgb_min)+rgb_min; //float t = v * (1- (1 - f) * s);
    }else{
        rgb_mid = rgb_max-f*(rgb_max-rgb_min); //float q = v * (1 - f * s);
    }

    switch (hi){
        case 0:
            *r = 255 * rgb_max; //v;
            *g = 255 * rgb_mid; //t;
            *b = 255 * rgb_min; //p;
            break;
        case 1:
            *r = 255 * rgb_mid; //q;
            *g = 255 * rgb_max; //v;
            *b = 255 * rgb_min; //p;
            break;
        case 2:
            *r = 255 * rgb_min; //p;
            *g = 255 * rgb_max; //v;
            *b = 255 * rgb_mid; //t;
            break;
        case 3:
            *r = 255 * rgb_min; //p;
            *g = 255 * rgb_mid; //q;
            *b = 255 * rgb_max; //v;
            break;
        case 4:
            *r = 255 * rgb_mid; //t;
            *g = 255 * rgb_min; //p;
            *b = 255 * rgb_max; //v;
            break;
        case 5:
            *r = 255 * rgb_max; //v;
            *g = 255 * rgb_min; //p;
            *b = 255 * rgb_mid; //q;
            break;
    }
}

uint8_t r[24],g[24],b[24];
uint16_t mem[8];

void set_color(){
    for(uint8_t i=0;i<MAX_LED;i++){
        //Serial.println(String(r[i]) + ", " + String(g[i]) + ", " + String(b[i]));
        strip.setPixelColor(i, r[i], g[i], b[i]);
    }
    strip.show();
}

void light_flow_setup(uint16_t T=30,uint16_t bias=120){
    mem[0]=T;    //in second
    mem[1]=bias; //phase bias in degree
    mem[2]=0;    //phase
    mem[3]=(uint16_t)millis()/1000; //last update time in second
}

bool light_flow(){
    uint16_t *pT=mem,*pbias=mem+1,*pphase=mem+2,*pt=mem+3;
    uint16_t now=(uint16_t)millis()/1000;
    uint16_t delta_phase=(now-*pt)*360/(*pT);
    if(delta_phase>4){
        *pphase+=delta_phase;
        *pt=now;
        for(uint8_t j=0;j<3;j++){
            uint8_t h=(*pphase+j*(*pbias))%360;
            for(uint8_t i=0;i<8;i++){
                uint8_t index_bias=i+8*j;
                hsv2rgb(h,1,1,r+index_bias,g+index_bias,b+index_bias);
        }}
        normalize_bright();
        return true;
    }else{
        return false;
    }
}

void normalize_bright(){
    for(uint8_t i=8;i<MAX_LED;i++){
        uint16_t sum=r[i]+g[i]+b[i];
        r[i]=r[i]*255.0/sum;
        g[i]=g[i]*255.0/sum;
        b[i]=b[i]*255.0/sum;
    }
}

void setup(){
    strip.begin();
    Serial.begin(115200);
    light_flow_setup();
}

void loop(){
    if(light_flow()){set_color();}
    delay(1000);
}
