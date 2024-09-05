import bagel.*;

import java.io.BufferedReader;
import java.io.FileReader;

/**
 * Parts of this code have been taken from SWEN20003 Project 1, Semester 1, 2023 that was written by Maheen Abdul Khaliq
 * Khan,1193813
 *
 *
 * Maheen Abdul Khaliq Khan,1193813
 * April 2023
 */
public class ShadowPac extends AbstractGame  {
    public final static int WINDOW_WIDTH = 1024;

    public final static int WINDOW_HEIGHT = 768;
    public final static int MAX_GAME_OBJECTS = 271;
    public final static int DEFAULT_SPEED = 3;

    private final Image BACKGROUND_IMAGE = new Image("res/background0.png");

    /*Pixel length sizes of obstacles*/
    private final static int FIRST_HEART_POSITION_X = 900;
    private final static int FIRST_HEART_POSITION_Y = 10;

    public final static int NO_OF_GHOSTS = 10; /*Total of each game components,made public as they are access in other
                                                            classes*/
    public final static int NO_OF_WALLS = MAX_GAME_OBJECTS;

    public final static int NO_OF_DOTS = MAX_GAME_OBJECTS;

    public final static int NO_OF_HEARTS = 3;
    private final static int LEVEL0_SCORE = 1210; /*final score of each level*/
    private final static int LEVEL1_SCORE = 800;
    private static final int LEVEL_COMPLETE_MESSAGE_RENDER_TIME = 300;
    private final static int WALL_LENGTH = 50;
    private final static int GAME_OBJECT_LENGTH = 25;
    private static final int FRENZY_TIME = 1000;
    private static final int FRENZY_INCREASE_IN_SPEED = 3;

    private final static String GAME_TITLE = "SHADOW PAC"; /*Game Title*/


    private boolean gameWon = false; /*Check if game Won*/
    private boolean gameLost = false;/*Check if game lost*/

    private final static String LEVEL1_CSV = "res/level1.csv"; /*file names for both levels*/
    private final static String LEVEL0_CSV = "res/level0.csv";


    private static boolean spacePressed = false; /*Check if space pressed to start level*/
    private static boolean level0Complete = false;/*Check if level 0 complete*/

    private static boolean csvRead = false; /*Check if space pressed to start game*/
    private static boolean newlevel = true; /*Check if space pressed to start game*/


    public ShadowPac(){
        super(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE);
    }
    private int currPosX = 0; /*current and initial positions of x*/
    private int currPosY = 0;
    private int initPosX = 0;
    private int initPosY = 0;
    private boolean showLevelCompleteRender = false;
    private int rendertimeframe = 0;
    private static int lives = NO_OF_HEARTS;/*The current lives of the player*/
    private int score = 0; /*Score of the player*/
    private int frenzyTimer = 0;/*Timer for frenzyMode*/
    private int speed = DEFAULT_SPEED;
    double directionDegree = 0; /*Check direction of move made*/
    private final static double RIGHT_ROTATION = 0.0; /*PacMan rotation with direction*/
    private final static double LEFT_ROTATION = Math.PI;
    private final static double UP_ROTATION = (3.0*Math.PI)/2.0;
    private final static double DOWN_ROTATION = (Math.PI)/2.0;
    private Ghost[] ghosts = new Ghost[NO_OF_GHOSTS];/*The Objects in the game*/
    private PinkGhost pinkGhost = new PinkGhost();
    private BlueGhost blueGhost = new BlueGhost();
    private RedGhost redGhost = new RedGhost();
    private GreenGhost greenGhost = new GreenGhost();
    private Dot[] dots = new Dot[NO_OF_DOTS];
    private Wall[] walls = new Wall[NO_OF_WALLS];
    private GameObject[] hearts = new GameObject[NO_OF_HEARTS];
    private PacManCharacter player = new PacManCharacter();
    private Cherry cherry = new Cherry();
    private Pellet pellet = new Pellet();
    private DisplayObject gameDisplay = new DisplayObject(); /*Class used to display game components*/

    private final String PLAYER_STR = "Player";/*String of objects used to read from csv*/
    private final String WALL_STR = "Wall";
    private final String DOT_STR = "Dot";
    private final String GHOST_STR = "Ghost";

    private final String PINK_GHOST_STR = "GhostPink";
    private final String RED_GHOST_STR = "GhostRed";
    private final String GREEN_GHOST_STR = "GhostGreen";
    private final String BLUE_GHOST_STR = "GhostBlue";


    private final String HEART_STR = "Heart";
    private final String PELLET_STR = "Pellet";
    private final String CHERRY_STR = "Cherry";
    public static boolean isLevel0Complete() {
        return level0Complete;
    }

