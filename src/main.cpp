#include <Arduino.h>
#include "Looper.h"
#include "Drive.h"

Looper enabledLooper(0.02);

Drive& drive = Drive::getInstance();

void setup() {
    Serial.begin(115200);

    drive.registerEnabledLoops(&enabledLooper);

    enabledLooper.start();
    
    Serial.println("Robot Initialized");
}

void loop() {
    double joy_x = 0.0; 
    double joy_y = 0.5; 
    double joy_rot = 0.0;

    drive.setOpenLoop(joy_y, joy_x, joy_rot);

    drive.readPeriodicInputs();
    drive.writePeriodicOutputs();

    delay(20);
}