// SpaceInvader.java
// Sprite images from http://www.cokeandcode.com/tutorials
// Nice example how the different actor classes: SpaceShip, Bomb, SpaceInvader, Explosion
// act almost independently of each other. This decoupling simplifies the logic of the application

// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jgamegrid.*;

import java.awt.Point;
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.util.*;

public class SpaceInvader extends GameGrid implements GGKeyListener
{
  private int nbRows = 3;
  private int nbCols = 3;
  private final int MAX_NB_ROWS = 11;
  private final int MAX_NB_COLS = 11;
  private boolean isGameOver = false;
  private boolean isAutoTesting = false;
  private Properties properties = null;
  private boolean speedincreased = false;
  private StringBuilder logResult = new StringBuilder();
  private ArrayList<Point> powerfulAlienLocations = new ArrayList<Point>();
  private ArrayList<Point> invulnerableAlienLocations = new ArrayList<Point>();
  private ArrayList<Point> multipleAlienLocations = new ArrayList<Point>();
  private Alien[][] alienGrid = null;
  private SpaceShip ss = new SpaceShip(this);
  private int previousIncrease = 0;
  private int speed = 1;
  private final static int INVALID = -1;
  private final static int COL_SPACE = 10;
  private final static int DEAD_ALIEN = 0;

  public final String NORMAL_ALIEN_IMAGE_NAME = "sprites/alien.gif";
  public final String POWERFUL_ALIEN_IMAGE_NAME = "sprites/powerful_alien.gif";

  public final String MULTIPLE_ALIEN_IMAGE_NAME = "sprites/multiple_alien.gif";
  public final String INVULNERABLE_ALIEN_IMAGE_NAME =  "sprites/invulnerable_alien.gif";

  public final String NORMAL_ALIEN_TYPE_STR = "alien";

  public final String POWERFUL_ALIEN_TYPE_STR = "powerful";
  public final String MULTIPLE_ALIEN_TYPE_STR = "multiple";
  public final String INVULNERABLE_ALIEN_TYPE_STR = "invulnerable";

  public final String INVULNERABLE_ACTIVE_ALIEN_TYPE_STR = "invulnerable-Active";
  public final int FIRST_ROW_Y_LOCATION = 10;

  public final int NORMAL_ALIEN_LIVES = 1;

  public final int POWERFUL_ALIEN_LIVES = 5;

  public final int MAX_TOP_ROW_LOCATION = 100;


  List<String> movements = null;


  public SpaceInvader(Properties properties) {
    super(200, 100, 5, false);
    this.properties = properties;
  }

  private ArrayList<Point> convertFromProperty(String propertyName) {
    String powerfulAlienString = properties.getProperty(propertyName);
    ArrayList<Point> alienLocations = new ArrayList<>();
    if (powerfulAlienString != null) {
      String [] locations = powerfulAlienString.split(";");
      for (String location: locations) {
        String [] locationPair = location.split("-");
        int rowIndex = Integer.parseInt(locationPair[0]);
        int colIndex = Integer.parseInt(locationPair[1]);
        alienLocations.add(new Point(rowIndex, colIndex));
      }
    }
    return alienLocations;
  }

  private void setupAlienLocations() {
    powerfulAlienLocations = convertFromProperty("Powerful.locations");
    invulnerableAlienLocations = convertFromProperty("Invulnerable.locations");
    multipleAlienLocations = convertFromProperty("Multiple.locations");
  }

  private boolean arrayContains(ArrayList<Point>locations, int rowIndex, int colIndex) {
    for (Point location : locations) {
      if (location.getX() == rowIndex && location.getY() == colIndex) {
        return true;
      }
    }
    return false;
  }

  private void setupAliens() {
    setupAlienLocations();
    isAutoTesting = Boolean.parseBoolean(properties.getProperty("isAuto"));
    String aliensControl = properties.getProperty("aliens.control");
    if (aliensControl != null) {
      movements = Arrays.asList(aliensControl.split(";"));
    }
    alienGrid = new Alien[MAX_NB_ROWS][MAX_NB_COLS];
    for (int i = 0; i < nbRows; i++) {
      for (int k = 0; k < nbCols; k++) {
        Alien alien = null;
//        Setting up the different kinds of aliens, useful for the extended version of the game.
        if (arrayContains(powerfulAlienLocations, i, k)) {

          alien = new PowerfulAlien(POWERFUL_ALIEN_IMAGE_NAME, POWERFUL_ALIEN_TYPE_STR,
                    i, k, POWERFUL_ALIEN_LIVES);

        } else if (arrayContains(invulnerableAlienLocations, i, k)) {

          alien = new InvulnerableAlien(INVULNERABLE_ALIEN_IMAGE_NAME, INVULNERABLE_ALIEN_TYPE_STR,
                  i, k, NORMAL_ALIEN_LIVES);

        } else if (arrayContains(multipleAlienLocations, i, k)) {

          alien = new MultiplyAlien(MULTIPLE_ALIEN_IMAGE_NAME, MULTIPLE_ALIEN_TYPE_STR,
                  i, k, NORMAL_ALIEN_LIVES);

        } else {

          alien = new Alien(NORMAL_ALIEN_IMAGE_NAME, NORMAL_ALIEN_TYPE_STR, i, k, NORMAL_ALIEN_LIVES);

        }
        addActor(alien, new Location(100 - 5 * nbCols + 10 * k, 10 + 10 * i));
        alien.setTestingConditions(isAutoTesting, movements);
        alienGrid[i][k] = alien;

      }
    }

  }

