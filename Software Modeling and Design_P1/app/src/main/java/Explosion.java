// Explosion.java
// Used for SpaceInvader
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jgamegrid.*;

public class Explosion extends Actor
{
  public Explosion()
  {
    super("sprites/explosion1.gif");
    setSlowDown(3);
  }

  public void act()
  {
    removeSelf();
  }


}
