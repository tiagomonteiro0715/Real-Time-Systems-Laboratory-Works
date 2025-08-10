import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class Cylinder12Thread extends Thread {

    private final AtomicBoolean close1;
    private final AtomicBoolean close2;
    private final ArrayBlockingQueue<Integer> queue;
    private final Mechanism mechanism = new Mechanism();
    private final Cylinder1 cl1 = new Cylinder1();
    private final Cylinder2 cl2 = new Cylinder2();
    public static int caixa1 = 0;
    public static int caixa2 = 0;
    public static int caixa3 = 0;
    public static int caixaR = 0;

    // Construtor da classe Cylinder12Thread
    public Cylinder12Thread(ArrayBlockingQueue<Integer> queue, AtomicBoolean close1, AtomicBoolean close2) {
        this.queue = queue;
        this.close1 = close1;
        this.close2 = close2;
    }

    // Inicialização do cilindro e interação com a fila
    public void initializeCylinderThread() throws InterruptedException {
        while (true) { // Loop contínuo para processar caixas repetidamente
            int box = queue.take(); // Pega o próximo valor da fila de caixas

            // Leitura dos estados de close1 e close2
            boolean isClose1 = close1.get();
            boolean isClose2 = close2.get();

            if (box == 1 && !isClose1) {
                mechanism.conveyorStop();
                cl1.gotoPosition(1);
                Thread.sleep(500); // Tempo para o cilindro alcançar a posição
                cl1.gotoPosition(0);
                mechanism.conveyorMove();
                mechanism.flashLed(1);
                caixa1++;

            } else if (box == 2 && !isClose2) {
                // Aguarda enquanto o sensor do cilindro 2 indica que ele não está pronto
                while (!mechanism.sensorCylinder2()) {
                    Thread.sleep(100); // Espera para liberar a CPU e evitar uso excessivo
                }
                mechanism.conveyorStop();
                cl2.gotoPosition(1);
                Thread.sleep(500); // Tempo para o cilindro alcançar a posição
                cl2.gotoPosition(0);
                mechanism.conveyorMove();
                mechanism.flashLed(2);
                caixa2++;

            } else if (box == 3 || isClose1 || isClose2) {
                // Caso especial para boxes tipo 3 ou quando a doca está fechada
                while (!mechanism.sensorCylinder2()) {
                    Thread.sleep(100); // Espera até que o sensor detecte o estado correto
                }
                Thread.sleep(1000); // Delay para garantir segurança no processo
                mechanism.flashLed(3);
                if (box == 3) {
                    caixa3++;
                }
                if (isClose1 || isClose2) {
                    caixaR++;
                }
            }
        }
    }

    public static int ncaixas1() {
        return caixa1;
    }

    public static int ncaixas2() {
        return caixa2;
    }

    public static int ncaixas3() {
        return caixa3;
    }

    public static int ncaixasR() {
        return caixaR;
    }

    @Override
    public void run() {
        try {
            initializeCylinderThread(); // Inicia o processamento do cilindro
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Thread foi interrompida");
        }
    }
}