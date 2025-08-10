import java.util.concurrent.atomic.AtomicBoolean;

public class Switch2Thread extends Thread {
    private final Mechanism mechanism = new Mechanism();
    private final AtomicBoolean close2; // Estado da doca
    private static final int DEBOUNCE_DELAY_MS = 5000; // Intervalo de debounce para clique único (500ms)

    public Switch2Thread(AtomicBoolean close2) {
        this.close2 = close2;
    }

    public void switchLoop() throws InterruptedException {
        boolean currentState = close2.get();

        while (true) {
            if(mechanism.switchDock2Pressed()){
                if(currentState == false){
                    close2.set(true);
                    System.out.println("Close Dock2");
                    break;
                }else
                    close2.set(false);
                System.out.println("Open Dock2");
                break;
            }
        }
        //close1.set(currentState);
        Thread.sleep(DEBOUNCE_DELAY_MS);
    }

    @Override
    public void run() {
        try {
            this.switchLoop(); // Inicia o loop de monitoramento
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Thread foi interrompida");
        }
    }
}