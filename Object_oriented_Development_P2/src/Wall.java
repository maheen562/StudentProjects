import bagel.Image;

public class Wall extends GameObject{


    public final int WALL_LENGTH = 20;
    public Wall(){super();}
    public int getWALL_LENGTH(){
        return WALL_LENGTH;
    }

    public Wall(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }
}