  private void setupSpaceShip() {
    addActor(ss, new Location(100, 90));
    String spaceShipControl = properties.getProperty("space_craft.control");
    List<String> controls = null;
    if (spaceShipControl != null) {
      controls = Arrays.asList(spaceShipControl.split(";"));
    }
    ss.setTestingConditions(isAutoTesting, controls);
    addKeyListener(ss);
  }


  public String runApp(boolean isDisplayingUI) {
    setSimulationPeriod(Integer.parseInt(properties.getProperty("simulationPeriod")));
    nbRows = Integer.parseInt(properties.getProperty("rows"));
    nbCols = Integer.parseInt(properties.getProperty("cols"));
    setupAliens();
    setupSpaceShip();
    addKeyListener(this);
    getBg().setFont(new Font("SansSerif", Font.PLAIN, 12));
    getBg().drawText("Use <- -> to move, spacebar to shoot", new Point(400, 330));
    getBg().drawText("Press any key to start...", new Point(400, 350));

    if (isDisplayingUI) {
      show();
    }

    if (isAutoTesting) {
      setBgColor(java.awt.Color.black);  // Erase text
      doRun();
    }

    while(!isGameOver) {
      try {
        Thread.sleep(1000);
      } catch (InterruptedException e) {
        throw new RuntimeException(e);
      }
    }

    doPause();
    return logResult.toString();
  }

  /**
   * This method finds the top most row
   * and returns its index. This is used when
   * multiple alien adds its row of normal aliens.
   *
   * @return the index of the top most column.
   */
  public int RowAboveAlienGrid(){
    int topRowLocation = MAX_TOP_ROW_LOCATION;

    for (int i = 0; i < nbRows; i++) {
      for (int j = 0; j < nbCols; j++) {
//        If the alien grid hasn't been removed we find the top most row.
        if(!alienGrid[i][j].isRemoved()){
          if(topRowLocation > alienGrid[i][j].getLocation().y){
            topRowLocation = alienGrid[i][j].getLocation().y;

          }
        }
      }
    }
//    If the top most row is not empty (it isn't in the game screen yet), it is invalid.
//    So a new row of aliens cannot be added yet.
    if(topRowLocation <= FIRST_ROW_Y_LOCATION){
      return INVALID;
    }
//  There is space for a new row of aliens, and the index to that row is returned.
    return topRowLocation - FIRST_ROW_Y_LOCATION;
  }

  /**
   * This method finds the left most column
   * and returns its index. This is used when
   * multiple alien adds its row of normal aliens.
   *
   * @return the index of the left most column.
   */
  public int leftMostCol(){
    int leftCol = 1000;
    for (int i = 0; i < nbRows; i++) {
      for (int j = 0; j < nbCols; j++) {
//        If this row and column has not been removed then we check to see
//        if our current left most column is actually left most. We update it, if it isn't.
        if(alienGrid[i][j] !=  null && !alienGrid[i][j].isRemoved()){
          if(leftCol > alienGrid[i][j].getLocation().x){
            leftCol = alienGrid[i][j].getLocation().x;
          }
        }

      }
    }
    return leftCol;
  }

  /**
   * This method adds a row of normal aliens to the game
   * when a multiple alien is active in the plus version of
   * the game.
   *
   * @param yLocation This is the y location of the alien we want to add on top of.
   * @param xLocation This is the x location of the alien we want to add on top of.
   * @param direction This is the direction in which the existing aliens are moving.
   * @param nbSteps This is the number of steps moved b
   *
   * @return Nothing is returned.
   */
  public void AddAlienRowOnTop(int yLocation,int xLocation,double direction,int nbSteps){

    nbRows++;

    //shift all the indexes down, so we can add on top of the grid.
    for (int i = 0; i < (nbRows-1); i++) {
      for(int j = 0 ; j<nbCols;j++){
        alienGrid[i+1][j] = alienGrid[i][j];
      }
    }

    Alien alien;
    int addedColLocation = 0;
//    add in the new aliens to the grid.
    for(int j=0;j<nbCols;j++){
      alien = new Alien(NORMAL_ALIEN_IMAGE_NAME, NORMAL_ALIEN_TYPE_STR,
                                  0, j, NORMAL_ALIEN_LIVES);

      addActor(alien, new Location(xLocation + addedColLocation , yLocation ));

      alien.setTestingConditions(isAutoTesting, movements);

//      Setting the location, direction, and nbSteps of the new alien.
      alienGrid[0][j] = alien;
      alienGrid[0][j].setNbSteps(nbSteps);
      alienGrid[0][j].setDirection(direction);
      //alienGrid[0][j].setSpeed(speed);

//     Ensuring there is space in the new column of alien.
      addedColLocation += COL_SPACE;
    }

  }




