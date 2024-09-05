import bagel.*;
import java.util.Scanner;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;



/**
 * Skeleton Code for SWEN20003 Project 1, Semester 1, 2023
 *
 * Please enter your name below
 * @mabdulkhaliq
 * Maheen Abdul Khaliq Khan, 1193813, ShadowPac game, Semester 1 2023
 */
public class ShadowPac extends AbstractGame  {
    private final static int WINDOW_WIDTH = 1024; /*Screen Window Width*/
    private final static int WINDOW_HEIGHT = 768; /*Screen Window Height*/

    private final static int GAME_TITLE_BOTTOM_X = 260; /*Coordinates for Title text in game*/
    private final static int GAME_TITLE_BOTTOM_Y = 250;

    private final static String GAME_TITLE = "SHADOW PAC"; /*All text used in game*/

    private final static String GAME_SUBTITLE = " PRESS SPACE TO START\nUSE ARROW KEYS TO MOVE";
    private final static String GAME_WON_MESSAGE = "WELL DONE!";
    private final static String GAME_LOST_MESSAGE = "GAME OVER!";
    private final static String SCORE_TEXT = "SCORE ";

    private final static int NO_OF_GHOSTS = 4; /*Total of each game components*/
    private final static int NO_OF_WALLS = 145;

    private final static int NO_OF_DOTS = 121;

    private final static int NO_OF_HEARTS = 3;
    private final static int SPEED = 4; /*Speed of moving Pacman Character*/

    private final static int PLAYER_IMG_RENDER_TIME = 15; /*Render time for Pac and PacOpen character*/
    private final static int RIGHT_ROTATION = 0; /*PacMan rotation with direction*/
    private final static int LEFT_ROTATION = 90;
    private final static int UP_ROTATION = 180;
    private final static int DOWN_ROTATION = 270;
    private final Image BACKGROUND_IMAGE = new Image("res/background0.png");/*Images used in game*/

    private final Image WALLS = new Image("res/wall.png");
    private final Image DOTS = new Image("res/dot.png");
    private final Image GHOSTS = new Image("res/ghostRed.png");

    private final Image HEARTS = new Image("res/heart.png");

    private final Image OPEN_MOUTH_PAC = new Image("res/pacOpen.png");

    private final Image CLOSED_MOUTH_PAC = new Image("res/pac.png");
                                                    /*Pixel length sizes of obstacles*/
    public final int WALL_LENGTH = 50;
    public final int DOT_LENGTH = 13;
    public final int GHOST_LENGTH = 17;




    private final Font TITLE_FONT = new Font("res/FSO8BITR.TTF",64);/*Font used in game*/
    private final Font INTRO_MESSAGE_SUBTITLE = new Font("res/FSO8BITR.TTF",24);

    private final Font SCORE = new Font("res/FSO8BITR.TTF",20);
                                                    /*Constants from state changes*/
    private static boolean spacePressed = false; /*Check if space pressed to start game*/

    private static boolean csvread = false;/*Check if csv of level read*/

    private boolean liveLost = false;/*Check if  player looses a life*/
    private boolean gameWon = false; /*Check if game Won*/
    private boolean gameLost = false;/*Check if game lost*/
    private int lives = 3;/*No of lives*/

    private int score = 0; /*Score of player*/
    int countForOpenMouth = 0;/*Countour for open mouth render*/
    int directionDegree = 0; /*Check direction of move made*/

    private int initialX = 0;
    private int initialY = 0;




    public ShadowPac(){
        super(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE);
    }


    /*Arrays for components in game*/
    private GhostCoordinates[] ghostCoordinates = new GhostCoordinates[NO_OF_GHOSTS];


    private DotCoordinates[] dotCoordinates = new DotCoordinates[NO_OF_DOTS];
    private WallCoordinates[] wallCoordinates = new WallCoordinates[NO_OF_WALLS];

    private Coordinates[] hearts = new Coordinates[NO_OF_HEARTS];
    private PacManCharacter player = new PacManCharacter();

    int playerX = 0; /*Position of where the player moves*/
    int playerY = 0;

    private DrawOptions Rotation = new DrawOptions() ;/*Ratation of PacMan character*/
    int direction = 0;
    boolean obstacle = false;


