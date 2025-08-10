#include <jni.h>
#include <SplitterConveyor.h>

JNIEXPORT void JNICALL Java_SplitterConveyor_initializeHardwarePorts(JNIEnv* ignoreEnv, jclass ignorejClass) {
    inicializeHardwarePorts();
}

//Cylinder1---------------------------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1MoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder1_moveForward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1MoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder1_moveBackward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1Stop(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder1_stop();
}
JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinder1Position(JNIEnv* ignoreEnv, jclass ignorejClass) {
    return getCylinder1Position();
}
JNIEXPORT bool JNICALL Java_SplitterConveyor_isBoxAtSensor1(JNIEnv* ignoreEnv, jclass ignorejClass) {
    return isBoxAtSensor1();
}
//----------------------------------------------------------------------------------------------------------------

//Cylinder2-------------------------------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2MoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder2_moveForward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2MoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder2_moveBackward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2Stop(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinder2_stop();
}
JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinder2Position(JNIEnv* ignoreEnv, jclass ignorejClass) {
    return getCylinder2Position();
}
JNIEXPORT bool JNICALL Java_SplitterConveyor_isBoxAtSensor2(JNIEnv* ignoreEnv, jclass ignorejClass) {
    return isBoxAtSensor2();
}
//-------------------------------------------------------------------------------------------------------------------

//CylinderStart----------------------------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartMoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinderStart_moveForward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartMoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinderStart_moveBackward();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartStop(JNIEnv* ignoreEnv, jclass ignorejClass) {
    cylinderStart_stop();
}
JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinderStartPosition(JNIEnv* ignoreEnv, jclass ignorejClass) {
    return getCylinderStartPosition();
}
//--------------------------------------------------------------------------------------------------------------------

//Conveyor--------------------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_SplitterConveyor_conveyorMove(JNIEnv* ignoreEnv, jclass ignorejClass) {
    conveyorMove();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_conveyorStop(JNIEnv* ignoreEnv, jclass ignorejClass) {
    conveyorstop();
}
//----------------------------------------------------------------------------------------------------------

//Dock-------------------------------------------------------------------------------------------------------
JNIEXPORT bool JNICALL Java_SplitterConveyor_isBoxAtDock1(JNIEnv* ignoreEnv, jclass ignoreJClass) {
    return isBoxAtDock1();
}
JNIEXPORT bool JNICALL Java_SplitterConveyor_isBoxAtDock2(JNIEnv* ignoreEnv, jclass ignoreJClass) {
    return isBoxAtDock1();
}
JNIEXPORT bool JNICALL Java_SplitterConveyor_isBoxAtDockEnd(JNIEnv* ignoreEnv, jclass ignoreJClass) {
    return isBoxAtDock1();
}
//---------------------------------------------------------------------------------------------------------------

JNIEXPORT jint JNICALL Java_SplitterConveyor_getIdentificationSensors(JNIEnv* ignoreEnv, jclass ignoreJClass) {
    return getIdentificationSensors();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_turnOnLed(JNIEnv* ignoreEnv, jclass ignorejClass) {
    turnOnLed();
}
JNIEXPORT void JNICALL Java_SplitterConveyor_turnOffLed(JNIEnv* ignoreEnv, jclass ignorejClass) {
    turnOffLed();
}