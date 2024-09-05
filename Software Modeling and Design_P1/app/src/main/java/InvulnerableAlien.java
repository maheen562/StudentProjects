// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

public class InvulnerableAlien extends Alien
{
    private boolean isvulnerable = false;
    private final int TRASPARENT_TIME = 4;

    private int transparentStepCount = 0;

    /**
     * Creates a new instance of the InvulernableAlien class with the specified parameters.
     *
     * @param imageName The name of the invulnerable alien's image.
     * @param type Description of the alien type (invulernable in this case).
     * @param rowIndex The row of the alien.
     * @param colIndex The column of the alien.
     * @param lives The number of lives this alien has.
     */
    public InvulnerableAlien (String imageName, String type, int rowIndex, int colIndex, int lives){
        super(imageName,type,rowIndex,colIndex, lives);
    }

    /**
     * This method handles the periods of time when the alien
     * is invulnerable and when it is not. It alters the type of the
     * alien to reflect this change. It uses the transparent time
     * to keep track of when the alien needs to become invulnerable,
     * and then makes this change. Nothing is returned.
     *
     * @return Nothing is returned.
     */
    public void handleTransparent(){
        if (!isvulnerable){
//          Make the alien invulnerable.
            transparentStepCount = this.getNbSteps();
            this.setType("invulnerable-Active");
            isvulnerable = true;
        }
        else{
//            If it has been invulnerable for long enough, make it vulnerable.
            if((transparentStepCount+TRASPARENT_TIME) >= this.getNbSteps()){
                isvulnerable = false;
                transparentStepCount = 0;
                this.setType("invulnerable");

            }
        }
    }

    /**
     * This method allows for the alien to perform
     * its regular movements whilst also taking into
     * consideration the invulnerability aspect.
     *
     * @return Nothing is returned.
     */
    @Override
    public void act(){
        super.act();
        handleTransparent();
    }

    /**
     * This method decreases the life of the alien if it has been
     * hit during a period where it is vulnerable.
     *
     * @return Nothing is returned.
     */
    @Override
    public void decreaseLives(){
//        The alien's invulnerability is not active, so it sustains damage.
        if(this.getType().equals("invulnerable")){
            this.setLives(this.getLives() - 1);
        }

    }



}