  @Override
  public void act() {
    logResult.append("Alien locations: ");
    for (int i = 0; i < nbRows; i++) {
      for (int j = 0; j < nbCols; j++) {
        Alien alienData = alienGrid[i][j];

        String isDeadStatus = alienData.isRemoved() ? "Dead" : "Alive";
        String gridLocation = "0-0";
        if (!alienData.isRemoved()) {
          gridLocation = alienData.getX() + "-" + alienData.getY();
        }

        String alienDataString = String.format("%s@%d-%d@%s@%s#", alienData.getType(),
                alienData.getRowIndex(), alienData.getColIndex(), isDeadStatus, gridLocation);
        logResult.append(alienDataString);
      }
    }
//    If the game version is plus, then we can increase speed once a shots threshold is reached.
   String version = properties.getProperty("version");
    if(version.equals("plus")) {
      // increase speed when needed
      increaseAllAliensSpeed();
    }
    logResult.append("\n");

    }

  /*increases Alien speed*/
  private void increaseAllAliensSpeed(){

    Alien alien = AlienfirstNotRemovedInstance();
    if(alien != null && !alien.isRemoved()){

      if (((ss.getNbShots() >= 10 && alien.getSpeed() <= 1/*speed == 2*/) || (ss.getNbShots() >= 50 && alien.getSpeed() <= 2/*speed == 3*/) ||
              (ss.getNbShots() >= 100 && alien.getSpeed() <= 3/*speed == 4*/) || (ss.getNbShots() >= 500 && alien.getSpeed() <= 4/*speed == 5*/)) && !speedincreased){
        alien.increaseSpeed();
        speedincreased = true;

        for (int i = 0; i < nbRows; i++) {
          for (int j = 0; j < nbCols; j++) {
            if(!alienGrid[i][j].isRemoved() && alienGrid[i][j]!=null ){
              alienGrid[i][j].CalculateRemainingSteps();
            }
          }
        }
        logResult.append("\n");
        notifyAliensMoveFast();

      } else if (ss.getNbShots() > previousIncrease) {
        speedincreased = false;
      }



    }

  }

  /*accesses first instance of alien removed*/
  private Alien AlienfirstNotRemovedInstance(){
    for (int i = 0; i < nbRows; i++) {
      for (int j = 0; j < nbCols; j++) {
        if(!alienGrid[i][j].isRemoved() && alienGrid[i][j]!=null ){
          return alienGrid[i][j];
        }
      }
    }
    return null;
  }
  public void notifyAliensMoveFast() {
      logResult.append("Aliens start moving fast");
  }


  public boolean notifyAlienHit(List<Actor> actors) {
    for (Actor actor: actors) {
      Alien alien = (Alien)actor;
      String alienData = String.format("%s@%d-%d",
              alien.getType(), alien.getRowIndex(), alien.getColIndex());
      logResult.append("An alien has been hit.");
      logResult.append(alienData + "\n");
//      If the alien has lives to decrease, then we decrease the life.
      if(alien.getLives() > DEAD_ALIEN){
        alien.decreaseLives();
        if(alien.getLives() == DEAD_ALIEN) {
            return true;
        }
      }
    }
    return false;
  }


  /**
   * This method checks to see if the alien is invulnerable.
   * @param actors This is the list of actors in our game.
   *
   * @return True if the alien is invulnerable, False otherwise.
   */
  public boolean isAlienInvulnerable(List<Actor> actors){
    for (Actor actor: actors) {
      Alien alien = (Alien)actor;
      if(alien.getType().equals(INVULNERABLE_ACTIVE_ALIEN_TYPE_STR)){
        return true;
      }
    }
    return false;
  }

  public void setIsGameOver(boolean isOver) {
    isGameOver = isOver;
  }

  public boolean keyPressed(KeyEvent evt)
  {
    if (!isRunning())
    {
      setBgColor(java.awt.Color.black);  // Erase text
      doRun();
    }
    return false;  // Do not consume key
  }

  public boolean keyReleased(KeyEvent evt)
  {
    return false;
  }


}
