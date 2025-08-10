public interface Cylinder {

    public void moveForward();
    public void moveBackward();
    public void stop();
    public int getPosition(); // returns 0,1 or -1
    public void gotoPosition(int position);
    public void calibration();
    public boolean boxDetected();

}
