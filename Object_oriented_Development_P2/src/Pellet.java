    public class Pellet extends GameObject implements Removable{

        public Pellet(){super();}

        public Pellet(String objectName,int xPos,int yPos)
        {
            super(objectName,xPos, yPos);
        }


        @Override
        public void remove() {

            this.setxPos(-20);
            this.setyPos(-20);
            this.setObjectName("");
        }
    }


