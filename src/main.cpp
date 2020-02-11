#include <Arduino.h>
#include <M5Stack.h>
#undef min

#include "TinyIbis.h"

TinyIBIS::Ibis ibisSlave;

void setup() {
    M5.begin();
    Serial.begin(115200);
    Serial.println("\nStarting");
    pinMode(36, INPUT);
    pinMode(26, OUTPUT);
    Serial2.begin(1200, SERIAL_7E2, 16, 17);
    digitalWrite(26, LOW);
    Serial.println("Checking IBIS:");
    if (digitalRead(36)) {
        Serial.println("IBIS OKAY");
    } else {
        Serial.println("IBIS NOT-OKAY");
    }
    Serial.println("--------------");
};

void loop() {
    if (Serial2.available() > 0) {
        uint8_t c = Serial2.read();
        // Serial.write(c);
        switch (ibisSlave.encode(c)) {
            case TinyIBIS::t_telegram::DS001:
                Serial.println("Bus Line Number");
                break;
            case TinyIBIS::t_telegram::DS003:
                Serial.println("Next Stop");
                break;
            case TinyIBIS::t_telegram::DS010E:
                Serial.println("Current Delay");
                break;
            default:
                break;
        }
    }
    delay(2);
};
