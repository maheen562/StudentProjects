import java.util.Random;

public class GreenGhost extends Ghost{
    public GreenGhost(){
        super();
    }
    public GreenGhost(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }

    //private Random rand = new Random();
    private int noOfDirection = 2;

    int direction = 1;
    private final int GREEN_GHOST_SPEED  = 4;
    /*Movement Type 1 is vertical and movement type 2 is horizontal*/
    /*To generate random number. formula taken from https://www.educative.io/answers/how-to-generate-random-numbers-in-java*/
    private int movementType = (int)Math.floor(Math.random() * (noOfDirection) + 1);
    @Override
    public void changeDirection(){
        if (this.direction == 1){
            this.direction = 2 ;
        }
        else{
            this.direction = 1;
        }

    }
    public void move(int newPosx,int newPosy){
        if (this.direction == 1 && movementType == 1){
            this.yPos += GREEN_GHOST_SPEED;
        }
        else if (this.direction == 2 && movementType == 1){
            this.yPos -= GREEN_GHOST_SPEED;
        }
        else if (this.direction == 1 && movementType == 2){
            this.xPos += GREEN_GHOST_SPEED;
        }
        else if (this.direction == 2 && movementType == 2){
            this.xPos -= GREEN_GHOST_SPEED;
        }
    }
}