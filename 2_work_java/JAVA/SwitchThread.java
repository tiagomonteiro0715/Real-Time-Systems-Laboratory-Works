import java.util.concurrent.atomic.AtomicBoolean;

public class SwitchThread extends Thread {
    private final Mechanism mechanism = new Mechanism();
    private final AtomicBoolean close1; // Estado da doca
    private static final int DEBOUNCE_DELAY_MS = 5000; // Intervalo de debounce para clique único (500ms)

    public SwitchThread(AtomicBoolean close1) {
        this.close1 = close1;
    }

    public void switchLoop() throws InterruptedException {
        boolean currentState = close1.get();

        while (true) {
          if(mechanism.switchDock1Pressed()){
                if(currentState == false){
                    close1.set(true);
                    System.out.println("Close Dock1");
                    break;
                }else
                    close1.set(false);
                    System.out.println("Open Dock1");
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