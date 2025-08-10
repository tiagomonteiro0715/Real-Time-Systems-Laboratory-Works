import java.util.Scanner;

public class CylinderStart implements Cylinder {
    @Override
    public boolean boxDetected() {
        return false;
    }

    @Override
    public void moveForward() {
        SplitterConveyor.cylinderStartMoveForward();
    }

    @Override
    public void moveBackward() {
        SplitterConveyor.cylinderStartMoveBackward();
    }

    @Override
    public void stop() {
        SplitterConveyor.cylinderStartStop();
    }

    @Override
    public int getPosition() {
        return SplitterConveyor.getCylinderStartPosition();
    }

    @Override
    public void gotoPosition(int position) {
        long begin = System.currentTimeMillis(); // Obtém o tempo inicial em milissegundos
        long timeout = begin + 5000;

        if (this.getPosition() != position) {
            if (position == 0) {
                this.moveBackward();
            } else {
                this.moveForward();
            }

            // Aguarda até o cilindro atingir a posição desejada ou o tempo limite
            while (this.getPosition() != position && System.currentTimeMillis() < timeout) {
                try {
                    Thread.sleep(50); // Pausa para evitar uso intenso da CPU
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt(); // Restaura o status de interrupção
                }
            }

            // Adiciona um pequeno tempo extra para garantir que o movimento seja completo
            try {
                Thread.sleep(200); // Extende o movimento por 200ms
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            stop(); // Para o cilindro após atingir a posição
        }
    }

    @Override
    public void calibration() {
        Scanner scanner = new Scanner(System.in);
        char t = '\0'; // Initialize with a placeholder value
        Menu menu = new Menu();

        while (t != 'E') {
            System.out.println("A-> Move Forward | D->Move Backward | E->EXIT");
            t = scanner.next().toUpperCase().charAt(0);
            int pos_atual = getPosition();

            switch (t) {
                case 'A':
                    if (getPosition() != 1) {
                        System.out.println("Moving...");
                        this.moveForward();

                        while (this.getPosition() == -1 || this.getPosition() == pos_atual) {
                            this.getPosition();
                        }
                        stop();
                    } else {
                        System.out.println("Reached limit");
                    }
                    break;
                case 'D':
                    if(getPosition() != 0) {
                        System.out.println("Moving...");
                        this.moveBackward();
                        while (this.getPosition() == -1 || this.getPosition() == pos_atual) {
                            this.getPosition();
                        }
                        stop();
                    }else{
                        System.out.println("Reached limit");
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
