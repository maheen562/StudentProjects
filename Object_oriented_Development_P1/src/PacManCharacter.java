import java.awt.*;
import java.lang.Math.*;

public class PacManCharacter extends Coordinates{

    public PacManCharacter(){
        super();
    }
    public PacManCharacter(int x,int y)
    {
        super(x, y);
    }

    /*Checks if components are not overlapping*/
    public boolean CheckEncounter(Coordinates obstacleCoordinates, int newX,int newY,int length){
        //(EuclideanDistance(obstacleCoordinates,newX,newY,midPoint) < distance){
        if(CheckAtObstableLocation(obstacleCoordinates,newX,newY,length)/* < distance*/){

            return true;
        }
        return false;
    }


    public boolean CheckAtObstableLocation(Coordinates obstacleCoordinates, int newX, int newY,int length){

        Rectangle obstacle = new Rectangle(obstacleCoordinates.getX(),obstacleCoordinates.getY(),length,length);
        Rectangle player = new Rectangle(newX,newY,25,25);


        if(player.intersects(obstacle)){
            return true;
        }


        return false;
    }

}
