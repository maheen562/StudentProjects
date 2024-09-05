import java.awt.*;
import java.util.Random;

public class PinkGhost extends Ghost{
    private final int RIGHT_DIR = 1;

    private final int LEFT_DIR =2;
    private final int DOWN_DIR = 3;
    private final int UP_DIR = 4;

    public PinkGhost(){
        super();
    }
    public PinkGhost(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }

    // private Random rand = new Random();
    private int noOfDirection = 4;
    private final int PINK_GHOST_SPEED = 3;

    /*Movement Type 1 is vertical and movement type 2 is horizontal*/
    private int movementType = (int)Math.floor(Math.random() * (noOfDirection) + 1);
    private int wallDirection = 0;
    private int getWallDirection(){
        return this.wallDirection;
    }
    private void setWallDirection(int direction){
        this.wallDirection = direction;
    }
    private int oldPositionX = 0;
    private int oldPositionY = 0;

    public void changeDirection(){
        //int currentMovementType = this.movementType;

        int oldmovement = movementType;
        movementType = (int)Math.floor(Math.random() * (noOfDirection) + 1);

        while(movementType == oldmovement){

            movementType = (int)Math.floor(Math.random() * (noOfDirection) + 1);

        }
        /*if(oldmovement == UP_DIR){
            //if(movementType == LEFT_DIR || movementType == RIGHT_DIR){
                this.yPos += 3;
            //}
        }
        else if(oldmovement == DOWN_DIR){
            //if(movementType == LEFT_DIR || movementType == RIGHT_DIR){
                this.yPos -= 3;
            //}
        }
        else if(oldmovement == LEFT_DIR){
            //if(movementType == LEFT_DIR || movementType == RIGHT_DIR){
                this.xPos -= 3;
            //}
        }
        else if(oldmovement == RIGHT_DIR){
            //if(movementType == LEFT_DIR || movementType == RIGHT_DIR){
                this.xPos += 3;
            //}
        }*/



    }

    public void move(int newPosx,int newPosy){
        oldPositionX = this.xPos;
        oldPositionY = this.yPos;
        if (this.movementType == 1){
            this.xPos += PINK_GHOST_SPEED;
        }
        else if (this.movementType == 2){
            this.xPos -= PINK_GHOST_SPEED;
        }
        else if (this.movementType == 3){
            this.yPos += PINK_GHOST_SPEED;
        }
        else if (this.movementType == 4){
            this.yPos -= PINK_GHOST_SPEED;
        }
    }

    @Override
    public boolean Encounter(GameObject[] gameObjects, int noOfObjects,int objectALength,int objectBLength) {



        Rectangle rectObject = new Rectangle(this.getxPos(), this.getyPos(), objectALength, objectALength);
        int xDiff = 0;
        int yDiff = 0;
        for (int i = 0; i < noOfObjects; i++) {
            if (gameObjects[i] != null) {
                Rectangle eachObjectB = new Rectangle(gameObjects[i].getxPos(), gameObjects[i].getyPos(), objectBLength, objectBLength);
                if (Encounter(rectObject, eachObjectB)) {
                    xDiff = this.getxPos() - gameObjects[i].getxPos();
                    yDiff = this.getyPos() - gameObjects[i].getyPos();
                    this.setxPos(oldPositionX);
                    this.setyPos(oldPositionY);
                    if(xDiff> 0 && yDiff < 0) {
                        setWallDirection(DOWN_DIR);
                    }
                    else if (xDiff > 0 && yDiff >0){
                        setWallDirection(LEFT_DIR);
                    }
                    else if(xDiff <= 0){
                        setWallDirection(RIGHT_DIR);
                    } else if (yDiff >= 0) {
                        setWallDirection(UP_DIR);
                    }

                    return true;
                }
            }
        }
        return false;
    }
}
