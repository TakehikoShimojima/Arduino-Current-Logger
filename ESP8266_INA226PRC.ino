#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <Wire.h>
#include "INA226PRC.h"

extern "C" {
#include "user_interface.h"
}

#define LED 4
#define SDA 14
#define SCL 13

INA226PRC ina226prc;

Ticker t1;
volatile int t1flag;

#define SAMPLE_PERIOD 5     // サンプリング間隔(ミリ秒)
#define SAMPLE_SIZE 4000     // 5ms x 4000 = 20秒

short ampbuf[SAMPLE_SIZE];
int _indx = 0;

int startflag = 0;

void t1callback() {
    t1flag = 1;
}

void setup()
{
    Serial.begin(115200);
    delay(20);
    Serial.println("Start");
    pinMode(LED, OUTPUT);

    ina226prc.begin(SDA, SCL);

    Serial.print("Manufacture ID: ");
    Serial.println(ina226prc.readId(), HEX);

    t1flag = 0;
    t1.attach_ms(SAMPLE_PERIOD, t1callback);
}

void loop()
{
    short amp;
    
    while (t1flag == 0) {
        yield();
    }
    t1flag = 0;

    amp = ina226prc.readCurrentReg();
    if (amp > 3 || amp < -3) {
        startflag = 1;
    }
    if (startflag == 0) {
        return;
    }
    ampbuf[_indx] = amp;
    if (++_indx < SAMPLE_SIZE) {
        return;
    }

    t1.detach();
    for (_indx = 0; _indx < SAMPLE_SIZE; _indx++) {
        Serial.print(_indx);
        Serial.print(", ");
        Serial.println(ampbuf[_indx] * 0.1);
    }
}

