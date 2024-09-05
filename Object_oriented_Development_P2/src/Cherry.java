public class Cherry extends GameObject implements Removable,Scorable{

    public Cherry(){super();}

    public Cherry(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
    }
    public final int ADD_SCORE = 20;


    @Override
    public int AddScore() {
        this.remove();
        return ADD_SCORE;
    }

    @Override
    public void remove() {

        this.setxPos(-200);
        this.setyPos(-200);
        this.setObjectName("");
    }

}
