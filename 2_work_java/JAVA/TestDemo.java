import java.util.Scanner;

public class TestDemo {

    public static void main(String[] args) throws InterruptedException {
        int t=-1;
        SplitterConveyor.initializeHardwarePorts();
        Cylinder1 cylinder1 = new Cylinder1();
        Cylinder2 cylinder2 = new Cylinder2();
        CylinderStart cylinderStart = new CylinderStart();
        Mechanism mechanism = new Mechanism();
        while(t!=0){
            System.out.println("Enter an option: ");
            Scanner sc = new Scanner(System.in);
            t=sc.nextInt();
            switch(t){
                case 1: cylinder1.gotoPosition(1); break;
                case 2: cylinder1.calibration(); break;
                case 3: cylinder1.gotoPosition(0); break;
                case 4: cylinder2.gotoPosition(1); break;
                case 5: cylinder2.gotoPosition(0); break;
                case 6: mechanism.flashLed(3); break;
                case 7: mechanism.ledSwitch(1); break;
            }
        }
    }

}
