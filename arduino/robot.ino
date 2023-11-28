#include <Wire.h>

int ENA = 5;
int ENB = 6;
int IN1 = 10;
int IN2 = 11;
int IN3 = 12;
int IN4 = 13;

void setup(){
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    Serial.begin(115200);

}

void loop(){
    Wire.onReceive(receiveEvent);

}

