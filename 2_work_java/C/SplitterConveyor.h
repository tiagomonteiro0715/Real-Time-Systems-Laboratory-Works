#pragma once

#include <interface.h>

void setBitValue(uInt8* variable, int n_bit, int new_value_bit);
int getBitValue(uInt8 value, uInt8 n_bit);

void cylinder1_moveForward();
void cylinder1_moveBackward();
void cylinder1_stop();
int getCylinder1Position();
bool isBoxAtSensor1();

void cylinder2_moveForward();
void cylinder2_moveBackward();
void cylinder2_stop();
int getCylinder2Position();
bool isBoxAtSensor2();

void cylinderStart_moveForward();
void cylinderStart_moveBackward();
void cylinderStart_stop();
int getCylinderStartPosition();

void conveyorMove();
void conveyorstop();

bool isBoxAtDock1();
bool isBoxAtDock2();
bool isBoxAtDockEnd();

int getIdentificationSensors();
void inicializeHardwarePorts();
void turnOnLed();
void turnOffLed();