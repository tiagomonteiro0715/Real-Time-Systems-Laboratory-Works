public class CylinderThread extends Thread {

    private CylinderStart cylinder = new CylinderStart();

    public void initializeCylinderThread() throws InterruptedException {
            this.cylinder.gotoPosition(1);
            Thread.sleep(1000);
            this.cylinder.gotoPosition(0);
            Thread.sleep(500);

    }

    public void run() {
        try{
            this.initializeCylinderThread();
        }catch(InterruptedException e){
            throw new RuntimeException(e);
        }
    }

}
