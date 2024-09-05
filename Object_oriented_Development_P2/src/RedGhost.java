public class RedGhost extends Ghost implements Movable{
    public RedGhost(){
        super();
    }
    public RedGhost(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }

    private int direction = 1;
    public void changeDirection(){
        if (this.direction == 1){
            this.direction = 2;
        }
        else{
            this.direction = 1;
        }

    }

    private final int RED_GHOST_SPEED = 1;
    @Override
    public void move(int newPosx,int newPosy){
        if (this.direction == 1){
            this.xPos += RED_GHOST_SPEED;
        }
        else {
            this.xPos -= RED_GHOST_SPEED;
        }
    }

}
