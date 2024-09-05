import bagel.DrawOptions;
import bagel.Font;
import bagel.Image;

/**
 * Display Object class is used to display all components of the game from Introductory messages to game components
 * */
public class DisplayObject {


    private final Font TITLE_FONT = new Font("res/FSO8BITR.TTF",64);/*Font used in game*/
    private final Font INTRO_MESSAGE_SUBTITLE = new Font("res/FSO8BITR.TTF",24);
    private final Font INTRO_LEVEL1_MESSAGE_SUBTITLE = new Font("res/FSO8BITR.TTF",40);
    private final Font SCORE = new Font("res/FSO8BITR.TTF",20);

    private final static String GAME_WON_MESSAGE = "WELL DONE!";/*Test used in game*/
    private final static String GAME_LOST_MESSAGE = "GAME OVER!";
    private final static String LEVEL_COMPLETE_MESSAGE = "LEVEL COMPLETE!";

    private final static String GAME_TITLE = "SHADOW PAC";
    private final static String GAME_SUBTITLE_LEVEL_0 = " PRESS SPACE TO START\nUSE ARROW KEYS TO MOVE";
    private final static String GAME_SUBTITLE_LEVEL_1 = " PRESS SPACE TO START\nUSE ARROW KEYS TO MOVE\n" +
                                                                        "EAT THE PELLET TO ATTACK";
    private final static String SCORE_TEXT = "SCORE ";

    private final static int GAME_TITLE_BOTTOM_X = 260; /*Coordinates for Title text in game. All titles positions
                                                                     adjusted around these coordinates*/
    private final static int GAME_TITLE_BOTTOM_Y = 250;




    private final Image DOTS = new Image("res/dot.png"); /*Images of all objects*/
    private final Image WALLS = new Image("res/wall.png");
    private final Image RED_GHOST = new Image("res/ghostRed.png");
    private final Image PINK_GHOST = new Image("res/ghostPink.png");
    private final Image GREEN_GHOST = new Image("res/ghostGreen.png");
    private final Image BLUE_GHOST = new Image("res/ghostBlue.png");
    private final Image FRENZY_GHOST = new Image("res/ghostFrenzy.png");
    private final Image PELLET = new Image("res/Pellet.png");
    private final Image CHERRY = new Image("res/Cherry.png");
    private final Image HEARTS = new Image("res/heart.png");
    private final Image OPEN_MOUTH_PAC = new Image("res/pacOpen.png");

    private final Image CLOSED_MOUTH_PAC = new Image("res/pac.png");
    private DrawOptions Rotation = new DrawOptions() ;/*Rotation of PacMan character*/


    /** drawGameComponent is used to display a game component in the game.
     * @param coordinates is an array of the top left positions of a certain object.
     * @param n is the maximum number of each component,
     * @param component is the Image to be displayed. This function is specific to objects that have multiples.
     * For instance Dots and Walls*/
    public void drawGameComponent(GameObject[] coordinates, int n, Image component) {

        for (int i = 0; i < n; i++) {
            if (coordinates[i] != null) {
                component.drawFromTopLeft(Double.valueOf(coordinates[i].getxPos()), Double.valueOf(coordinates[i].getyPos()));
            }

        }
    }

    /** drawGameComponent is used to display a game component in the game.
     * @param coordinates is the top left positions of a certain object.
     * @param component is the Image to be displayed. This function is specific to objects that have one instance.
     * For instance Cherry and Pallet*/
    public void drawGameComponent(GameObject coordinates, Image component) {

        if (coordinates != null) {
            component.drawFromTopLeft(Double.valueOf(coordinates.getxPos()), Double.valueOf(coordinates.getyPos()));
        }
    }

    /**This displays the message at the very start of the game
     * */
    public void showIntroMessageLevel0(){
        TITLE_FONT.drawString(GAME_TITLE,GAME_TITLE_BOTTOM_X,GAME_TITLE_BOTTOM_Y);
        INTRO_MESSAGE_SUBTITLE.drawString(GAME_SUBTITLE_LEVEL_0,
                GAME_TITLE_BOTTOM_X + 60,GAME_TITLE_BOTTOM_Y + 190);
    }

    /**This displays the message at the very start of level11
     * */
    public void showIntroMessageLevel1(){
        //TITLE_FONT.drawString(GAME_TITLE,GAME_TITLE_BOTTOM_X,GAME_TITLE_BOTTOM_Y);
        INTRO_LEVEL1_MESSAGE_SUBTITLE.drawString(GAME_SUBTITLE_LEVEL_1,
                200,350 );
    }

