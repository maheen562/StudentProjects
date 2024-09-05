// CardStrategy.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.*;

@SuppressWarnings("serial")

public interface CardStrategy {

    /**
     * This method selects a valid card to be played according to the specific strategy that implements
     * this interface.
     *
     * @param playerHand is the hand of the current player
     * @param lastPlayedCard is the last played card in the game, used for
     * @param nextPlayer used to determine what is the player index of the current player
     *
     * @return is the card selected by the player*/
    Card playCard(Hand playerHand, Card lastPlayedCard, int nextPlayer);


}
