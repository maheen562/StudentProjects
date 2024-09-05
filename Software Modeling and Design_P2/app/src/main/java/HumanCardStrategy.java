// HumanCardStrategy.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.Card;
import ch.aplu.jcardgame.Hand;
import static ch.aplu.util.BaseTimer.delay;

public class HumanCardStrategy  implements CardStrategy{

    private CountingUpRules rules = new CountingUpRules();
    private final int delayTime = 600; /*The time delay between a human selection and when the card is placed
                                                                                                on the board*/
    private boolean isWaitingForPass = false; /*boolean to check if human has completed turn*/

    /**
     * This function gets the human gesture in the game, checks for the validity of the card and if it is valid it
     * returns the card. If the human passes it returns null. If the human picks an invalid card, the code prompts it
     * to pick a valid card or pass their turn. It also checks if the ace of clubs is played, if it is not played and
     * the human player has the ace of clubs, then it must be played.
     *
     * @param playerHand is the hand of the current player
     * @param lastPlayedCard is the last played card in the game, used for
     * @param nextPlayer used to determine what is the player index of the player
     *
     * @return is the card selected by the player*/
    @Override
    public Card playCard(Hand playerHand, Card lastPlayedCard,int nextPlayer){
        playerHand.setTouchEnabled(true);
        rules.setSelectedCard(null);

        if (rules.CheckAceClubPlayed()){
            rules.setStatus("Player 0 double-click on card to follow or press Enter to pass");

            waitForPlayerAction();

//            If the player passes then exit the method.
            if (rules.getSelectedCard() == null){
                return null;
            }
//            Keep waiting until either the player selects a card or passes.
            if (rules.getSelectedCard() != null) {
//                Check the selected card for validity.
                while (!rules.isValidPlay(rules.getSelectedCard(), lastPlayedCard)) {
                    rules.setSelectedCard(null);

                    // Card is not valid, so ignore the selection and display a warning
                    rules.setStatus("Invalid card selected. Player 0 double-click on a valid card to follow or press Enter to pass");
                    playerHand.setTouchEnabled(true);
                    waitForPlayerAction();
//                    The player has passed their turn.
                    if (!isWaitingForPass) return null;

                }
            }
            return rules.getSelectedCard();

        }else{
            rules.setStatus("Player 0 must play the Ace of Clubs.");

            return rules.drawAceClubCard(playerHand.getCardList());
        }

    }

    /**
     *  This method is used to wait until the human player either selects a card or passes their turn,
     *      in other words it waits for an action to be performed.*/

    private void waitForPlayerAction() {
        isWaitingForPass = true;

        // Continuously loop until a card is selected or the human player decides to pass.
        while (rules.getSelectedCard() == null && !rules.getKeyPressed() && isWaitingForPass) {
            delay(delayTime);
        }
//      The human has passed their turn.
        if(rules.getKeyPressed()){
            rules.UpdateKeyPressed();
            isWaitingForPass = false;

        }

    }

}
