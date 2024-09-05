public class Coordinates {
    private final static int WINDOW_WIDTH = 1024;
    private final static int WINDOW_HEIGHT = 768;
    public final int PLAYER_DIAMETER = 20;

    private int x = 0;
    private int y = 0;

    public Coordinates(){
        x = 0;
        y = 0;
    }

    public Coordinates[] fillArray(Coordinates[] arr, int size){
        x = 0;
        y = 0;
        for (int i=0;i<size;i++){
            arr[i] = new Coordinates();
        }
        return arr;
    }

    public Coordinates(int x, int y){
        this.x = x;
        this.y = y;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setCoordinates(int x,int y){
        this.x = x;
        this.y = y;
    }

    public boolean isValid(int newX, int newY){
        if(newX < 0 || newY < 0 || newX > (WINDOW_WIDTH-PLAYER_DIAMETER)
                || newY > (WINDOW_HEIGHT - PLAYER_DIAMETER)){
            return false;
        }
        return true;
    }
}



