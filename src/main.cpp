#include <Arduino.h>
#include <Wire.h>
#include "VCNL3036.h"
#include <stdio.h>


#define INT_PIN p0 // Define the interrupt pin number, change '0' to the correct pin number

VCNL3036 proximity_sensor;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    proximity_sensor.setInterruptPin(INT_PIN); // Set the interrupt pin
    pinMode(INT_PIN, INPUT_PULLUP); // Set the interrupt pin as input with pull-up

    if (!proximity_sensor.begin(Wire)) {
        Serial.println("Sensor initialization failed");
        while (1);
    }

    if (!proximity_sensor.defaultConfiguration()) {
        Serial.println("Failed to apply default configuration");
        while (1);
    }

    Serial.println("Setup complete");
}

void loop() {
    uint16_t proximity_data = proximity_sensor.readProximity();
    Serial.print("Proximity: ");
    Serial.println(proximity_data);
    delay(1000); // Wait for 1 second before reading again.
}

int main() {
    init();
    setup();
    while (true) {
        loop();
    }
    return 0;
}