    public static int getLives() {
        return lives;
    }
    private static String csvFile = null; /*string for csvFile*/
    /**
     * This is the main method that is used to run the ShadowPac game. It first reads int the csv for level 0, and them
     * runs the game.
     */
    public static void main(String[] args) {
        csvFile = LEVEL0_CSV;
        ShadowPac game = new ShadowPac();
        game.run();
    }

    /**
     * Performs a state update.
     * Allows the game to exit when the escape key is pressed.
     * It has a frame rate of 60 frames per minute. In each state, It first checks if we are at the beginning
     * of a level or end of the level, or if we have won or lost the game. If so, it displays the appropraite message.
     * In the case that we are in the new level, it reads in the csv, else it takes in the direction the player has
     * moved in, plays the level and draws the game at that frame.
     */
    @Override
    protected void update(Input input) {

        if (input.wasPressed(Keys.ESCAPE)){
            Window.close();
        }

        if(input.wasPressed(Keys.SPACE)){
            spacePressed = true;
        }
        BACKGROUND_IMAGE.draw(Window.getWidth()/2.0, Window.getHeight()/2.0);


        if (showLevelCompleteRender) {
            rendertimeframe++;
            gameDisplay.LevelCompleteMessage();
            if (rendertimeframe == LEVEL_COMPLETE_MESSAGE_RENDER_TIME) {
                showLevelCompleteRender = false;
            }
        }
        else if (!(spacePressed) && !(showLevelCompleteRender)) {

            if(!(level0Complete)){
                gameDisplay.showIntroMessageLevel0();
            }
            else {
                gameDisplay.showIntroMessageLevel1();

            }
        }

        else {
            if (newlevel) {
                if(level0Complete == true){
                    csvFile = LEVEL1_CSV;
                }else{
                    AddHeartCoordinates();
                }
                readCSV();
                csvRead = true;
                newlevel = false;
            }
            if (!gameWon && !gameLost && !showLevelCompleteRender) {
                if (input.isDown(Keys.LEFT)) {
                    currPosX -= speed;
                    directionDegree = LEFT_ROTATION;
                }
                if (input.isDown(Keys.RIGHT)) {
                    currPosX += speed;
                    directionDegree = RIGHT_ROTATION;
                }
                if (input.isDown(Keys.UP)) {
                    currPosY -= speed;
                    directionDegree = UP_ROTATION;
                }
                if (input.isDown(Keys.DOWN)) {
                    currPosY += speed;
                    directionDegree = DOWN_ROTATION;
                }

                PlayLevel();
                gameDisplay.drawGame(walls,dots,hearts,ghosts,pinkGhost,player,blueGhost,redGhost,greenGhost,pellet,
                        cherry,directionDegree,score);

                if (lives == 0) {
                    gameLost = true;
                }
                else if(score == LEVEL0_SCORE && level0Complete == false){
                    level0Complete = true;
                    newlevel = true;
                    spacePressed = false;
                    showLevelCompleteRender = true;

                } else if (score == LEVEL1_SCORE && level0Complete == true) {
                    gameWon = true;
                }
            } else if (gameWon) {
                gameDisplay.showGameWonMessage();
            } else if (gameLost) {
                gameDisplay.showGameLostMessage();
            }

        }



    }

    /**
     * The Play Level function plays the level. It moves the PacMacCharacter and checks to see if there is any encounter
     * with the rest of the objects. If it encounters a wall then it does not move there. It is encounters a Dot,Cherry,
     * Pellet or Ghost, it does the action accordingly. For instance, if it encounters dot, it will increase score. If
     * it encounters a Ghost which is not frenzy, it will lose a life and regenerate in its start position. It is called
     * once in each update, so for level 1, it also calls the moveGhosts function which moves the ghosts. It is used to
     * play both levels.
     * */
    private void PlayLevel(){
        if (player.isValidMove(currPosX, currPosY, walls, NO_OF_WALLS, GAME_OBJECT_LENGTH, WALL_LENGTH)) {
            player.move(currPosX, currPosY);
        } else {
            currPosX = player.getxPos();
            currPosY = player.getyPos();
        }
        for (int i = 0; i < NO_OF_DOTS; i++) {
            if(dots[i]!=null){
                if (player.Encounter(dots[i], GAME_OBJECT_LENGTH,GAME_OBJECT_LENGTH)) {
                    score += dots[i].AddScore();
                }
            }
        }
            for(int i=0;i<NO_OF_GHOSTS;i++){

                if(ghosts[i] != null) {
                    if (player.Encounter(ghosts[i], GAME_OBJECT_LENGTH,GAME_OBJECT_LENGTH)) {
                        if (ghosts[i].getFrenzy() == false) {
                            player.move(initPosX, initPosY);
                            currPosX = initPosX;
                            currPosY = initPosY;
                            lives--;

                        } else {
                            ghosts[i].remove();
                        }
                    }
                }
                if(ghosts[i] == null){
                    break;
                }

            }
            if(level0Complete == true){
                MoveGhosts();
            }

        if (player.Encounter(pellet,GAME_OBJECT_LENGTH,GAME_OBJECT_LENGTH)) {
            pellet.remove();
            ghosts[0].makeAllGhostsFrenzy(ghosts,NO_OF_GHOSTS);
            speed += FRENZY_INCREASE_IN_SPEED;
        }else if (player.Encounter(cherry,GAME_OBJECT_LENGTH,GAME_OBJECT_LENGTH)) {
            cherry.AddScore();
        }
        if(ghosts[0].getFrenzy() == true){
            frenzyTimer++;
            if(frenzyTimer == FRENZY_TIME){
                ghosts[0].makeAllGhostsNormal(ghosts,NO_OF_GHOSTS);
                frenzyTimer = 0;
                speed = DEFAULT_SPEED;
            }
        }


    }



