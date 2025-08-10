import java.io.IOException;
import java.util.Scanner;

public class Menu {

    CylinderStart clStart = new CylinderStart();
    Cylinder1 cl1 = new Cylinder1();
    Cylinder2 cl2 = new Cylinder2();
    Scanner scanner = new Scanner(System.in);
    Processo processo = new Processo();
    boolean volta = false;
    Mechanism mech = new Mechanism();

    public void menu() {

        try {
            char t = '\0'; // Initialize with a placeholder value

            System.out.println("Options:");
            System.out.println(" Option S - Start Process");
            System.out.println(" Option F - Finish Process");
            System.out.println(" Option C- Calibration");
            System.out.println(" Option I - Statistics");
            System.out.println();

            while (t != 'F') {
                System.out.print("Enter an option: ");
                t = scanner.next().toUpperCase().charAt(0);


                switch (t) {
                    case 'S':
                        t = '\0';
                        // System.out.println("Starting calibration for all cylinders...");
                        while (t != 'B') {
                            System.out.println("P-> Start Process | B->Back");
                            t = scanner.next().toUpperCase().charAt(0);
                            if (t == 'P') {
                                Processo.processo();
                            } else if (t == 'B') {
                                this.menu();
                            } else {
                                System.out.println("Invalid option! Please try again.");
                            }

                        }
                        break;// Exit menu after calibration
                    //Da erro com o return, fazer uma classe para prosseguir
                    case 'C':
                        menuCalibration();
                        break;
                    case 'F':
                        System.out.println("Finishing game...");
                        mech.conveyorStop();
                        scanner.close();
                        System.exit(0);
                        break; // Exit menu
                    case 'I':
                        System.out.println("Displaying statistics...");
                        System.out.println("Number of boxes from Iquêá:" + Cylinder12Thread.ncaixas1());
                        System.out.println("Number of boxes from ConforRama:" + Cylinder12Thread.ncaixas2());
                        System.out.println("Number of boxes that go to the DockEnd:" + Cylinder12Thread.ncaixas3());
                        System.out.println("Number of boxes that are rejected:" + Cylinder12Thread.ncaixasR());
                        System.out.println("B->Back");
                        t = scanner.next().toUpperCase().charAt(0);
                        if (t == 'B') {
                            this.menu();
                        }
                        break;
                    default:
                        System.out.println("Invalid option! Please try again.");
                }
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void menuCalibration() {

        int op = 0; // Initialize with a placeholder value

        System.out.println("Options:");
        System.out.println(" Option 1 -> Calibration Cylinder1");
        System.out.println(" Option 2 -> Calibration Cylinder2");
        System.out.println(" Option 3 -> Calibration CylinderStart");
        System.out.println(" Option 4 - Turn Back Menu");
        System.out.println();

        while (op != 4) {
            System.out.print("Enter an option: ");
            Scanner sc = new Scanner(System.in);
            op = sc.nextInt();

            switch (op) {
                case 1:
                    cl1.calibration();
                    break;
                case 2:
                    cl2.calibration();
                    break;
                case 3:
                    clStart.calibration();
                    break;// Exit calibrationMenu
                case 4:
                    this.menu();
                    break;
                default:
                    System.out.println("Invalid option! Please try again.");
            }
        }
    }

}

