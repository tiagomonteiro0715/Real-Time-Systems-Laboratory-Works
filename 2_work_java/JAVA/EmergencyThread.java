import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

public class EmergencyThread extends Thread {

    private final Mechanism mechanism = new Mechanism();
    private final Cylinder1 cylinder1 = new Cylinder1();
    private final Cylinder2 cylinder2 = new Cylinder2();
    private final CylinderStart cylinderStart = new CylinderStart();
    private final AtomicBoolean paused = new AtomicBoolean(false); // Estado de pausa
    private final AtomicBoolean blinking = new AtomicBoolean(false); // Controle do piscar da luz

    // Thread para gerenciar o piscar da luz
    private class BlinkingThread extends Thread {
        @Override
        public void run() {
            try {
                while (blinking.get()) { // Continua piscando enquanto blinking é true
                    mechanism.flashLed(60); // Pisca a luz
                    Thread.sleep(500); // Intervalo de piscar
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                mechanism.ledSwitch(0);
            }
        }
    }

    public void emergencyControl() {
        Scanner scanner = new Scanner(System.in);
        BlinkingThread blinkingThread = null;

        while (true) {
            System.out.println("Pressione 'X' para parar ou 'R' para retomar:");
            char op = scanner.next().toUpperCase().charAt(0);

            if (op == 'X' && !paused.get()) {
                // Pausar o sistema e iniciar o piscar da luz
                paused.set(true);
                blinking.set(true);

                blinkingThread = new BlinkingThread(); // Inicia uma nova instância para piscar a luz
                blinkingThread.start();

                // Parar os mecanismos
                mechanism.conveyorStop();
                cylinder1.stop();
                cylinder2.stop();
                cylinderStart.stop();
                System.out.println("Sistema pausado. Pressione 'R' para retomar.");
            }
            else if (op == 'R' && paused.get()) {
                // Retomar o sistema e parar o piscar da luz
                paused.set(false);
                blinking.set(false); // Sinaliza para a thread de piscar parar

                if (blinkingThread != null && blinkingThread.isAlive()) {
                    blinkingThread.interrupt(); // Interrompe a thread de piscagem se estiver ativa
                }

                System.out.println("Sistema retomado.");
                mechanism.conveyorMove();
               // cylinder1.start();
                //cylinder2.start();
                //cylinderStart.start();
            }
        }
    }

    @Override
    public void run() {
        try {
            this.emergencyControl();
        } catch (Exception e) {
            Thread.currentThread().interrupt();
            System.out.println("EmergencyThread foi interrompida.");
        }
    }
}