    private static String csvFile = null;
    /*Reading file from command line. Base code taken from Lecture 8 slides*/


    /*Intializing all component arrays*/
    private void InitializeGhostArray(){
        for (int i=0;i<NO_OF_GHOSTS;i++){
            ghostCoordinates[i] = new GhostCoordinates();
        }

    }

    private void InitializeDotArray(){
        for (int i=0;i<NO_OF_DOTS;i++){
            dotCoordinates[i] = new DotCoordinates();
        }

    }

    private void InitializeWallArray(){
        for (int i=0;i<NO_OF_WALLS;i++){
            wallCoordinates[i] = new WallCoordinates();
        }

    }

    private void InitializeHeartsArray(){
        for (int i=0;i<NO_OF_HEARTS;i++){
            hearts[i] = new Coordinates();
        }
        int x = 900;
        int y = 10;
        for(int i=0;i<NO_OF_HEARTS;i++){
            hearts[i].setCoordinates(x,y);
            x+= 30;
        }
    }
    /*Constants to help read csv*/
    private final String GHOST_STR = "Ghost";
    private final String WALL_STR = "Wall";
    private final String DOT_STR = "Dot";
    private final String PLAYER_STR = "Player";
    /**
     * Method used to read file and create objects
     */
    private void readCSV() {

        int iterGhosts = 0,iterWalls = 0,iterDots = 0;

        String text = null;

        //String csvFile = argv[2];
        InitializeGhostArray();
        InitializeDotArray();
        InitializeWallArray();
        InitializeHeartsArray();




        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))){

            while ((text = br.readLine()) != null){

                String[] splitText = text.split(",");

                if(equals(splitText[0],GHOST_STR)){

                    ghostCoordinates[iterGhosts].setCoordinates(Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    iterGhosts++;

                }
                else if(equals(splitText[0],WALL_STR)){

                    wallCoordinates[iterWalls].setCoordinates(Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    iterWalls++;

                }
                else if(equals(splitText[0],DOT_STR)){

                    dotCoordinates[iterDots].setCoordinates(Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    iterDots++;

                }
                else if(equals(splitText[0],PLAYER_STR)){
                    player.setCoordinates(Integer.parseInt(splitText[1]),Integer.parseInt(splitText[2]));
                    playerX = player.getX();
                    playerY = player.getY();
                    initialX = playerX;
                    initialY = playerY;

                }




            }
        } catch (Exception e){
            e.printStackTrace();
        }


    }


    public static void main(String[] args) {
        csvFile = args[0];
        ShadowPac game = new ShadowPac();
        game.run();
    }

    /**
     * Performs a state update.
     * Allows the game to exit when the escape key is pressed.
     */
    /*Updated when key pressed*/


    @Override
    protected void update(Input input) {


        if (input.wasPressed(Keys.ESCAPE)){
            Window.close();
        }

        BACKGROUND_IMAGE.draw(Window.getWidth()/2.0, Window.getHeight()/2.0);

        if((input.wasPressed(Keys.SPACE))){
            spacePressed = true;
        }

        if (!(spacePressed)) {
            showIntroMessage();
        }
        else{
            if(!(csvread)){
                readCSV();
                csvread = true;
            }

            if (input.isDown(Keys.LEFT)) {
                //if(player.isValid(playerX,playerY)){
                    playerX -= SPEED;
                    directionDegree = LEFT_ROTATION;

                //}

            }
            if (input.isDown(Keys.RIGHT)) {

                //if(player.isValid(playerX,playerY)){
                    playerX += SPEED;
                    directionDegree = RIGHT_ROTATION;

                //}

            }
            if (input.isDown(Keys.UP)) {
               // if(player.isValid(playerX,playerY)) {
                    playerY -= SPEED;
                    directionDegree = UP_ROTATION;

                //}

            }
            if (input.isDown(Keys.DOWN)) {
                //if(player.isValid(playerX,playerY)) {
                    playerY += SPEED;
                    directionDegree = DOWN_ROTATION;
                //}
            }


            //System.out.println("Test");
            if(player.isValid(playerX,playerY) ){
               // player.setX(playerX);
               // player.setY(playerY);

                for(int i=0;i<NO_OF_WALLS;i++){
                    if(player.CheckEncounter(wallCoordinates[i],playerX,playerY,WALL_LENGTH)){
                        obstacle = true;
                        break;
                    }
                }
                if(!obstacle){
                    player.setX(playerX);
                    player.setY(playerY);
                }
                else {
                    playerX = player.getX();
                    playerY = player.getY();
                    obstacle = false;

                }

                for(int i=0;i<NO_OF_DOTS;i++){
                    if(player.CheckEncounter(dotCoordinates[i],playerX,playerY,DOT_LENGTH)){
                        dotCoordinates[i].RemoveDot();
                        score += 10;
                        if (score == 1210){
                            gameWon = true;
                        }
                        break;
                    }
                }

                for(int i=0;i<NO_OF_GHOSTS;i++){
                    if(player.CheckEncounter(ghostCoordinates[i],playerX,playerY,GHOST_LENGTH)){
                        lives--;
                        playerX = initialX;
                        playerY = initialY;

                        if(lives == 0){
                            gameLost = true;
                        }
                        //player.setX(initialX);
                        //player.setY(initialY);

                    }

                }

            }

            else {
                playerX = player.getX();
                playerY = player.getY();
            }


            if(!gameWon && !gameLost){
                drawGame();
                countForOpenMouth++;
                if(countForOpenMouth <= PLAYER_IMG_RENDER_TIME){
                    DrawPlayer(1,directionDegree);
                }
                else if(countForOpenMouth <(PLAYER_IMG_RENDER_TIME*2)){
                    DrawPlayer(2,directionDegree);
                }
                else if(countForOpenMouth == (PLAYER_IMG_RENDER_TIME*2)){
                    DrawPlayer(2,direction);
                    countForOpenMouth = 0;
                }

            } else if (gameWon) {
                showGameWonMessage();
            }
            else if (gameLost){
                showGameLostMessage();
            }



        }





    }

    /*equals method for comparing strings*/
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



    /*Staring message printed on screen*/
    private void showIntroMessage(){
        TITLE_FONT.drawString(GAME_TITLE,GAME_TITLE_BOTTOM_X,GAME_TITLE_BOTTOM_Y);
        INTRO_MESSAGE_SUBTITLE.drawString(GAME_SUBTITLE,GAME_TITLE_BOTTOM_X + 60,GAME_TITLE_BOTTOM_Y + 190);
    }
    /*Winning message printed on screen*/
    private void showGameWonMessage(){
        TITLE_FONT.drawString(GAME_WON_MESSAGE,GAME_TITLE_BOTTOM_X ,GAME_TITLE_BOTTOM_Y+ 150);
    }
    /*Losing message printed on screen*/
    private void showGameLostMessage(){
        TITLE_FONT.drawString(GAME_LOST_MESSAGE,GAME_TITLE_BOTTOM_X ,GAME_TITLE_BOTTOM_Y+ 150);
    }
    /*Draws game graphics in update*/
    private void drawGame(){
        drawGameComponent(ghostCoordinates,NO_OF_GHOSTS,GHOSTS);
        drawGameComponent(wallCoordinates,NO_OF_WALLS,WALLS);
        drawGameComponent(dotCoordinates,NO_OF_DOTS,DOTS);
        drawGameComponent(hearts,lives,HEARTS);
        SCORE.drawString(SCORE_TEXT + String.valueOf(score),25,25);

    }
    /*Draws player in update*/
    private void DrawPlayer(int imgNo, int directionCoordinate){
        if(imgNo == 1){
            OPEN_MOUTH_PAC.drawFromTopLeft(Double.valueOf(player.getX()),Double.valueOf(player.getY()),
                        Rotation.setRotation(Double.valueOf(directionCoordinate)));
        }
        else{
            CLOSED_MOUTH_PAC.drawFromTopLeft(Double.valueOf(player.getX()),Double.valueOf(player.getY()),
                        Rotation.setRotation(Double.valueOf(directionCoordinate)));
        }
    }


    private void drawGameComponent(Coordinates[] coordinates,int n,Image component){
        for(int i=0;i<n;i++){
            component.drawFromTopLeft(Double.valueOf(coordinates[i].getX()),Double.valueOf(coordinates[i].getY()));
        }
    }
}
