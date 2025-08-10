#pragma once

extern "C" {
#include <interface.h>

}

int getBitValue(uInt8 value, uInt8 bit_n);
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);

// X axis related functions
void moveXLeft();
void moveXRight();
void stopX();
int getXPos();
void gotoX(int x);

// Y axis related functions
void moveYInside();
void moveYOutside();
void stopY();
int getYPos();
void gotoY(int y);

// Z axis related functions
void moveZUp();
void moveZDown();
void stopZ();
int getZPos();
void gotoZ(int z);
int Platecount();
uInt8 getEmergencyP2();
void updateEmergencyP2(uInt8 p);
void SemiAutoCalibration();
void turnOnGreenUpLight();
void turnOffGreenUpLight();
void turnOnGreenDownLight();
void turnOffGreenDownLight();
//void turnOffDownFlashingLight();