import java.util.Scanner;

public class Cylinder1 implements Cylinder {
    @Override
    public boolean boxDetected() {
        return false;
    }

    @Override
    public void moveForward(){
        SplitterConveyor.cylinder1MoveForward();
    }

    @Override
    public void moveBackward(){
        SplitterConveyor.cylinder1MoveBackward();
    }

    @Override
    public void stop(){
        SplitterConveyor.cylinder1Stop();
    }

    @Override
    public int getPosition(){
        return SplitterConveyor.getCylinder1Position();
    }

    @Override
    public void gotoPosition(int position){
        if (this.getPosition() != position) {
            if (position == 0) {
                this.moveBackward();
            } else {
                this.moveForward();
            }
            while (this.getPosition() != position) {
                this.getPosition();
            }
            stop();
        }
    }

    @Override
    public void calibration() {
        Scanner scanner = new Scanner(System.in);
        char t = '\0'; // Initialize with a placeholder value
        Menu menu = new Menu();

        while (t!='E') {
            System.out.println("A-> Move Forward | D->Move Backward | E->EXIT");
            t = scanner.next().toUpperCase().charAt(0);
            int pos_atual = getPosition();

            switch (t) {
                case 'A':
                    if(getPosition()!=1) {
                        System.out.println("Moving...");
                        this.moveForward();

                        while (this.getPosition() == -1 || this.getPosition() == pos_atual) {
                            this.getPosition();
                        }
                        stop();
                    }else{
                        System.out.println("Reched limit");
                    }
                    break;
                case 'D':
                    if(getPosition()!=0) {
                        System.out.println("Moving...");
                        this.moveBackward();
                        while (this.getPosition() == -1 || this.getPosition() == pos_atual) {
                            this.getPosition();
                        }
                        stop();
                    }else{
                        System.out.println("Reched limit");
                    }
                    break;
                case 'E':
                    menu.menuCalibration();
                    break;
                default:
                    System.out.println("Invalid input");
            }
        }
    }
}