    /**This displays the message at the end of level 0
     * */
    public void LevelCompleteMessage(){
        TITLE_FONT.drawString(LEVEL_COMPLETE_MESSAGE,GAME_TITLE_BOTTOM_X-85,GAME_TITLE_BOTTOM_Y+90);
    }

    /**This displays the Game won message
     * */
    public void showGameWonMessage(){
        TITLE_FONT.drawString(GAME_WON_MESSAGE,GAME_TITLE_BOTTOM_X ,GAME_TITLE_BOTTOM_Y+ 150);
    }
    /*Losing message printed on screen*/

    /**This displays the Game lost message
     * */
    public void showGameLostMessage(){
        TITLE_FONT.drawString(GAME_LOST_MESSAGE,GAME_TITLE_BOTTOM_X ,GAME_TITLE_BOTTOM_Y+ 150);
    }


    /**This displays the Game Score
     * */
    public void displayScore(int score){
        SCORE.drawString(SCORE_TEXT + String.valueOf(score),25,25);

    }
    int countForOpenMouth = 0;/*Countor for open mouth render*/
    private final static int PLAYER_IMG_RENDER_TIME = 15; /*Render time for Pac and PacOpen character*/

    /**
     * This method is called in the Update method. It draws all the game components in the game including the live
     * hearts, ghosts, walls, dots, cherry, pellet, PacMan character , score etc. This method is used for both level0
     * and level 1. As any object which is null is ignored, it is feasible to have the same function for both levels.
     * Components like ghosts which are different for level 0 and level 1 have a check on them when displayed.
     * */
    public final int OPEN_MOUTH_REFERENCE_INDEX = 1;
    public final int CLOSED_MOUTH_REFERENCE_INDEX = 2;


    public void drawGame(Wall[] walls,Dot[] dots,GameObject[] hearts,Ghost[] ghosts,PinkGhost pinkGhost
                        ,PacManCharacter player,  BlueGhost blueGhost, RedGhost redGhost,GreenGhost greenGhost,
                          Pellet pellet,Cherry cherry,double directionDegree,int score){

        drawGameComponent(walls,ShadowPac.NO_OF_WALLS,WALLS);
        drawGameComponent(dots,ShadowPac.NO_OF_DOTS,DOTS);
        drawGameComponent(hearts,ShadowPac.getLives(),HEARTS);

        if(ShadowPac.isLevel0Complete() == false){
            drawGameComponent(ghosts,ShadowPac.NO_OF_GHOSTS,RED_GHOST);
        }
        else if(ghosts[0].getFrenzy() == true){
            drawGameComponent(ghosts,ShadowPac.NO_OF_GHOSTS,FRENZY_GHOST);
        }
        else {
            drawGameComponent(pinkGhost,PINK_GHOST);
            drawGameComponent(blueGhost,BLUE_GHOST);
            drawGameComponent(redGhost,RED_GHOST);
            drawGameComponent(greenGhost,GREEN_GHOST);
            drawGameComponent(pellet,PELLET);
            drawGameComponent(cherry,CHERRY);
        }

        displayScore(score);
        countForOpenMouth++;
        if (countForOpenMouth <= PLAYER_IMG_RENDER_TIME) {
            DrawPlayer(OPEN_MOUTH_REFERENCE_INDEX, directionDegree,player);
        } else if (countForOpenMouth < (PLAYER_IMG_RENDER_TIME * 2)) {
            DrawPlayer(CLOSED_MOUTH_REFERENCE_INDEX, directionDegree,player);
        } else if (countForOpenMouth == (PLAYER_IMG_RENDER_TIME * 2)) {
            DrawPlayer(CLOSED_MOUTH_REFERENCE_INDEX, directionDegree,player);
            countForOpenMouth = 0;
        }

    }

    /**
     * DrawPlayer is rendering 2 images after a certain timeframe of each. This is called in the Draw Game fuction.
     * @param imgNo tells which image number to render.
     * @param directionCoordinate tells us the degree in radian for rotation
     * @param player is the pacman player, used to get current position
     * */

    public void DrawPlayer(int imgNo, double directionCoordinate, PacManCharacter player){
        if(imgNo == OPEN_MOUTH_REFERENCE_INDEX){
            OPEN_MOUTH_PAC.drawFromTopLeft(Double.valueOf(player.getxPos()),Double.valueOf(player.getyPos()),
                    Rotation.setRotation(Double.valueOf(directionCoordinate)));
        }
        else{
            CLOSED_MOUTH_PAC.drawFromTopLeft(Double.valueOf(player.getxPos()),Double.valueOf(player.getyPos()),
                    Rotation.setRotation(Double.valueOf(directionCoordinate)));
        }
    }


}


