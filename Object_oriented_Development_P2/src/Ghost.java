public class Ghost extends GameObject implements Removable,Scorable{
    public Ghost(){super();}


    private int initialPositionX;/*intial position of ghost*/
    private int initialPositionY;

    /**
     * same as super.  Initial position stored to go back to after frenzy
     * */
    public Ghost(String objectName,int xPos,int yPos)
    {
        super(objectName,xPos, yPos);
        initialPositionX = this.xPos;
        initialPositionY = this.yPos;
    }

    /**
     * @return initialPositionX of Ghost
     * */
    public int getInitialPositionX() {
        return initialPositionX;
    }


    /**
     * @return initialPosition Y of Ghost
     * */
    public int getInitialPositionY() {
        return initialPositionY;
    }

    private boolean frenzy = false;

    private boolean limitreached = false;

    private int direction = 1;

    public int getDirection() {
        return direction;
    }

    public void setDirection(int direction){
        this.direction = direction;
    }
    public void changeDirection(){
        if (this.direction == 1){
            this.direction = 2 ;
        }
        else{
            this.direction = 1;
        }

    }

    //for override
    public void move(int newPosx,int newPosy){
        if (this.getDirection() == 1){
            this.xPos += 1;
        }
        else {
            this.xPos -= 1;
        }
    }

    public boolean getFrenzy(){
        return frenzy;
    }
    public void setFrenzy(boolean frenzy){
        this.frenzy = frenzy;
    }
    public boolean getlimitreached(){
        return limitreached;
    }
    public void setLimitreached(boolean limitreached){
        this.limitreached = limitreached;
    }

    public final int ADD_SCORE = 30;


    public void makeAllGhostsFrenzy(Ghost[] ghosts, int maxGhosts){
        for (int i=0;i<maxGhosts;i++){
            if(ghosts[i] != null){
                ghosts[i].setFrenzy(true);
            }
        }
    }

    public void makeAllGhostsNormal(Ghost[] ghosts, int maxGhosts){
        for (int i=0;i<maxGhosts;i++){
            if(ghosts[i] != null){
                ghosts[i].setFrenzy(false);
                ghosts[i].setxPos(ghosts[i].getInitialPositionX());
                ghosts[i].setyPos(ghosts[i].getInitialPositionY());


            }
        }
    }
    @Override
    public int AddScore() {
        this.remove();
        return ADD_SCORE;
    }

    @Override
    public void remove() {
        if(frenzy == true){
            this.setxPos(-50);
            this.setyPos(-50);
            this.setObjectName("");
        }

    }

}
