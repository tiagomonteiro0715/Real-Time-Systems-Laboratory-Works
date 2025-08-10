#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <interface.h>


void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
	uInt8 mask_on = (uInt8)(1 << n_bit);
	uInt8 mask_off = ~mask_on;
	if (new_value_bit) *variable |= mask_on;
	else *variable &= mask_off;
}
int getBitValue(uInt8 value, uInt8 n_bit)
// given a byte value, returns the value of bit n
{
	return(value & (1 << n_bit));
}

void cylinder1_moveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 1);
	setBitValue(&p, 3, 0);
	writeDigitalU8(2, p);
}
void cylinder1_moveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 0);
	setBitValue(&p, 3, 1);
	writeDigitalU8(2, p);
}
void cylinder1_stop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 0);
	setBitValue(&p, 3, 0);
	writeDigitalU8(2, p);
}

void cylinder2_moveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 6, 1);
	setBitValue(&p, 5, 0);
	writeDigitalU8(2, p);
}
void cylinder2_moveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 6, 0);
	setBitValue(&p, 5, 1);
	writeDigitalU8(2, p);
}
void cylinder2_stop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 6, 0);
	setBitValue(&p, 5, 0);
	writeDigitalU8(2, p);
}

void cylinderStart_moveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 1);
	setBitValue(&p, 0, 0);
	writeDigitalU8(2, p);
}
void cylinderStart_moveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 0);
	setBitValue(&p, 0, 1);
	writeDigitalU8(2, p);
}
void cylinderStart_stop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 0);
	setBitValue(&p, 0, 0);
	writeDigitalU8(2, p);
}

void inicializeHardwarePorts() {
	printf("\nwaiting for hardware simulator...");
	printf("\nReminding: gotoXZ requires kit calibration first...");
	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);
	writeDigitalU8(2, 0);
	printf("\ngot access to simulator...");
}

void conveyorMove()
{
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 2, 1);
	writeDigitalU8(2, p); // update port 2
}

void conveyorstop()
{
	uInt8 p = readDigitalU8(2); // read port 2

	setBitValue(&p, 2, 0);

	writeDigitalU8(2, p); // update port 2
}

int getCylinder1Position() {
	uInt8 p0 = readDigitalU8(0);

	if (!getBitValue(p0, 4))
		return 0;
	if (!getBitValue(p0, 3))
		return 1;
	return (-1);
}

int getCylinder2Position() {
	uInt8 p0 = readDigitalU8(0);

	if (!getBitValue(p0, 2))
		return 0;
	if (!getBitValue(p0, 1))
		return 1;
	return (-1);
}

int getCylinderStartPosition() {
	uInt8 p0 = readDigitalU8(0);

	if (getBitValue(p0, 6))
		return 0;
	if (getBitValue(p0, 5))
		return 1;
	return (-1);
}

bool isBoxAtDock1() {
	uInt8 p1 = readDigitalU8(1);
	return (getBitValue(p1, 4));
}

bool isBoxAtDock2() {
	uInt8 p1 = readDigitalU8(1);
	return (getBitValue(p1, 3));
}
bool isBoxAtDockEnd() {
	uInt8 p1 = readDigitalU8(1);
	return (getBitValue(p1, 2));
}

int getIdentificationSensors() {
	uInt8 p1 = readDigitalU8(1); // Read port 1

	if (getBitValue(p1, 5)) {
		return 1; // Sensor in P1.5 is active
	}
	if (getBitValue(p1, 6)) {
		return 2; // Sensor in P1.6 is active
	}
	return 0; // No sensors are active
}

void turnOnLed() {
	uInt8 p2 = readDigitalU8(2);

	setBitValue(&p2, 7, 1);

	writeDigitalU8(2, p2);

}

void turnOffLed() {
	uInt8 p2 = readDigitalU8(2);

	setBitValue(&p2, 7, 0);

	writeDigitalU8(2, p2);

}

bool isBoxAtSensor1(){
	uInt8 p0 = readDigitalU8(0);
	return (getBitValue(p0, 0));
}

bool isBoxAtSensor2() {
	uInt8 p1 = readDigitalU8(1);
	return (getBitValue(p1, 7));
}