    /**
     * This is used to move the ghosts in level one. It changes direction whenever there is an encounter with the wall
     * */
    private void MoveGhosts(){
        int count = 0;
            for(int i=0;i<NO_OF_GHOSTS;i++){
                count++;
                if(ghosts[i] != null) {
                    if (ghosts[i].Encounter(walls, NO_OF_WALLS, GAME_OBJECT_LENGTH, WALL_LENGTH)) {
                        ghosts[i].changeDirection();
                    }
                    ghosts[i].move(ghosts[i].getxPos(),ghosts[i].getyPos());
                }
            }
    }

    /**
     * This equals method compares strings. It is used to read in from CSV
     * @param a is a string to compare with
     * @param b
     *
     * */
    public boolean equals(String a, String b){
        if(a.length() != b.length()){
            return false;
        } else {
            for(int i=0;i<a.length();i++){
                if(a.charAt(i) != b.charAt(i)){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * This method reads in the CSV for each method. It assigns the coordinates to it's respective objects
     * */
    private void readCSV() {
        int iterGhost = 0, iterWall=0, iterCherry=0, iterPallet=0,iterDot =0;

        String text = null;




        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))){

            while ((text = br.readLine()) != null){

                String[] splitText = text.split(",");

                if(equals(splitText[0],PLAYER_STR)){

                    player.setObjectPosition(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    currPosX = player.getxPos();
                    currPosY = player.getyPos();
                    initPosX = currPosX;
                    initPosY = currPosY;
                }
                else{
                    if(equals(splitText[0],GHOST_STR)){
                        ghosts[iterGhost] = new Ghost(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        iterGhost++;
                    } else if(equals(splitText[0],RED_GHOST_STR)){
                        redGhost = new RedGhost(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        ghosts[iterGhost] = redGhost;
                        iterGhost++;
                    } else if(equals(splitText[0],PINK_GHOST_STR)){
                        pinkGhost = new PinkGhost(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        ghosts[iterGhost] = pinkGhost;
                        iterGhost++;
                    }
                    else if(equals(splitText[0],GREEN_GHOST_STR)){
                        greenGhost = new GreenGhost(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        ghosts[iterGhost] = greenGhost;
                        iterGhost++;
                    }
                    else if(equals(splitText[0],BLUE_GHOST_STR)){
                        blueGhost = new BlueGhost(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        ghosts[iterGhost] = blueGhost;
                        iterGhost++;
                    }
                    else if(equals(splitText[0],WALL_STR)){
                        walls[iterWall] = new Wall(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        iterWall++;
                    }
                    else if(equals(splitText[0],DOT_STR)){
                        dots[iterDot] = new Dot(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                        iterDot++;
                    }
                    else if(equals(splitText[0],CHERRY_STR)){
                        cherry = new Cherry(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    }
                    else if(equals(splitText[0],PELLET_STR)){
                        pellet = new Pellet(splitText[0],Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    }


                }


            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    /**This method is to add the Heart Coordinates. Only called once at the start of the game
     * */
    private void AddHeartCoordinates() {
        int x = FIRST_HEART_POSITION_X;
        int y = FIRST_HEART_POSITION_Y;
        for (int i=0;i<NO_OF_HEARTS;i++){
            hearts[i] = new GameObject(HEART_STR,x,y);
            x+= 30;
        }

    }

}

