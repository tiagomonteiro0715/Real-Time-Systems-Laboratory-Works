public class Mechanism {

    public void conveyorMove() {
        SplitterConveyor.conveyorMove();
    }

    public void conveyorStop() {
        SplitterConveyor.conveyorStop();
    }

    public boolean switchDock1Pressed() {
       return SplitterConveyor.isBoxAtDock1();
    }

    public boolean switchDock2Pressed() {
        return SplitterConveyor.isBoxAtDock2();
    }

    public boolean switchDockEndPressed() {
        return SplitterConveyor.isBoxAtDockEnd();
    }

    public boolean sensorCylinder1() {
        return SplitterConveyor.isBoxAtSensor1();
    }

    public boolean sensorCylinder2() {
        return SplitterConveyor.isBoxAtSensor2();
    }

    public void ledSwitch(int on) {
        if (on == 1) {
            SplitterConveyor.turnOnLed();
        } else {
            SplitterConveyor.turnOffLed();
        }
    }

    public void flashLed(int seconds) throws InterruptedException {

        long begin = System.currentTimeMillis(); // Obtém o tempo inicial em milissegundos
        long end = begin + seconds * 1000;      // Calcula o tempo final em milissegundos

        while (System.currentTimeMillis() < end) {
            this.ledSwitch(1);
            Thread.sleep(400);
            this.ledSwitch(0);
            Thread.sleep(400);
        }
    }
}

