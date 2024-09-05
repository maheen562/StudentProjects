import java.awt.*;

public class Dot extends GameObject implements Removable,Scorable{
    public final int ADD_SCORE = 10;
    public Dot(){super();}
    public Dot(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }




    @Override
    public int AddScore() {
        this.remove();
        return ADD_SCORE;
    }

    @Override
    public void remove() {
        this.setxPos(-100);
        this.setyPos(-100);
        this.setObjectName("");
    }
}