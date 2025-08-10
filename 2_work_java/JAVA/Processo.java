import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

public class Processo {
    public static AtomicBoolean close1 = new AtomicBoolean(false);
    public static AtomicBoolean close2 = new AtomicBoolean(false);

    public static void processo() throws InterruptedException {

        ArrayBlockingQueue<Integer> queue = new ArrayBlockingQueue<>(1);

        // Inicializa os mecanismos e movimentos dos cilindros
        Mechanism mechanism = new Mechanism();
        mechanism.conveyorMove();  // Este método deve mover o transportador, mas certifique-se de que ele está correto

        // Inicializa os cilindros e suas respectivas threads
        Cylinder clStart = new CylinderStart();
        CylinderThread clStartT = new CylinderThread();

        Cylinder cl1 = new Cylinder1();
        Cylinder12Thread cl12T = new Cylinder12Thread(queue, close1, close2);

        BoxIdentificationThread boxIDT = new BoxIdentificationThread(queue);
        SwitchThread switchT = new SwitchThread(close1);
        Switch2Thread switch2T = new Switch2Thread(close2);
        Switch3Thread switch3T = new Switch3Thread(close1, close2);
        EmergencyThread emergencyThread = new EmergencyThread();

        System.out.println("Paragem de emergência-> x");
        //emergencyThread.start();
        clStartT.start();
        boxIDT.start();
        cl12T.start();
        switchT.start();
        switch2T.start();
        switch3T.start();

        boxIDT.join();

        // Obtém a identificação da caixa após a execução das threads
        int boxID = boxIDT.getBoxIdentification();
        System.out.println("Result of box identification: " + boxID);

        // Exibe o resultado final com base na identificação da caixa
        switch (boxID) {
            case 1:
                System.out.println("Box 1");
                break;
            case 2:
                System.out.println("Box 2");
                break;
            case 3:
                System.out.println("Box 3");
                break;
            default:
                System.out.println("Unknown Box");
        }
    }
}