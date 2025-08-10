import java.util.concurrent.atomic.AtomicBoolean;

public class Switch3Thread extends Thread {
    private final Mechanism mechanism = new Mechanism();
    private final AtomicBoolean close1;// Estado da doca
    private final AtomicBoolean close2;
  //  private static final int DEBOUNCE_DELAY_MS = 5000; // Intervalo de debounce para clique único (500ms)

    public Switch3Thread(AtomicBoolean close1, AtomicBoolean close2) {
        this.close1 = close1;
        this.close2 = close2;
    }

    public void switchLoop() throws InterruptedException {

        while (true) {
            if(mechanism.switchDockEndPressed()){
               close1.set(true);
               close2.set(true);
               System.out.println("Switch dock end pressed(Dock1 and Dock1 is closed for 10 seconds)");
               Thread.sleep(10000);
               close1.set(false);
               close2.set(false);
               break;
            }
        }
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
