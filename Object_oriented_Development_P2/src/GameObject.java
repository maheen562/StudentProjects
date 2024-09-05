import bagel.AbstractGame;

import java.awt.*;

/**
 * GameObject is the parent class for all game objects in the game. It is not made abstract because then we wouldn't be
 * able to use it for Objects like lives, which need to be reprsented in the game, but do not need a seperate class.
 * */
public class GameObject{
    int xPos;/*x position of object*/
    int yPos;/*y position of object*/
    String objectName;/*string name of object*/


    /**
     * Game object intilized with empty string
     * */
    public GameObject(){
        xPos = 0;
        yPos = 0;
        objectName = "";
    }

    /**
     * Initializes gameObject X coordinate, y coordinate, objectname initially
     * @param xPos is the xPos we want to set
     * @param yPos is the yPos we want to set
     * @param objectName is the objectName we want to set
     *
     * */
    public GameObject(String objectName, int xPos,int yPos){
        this.xPos = xPos;
        this.yPos = yPos;
        this.objectName = objectName;
    }

    /**
     * Initializes gameObject X coordinate, y coordinate, objectname initially
     * @param xPos is the xPos we want to set
     * @param yPos is the yPos we want to set
     * @param objectName is the objectName we want to set
     *
     * */
    public void setObjectPosition(String objectName, int xPos,int yPos){
        this.xPos = xPos;
        this.yPos = yPos;
        this.objectName = objectName;
    }


    /**
     * Sets gameObject X coordinate, y coordinate only
     * @param xPos is the xPos we want to set
     * @param yPos is the yPos we want to set
     *
     * */
    public void setObjectPosition(int xPos,int yPos){
        this.xPos = xPos;
        this.yPos = yPos;
    }

    /**
     * gets gameObject X position only
     * */
    public int getxPos() {
        return xPos;
    }

    /**
     * gets gameObject Y position only
     * */
    public int getyPos() {
        return yPos;
    }


    /**
     * gets gameObject ObjectName only
     * */
    public String getObjectName() {
        return objectName;
    }


    /**
     * sets gameObject xPos only
     * @param xPos in the new xPos we want to set
     * */
    public void setxPos(int xPos) {
        this.xPos = xPos;
    }


    /**
     * sets gameObject yPos only
     * @param yPos in the new yPos we want to set
     * */
    public void setyPos(int yPos) {
        this.yPos = yPos;
    }

    /**
     * sets gameObject objectname only
     * @param  objectName the new yPos we want to set
     * */
    public void setObjectName(String objectName) {
        this.objectName = objectName;
    }

    /**
     * Checks if two objects have collided
     * @param gameObjectA is the rectangle surrounding object A
     * @param gameObjectB is the rectangle surrounding object B
     *
     * */
    public boolean Encounter(Rectangle gameObjectA, Rectangle gameObjectB){
        if(gameObjectA.intersects(gameObjectB)){
            return true;
        }
        return false;
    }

    /**
     * Checks if an object A has collided with an array of objects. e.g checking if the player has collided with any of the
     * walls
     * @param gameObjectA is the rectangle surrounding object A
     * @param gameObjects is the array of other object
     * @param noOfObjects is the length of the array of the array object
     *
     * @param objectLength is the length of the array object
     *
     * */
    public boolean Encounter(Rectangle gameObjectA, GameObject[] gameObjects, int noOfObjects,int objectLength){
        for(int i=0;i<noOfObjects;i++){
            if(gameObjects[i] != null){
                Rectangle obstacle = new Rectangle(gameObjects[i].getxPos(),gameObjects[i].getyPos(),objectLength,objectLength);
                if(Encounter(gameObjectA,obstacle)){
                    return true;
                }
            }

        }
        return false;
    }

    /**
     * Checks if an object A has collided with an array of objects. e.g checking if the player has collided with any of the
     * walls
     * @param gameObjects is the array of other object
     * @param noOfObjects is the length of the array of the array object
     * @param objectALength is the length of the first object
     * @param objectBLength is the length of the array object
     * This is specific to be used by inherited gameobjects
     *
     * */
    public boolean Encounter(GameObject[] gameObjects, int noOfObjects,int objectALength,int objectBLength){

        Rectangle rectObject = new Rectangle(this.getxPos(),this.getyPos(),objectALength,objectALength);

        for(int i=0;i<noOfObjects;i++){
            if(gameObjects[i] != null){
                Rectangle eachObjectB = new Rectangle(gameObjects[i].getxPos(),gameObjects[i].getyPos(),objectBLength,objectBLength);
                if(Encounter(rectObject,eachObjectB)){
                    return true;
                }
            }

        }
        return false;
    }
    /**
     * Checks if an object A has collided with another object
     * @param gameObjectB is the object to see collision with
     * @param objectALength is the length of the first object
     * @param objectBLength is the length of the array object
     * This is specific to be used by inherited gameobjects
     *
     * */
    public boolean Encounter(GameObject gameObjectB,int objectALength,int objectBLength){

        Rectangle rectObjectA = new Rectangle(this.getxPos(),this.getyPos(),objectALength,objectALength);
        Rectangle rectObjectB = new Rectangle(gameObjectB.getxPos(),gameObjectB.getyPos(),objectBLength,objectBLength);
        if(Encounter(rectObjectA,rectObjectB)){
            return true;
        }
        return false;
    }

    /**
     * Checks if to be made by object is Valid
     * @param newPosX is the new X position it will move to.
     * @param newPosY is the new Y position it will move to.
     * @param obstacles is any obstacles it may encounter like walls
     * @param noOfObstacles is the number of obstables it will encounter
     * @param objectLength is the pixal length og object
     * @param obstacleLength is the pixal length og obstacle
     * @return true if true
     * @return false if false
     *
     *
     * */
    public boolean isValidMove(int newPosX,int newPosY, GameObject[] obstacles,int noOfObstacles,int objectLength, int obstacleLength){
        if(newPosX > (ShadowPac.WINDOW_WIDTH) && (newPosY>ShadowPac.WINDOW_HEIGHT)){
            return false;
        }

            Rectangle object = new Rectangle(newPosX,newPosY,objectLength,objectLength);

            if(Encounter(object,obstacles,noOfObstacles,obstacleLength)){
                return false;
            }

        return true;
    }

}
