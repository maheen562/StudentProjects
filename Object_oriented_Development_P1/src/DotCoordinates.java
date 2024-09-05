public class DotCoordinates extends Coordinates{

    public DotCoordinates(){
        super();
    }
    public DotCoordinates(int x,int y){
        super(x, y);
    }

    public void RemoveDot(){
        this.setX(-1);
        this.setY(-1);
    }


}
