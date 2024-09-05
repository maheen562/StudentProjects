public class BlueGhost extends Ghost implements Movable{
    public BlueGhost(){
        super();
    }
    public BlueGhost(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }

    private int direction = 1;
    public void changeDirection(){
        if (this.direction == 1){
            this.direction =2;
        }
        else{
            this.direction = 1;
        }

    }
    private final int BLUE_GHOST_SPEED = 2;
    public void move(int newPosx,int newPosy){
        if (this.direction == 1){
            this.yPos += BLUE_GHOST_SPEED;
        }
        else {
            this.yPos -= BLUE_GHOST_SPEED;
        }
    }

}