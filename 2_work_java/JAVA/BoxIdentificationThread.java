import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Semaphore;

public class BoxIdentificationThread extends Thread {

    private int brickType = -1;
    private ArrayBlockingQueue<Integer> queue;
    private Semaphore sem;

    // Construtor da classe
    public BoxIdentificationThread(ArrayBlockingQueue<Integer> queue) {
        this.queue = queue;  // Inicializando a fila com capacidade 30
        //this.sem = sem;  // Inicializando o semáforo
    }

    public ArrayBlockingQueue<Integer> getQueue() {
        return queue;
    }

    public int getBoxIdentification() {
        return brickType;
    }

    public void startBoxIdentification() throws InterruptedException {
        Mechanism mechanism = new Mechanism();
        boolean dot1 = false;
        boolean dot2 = false;


        // sem.acquire();

        // Espera até o sensor do cilindro ser ativado
        while (!mechanism.sensorCylinder1()) {
            if (SplitterConveyor.getIdentificationSensors() == 1) {
                dot1 = true;
            } else if (SplitterConveyor.getIdentificationSensors() == 2) {
                dot2 = true;
            }
        }

        // Identificação do tipo de bloco
        if (dot1 && dot2) {
            brickType = 3;
        } else if (dot2 || dot1) {
            brickType = 2;
        } else {
            brickType = 1;
        }
        System.out.println("Starting box identification:" + brickType);
        // Adiciona o tipo de bloco à fila
        queue.put(brickType);
       // int i = 0;
        //i = queue.take();
        //System.out.println(i);
       //sem.release();
    }

    @Override
    public void run() {
        try {
            startBoxIdentification();  // Chama o método de identificação
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();  // Tratamento da interrupção
            System.out.println("Thread foi interrompida");
        }
    }
}
