// Alien.java
// Used for SpaceInvader
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jgamegrid.*;

import java.util.List;

public class Alien extends Actor
{
  private final int maxNbSteps = 16;
  private int nbSteps;
  private static int speed = 1;
  private boolean isMoving = true;
  private boolean isAutoTesting;
  private List<String> movements;
  private int movementIndex = 0;
  private String type;
  private int rowIndex;
  private int colIndex;
  private int lives;
  private  boolean speedincreased = false;
  private  int remainingNbSteps = 0;
  private final static int SLOW_DOWN = 7;
  private final static int LIFE_DECREASE = 1;
  private final static int RESET = 0;

  /**
   * Creates a new instance of the Alien class with the specified parameters.
   *
   * @param imageName The name of the alien's image.
   * @param type Description of the alien type.
   * @param rowIndex The row of the alien.
   * @param colIndex The column of the alien.
   * @param lives The number of lives this alien has.
   */

  public Alien(String imageName, String type, int rowIndex, int colIndex, int lives)
  {

    super(imageName);
    setSlowDown(SLOW_DOWN);
    this.type = type;
    this.rowIndex = rowIndex;
    this.colIndex = colIndex;
    this.lives = lives;
  }

  /**
   * This method handles increases the speed of an alien
   * in the plus version of the game whilst keeping
   * track of how many steps the alien now has to travel.
   *
   * @return Nothing is returned.
   */
  public void increaseSpeed(/*int newSpeed*/){
    speed++;

  }

  /*calculates remaining steps for all aliens*/
  public void CalculateRemainingSteps(){
    remainingNbSteps = ((((maxNbSteps*(speed-1))-nbSteps)/(speed-1))*(speed)) + nbSteps;

    speedincreased = true;
  }
public void setSpeed(int speed){
    this.speed=speed;
}

  public String getType() {
    return type;
  }

  public int getRowIndex() {
    return rowIndex;
  }

  public int getColIndex() {
    return colIndex;
  }

  public void reset()
  {
    nbSteps = SLOW_DOWN;
  }

  /**
   * Gets the value of the number of steps the alien has moved.
   *
   * @return The current value of 'nbSteps'.
   */
  public int getNbSteps()
  {
      return nbSteps;
  }

  /**
   * Sets the value of the number of steps moved.
   * @param nbSteps This is the number of steps moved.
   *
   * @return Nothing is returned.
   */
  public void setNbSteps(int nbSteps){
    this.nbSteps = nbSteps;
  }

  /**
   * Sets the value of the number of lives that the alien has.
   * @param lives This is the number of lives the alien has.
   *
   * @return Nothing is returned.
   */
  public void setLives(int lives) {
    this.lives = lives;
  }

  /**
   * Sets the type of the alien, primarily used for invulnerable alien.
   * @param newType This is the alien's new type.
   *
   * @return Nothing is returned.
   */
  public void setType(String newType){
    type = newType;
  }

  public void setTestingConditions(boolean isAutoTesting, List<String> movements) {
    this.isAutoTesting = isAutoTesting;
    this.movements = movements;
  }

  public int getSpeed() {
    return speed;
  }
  public int getRemainingNbSteps(){
    return remainingNbSteps;
  }

  /**
   * Decreases the value of the alien's lives by 1.
   *
   * @return Nothing is returned.
   */
  public void decreaseLives(){
    this.lives -= LIFE_DECREASE;
  }

  private void checkMovements() {
    if (isAutoTesting) {
      if (movements != null && movementIndex < movements.size()) {
        String movement = movements.get(movementIndex);
        if (movement.equals("S")) {
          isMoving = false;
        } else if (movement.equals("M")) {
          isMoving = true;
        }
        movementIndex++;
      }
    }
  }

  /**
   * This method allows for the alien to perform
   * its regular movements whilst also taking into
   * consideration the changes that come about when
   * speed is increased in the plus version of the
   * game.
   *
   * @return Nothing is returned.
   */
  public void act()
  {
    checkMovements();
    if (!isMoving) {
      return;
    }
//    If speed has increased and the alien has not hit the boundary then we move it forward.
    if(speedincreased){
      if(nbSteps < remainingNbSteps){
        move();
        nbSteps+=speed;
      }
      else {
//        The alien cannot move forward as it will hit the boundary so we reset change
//        direction and move it down.
        speedincreased = false;

        nbSteps = RESET;
        remainingNbSteps = RESET;

//      Changing the alien's direction.
        int angle;
        if (getDirection() == RESET)
          angle = 90;
        else
          angle = -90;
        turn(angle);
        move();
        turn(angle);

      }
    }

//    The alien's speed increased midway, so we increase how many steps it has to move.
    else if (nbSteps < (maxNbSteps*speed))
    {
//      If the alien can still move, we do so.
      move();
      nbSteps+=speed;
    }
    else
    {
//      The alien cannot move forward so we move it down and change directions.
      nbSteps = RESET;

      int angle;
      if (getDirection() == RESET)
        angle = 90;
      else
        angle = -90;
      turn(angle);
      move();
      turn(angle);
    }

//    Remove the alien once it hits the bottom of the screen.
    if (getLocation().y > 90)
      removeSelf();

  }


  /**
   * Gets the value of the number of lives the alien has.
   *
   * @return The number of lives the alien has.
   */
  public int getLives() {
    return this.lives;
  }

}
