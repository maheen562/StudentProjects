import bagel.DrawOptions;
import bagel.Image;

public class PacManCharacter extends GameObject implements Movable{
    public final int PLAYER_DIAMETER = 20;

    public PacManCharacter(){
        super();
    }
    public PacManCharacter(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }



    @Override
    public void move(int newPosx, int newPosy) {

        this.setxPos(newPosx);
        this.setyPos(newPosy);
    }


}
