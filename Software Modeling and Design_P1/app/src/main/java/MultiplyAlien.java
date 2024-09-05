// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import java.util.Random;

public class MultiplyAlien extends Alien{

    private boolean isMultiplied = false;
    private final static double RANDOM_LIMIT = 0.05;
    private final static int INVALID_ROW = -1;

    /**
     * Creates a new instance of the MultipleAlien class with the specified parameters.
     *
     * @param imageName The name of the multiple alien's image.
     * @param type Description of the alien type (multiple in this case).
     * @param rowIndex The row of the alien.
     * @param colIndex The column of the alien.
     * @param lives The number of lives this alien has.
     */
    public MultiplyAlien (String imageName, String type, int rowIndex, int colIndex, int lives){
        super(imageName,type,rowIndex,colIndex, lives);

    }

    /**
     * This method allows for the alien to perform
     * its regular movements whilst also taking into
     * consideration the multiplying aspect.
     *
     * @return Nothing is returned.
     */
    @Override
    public void act(){
        super.act();
        if (randomizer()){
            SpaceInvader spaceInvader = (SpaceInvader) gameGrid;
            if(spaceInvader != null){
//                Get the location above the current top of the alien grid.
                int xLocationOfRowAbove = spaceInvader.RowAboveAlienGrid();
//                Check to ensure there is space to add a new row of aliens.
                if (xLocationOfRowAbove != INVALID_ROW && !isMultiplied){
//                    Add the new row of aliens to the row above the alien grid in the left most column.
//                    We also keep track of the direction the grid is moving and steps to add the new row
//                    correctly.
                    int colLag = 0;
                    int nbstepsLag = 0;

                    if(this.getDirection() == 180.0){

                    }
                    else{

                        nbstepsLag = -1;
                    }
                   // colLag = 10;

                    spaceInvader.AddAlienRowOnTop(xLocationOfRowAbove,spaceInvader.leftMostCol() + colLag,
                            this.getDirection(),getNbSteps()+nbstepsLag);
                    isMultiplied = true;
                }
            }
        }
    }

    /**
     * This method generates a random value between 0.0 and 1.0.
     * It returns true or false based on if the random value
     * generated is greater than or less than 0.05. It is used
     * to randomly activate the ability of the Multiple alien (
     * i.e., adding a new row of normal aliens).
     *
     * @return True if random value is less than 0.05.
     * @return False if random value is greater than 0.05.
     */
    private boolean randomizer() {
        Random rand = new Random();
        return rand.nextDouble() < RANDOM_LIMIT;
    }

}
