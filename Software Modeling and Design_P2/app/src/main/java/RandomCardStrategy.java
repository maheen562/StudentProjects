// RandomCardStrategy.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import java.util.ArrayList;
import ch.aplu.jcardgame.Card;
import ch.aplu.jcardgame.Hand;
import java.util.Random;
import static ch.aplu.util.BaseTimer.delay;

public class RandomCardStrategy implements CardStrategy{
    private CountingUpRules rules = new CountingUpRules();
    private static final Random random = new Random(); /*for the random card selection*/

    /**
     * This code determines if the ace of clubs is played, and if it has it picks a card using the random card
     * strategy, which is just a random valid card. It also displays the relevant prompt.
     *
     * @param playerHand is the hand of the current player
     * @param lastPlayedCard is the last played card in the game, used for validity checking
     * @param nextPlayer used to determine what is the player index of the current player
     *
     * @return is the card selected by the player*/
    public Card playCard(Hand playerHand, Card lastPlayedCard, int nextPlayer){
        rules.setStatus("Player " + nextPlayer + " thinking...");
        delay(rules.gameThinkingTime());

        if (rules.CheckAceClubPlayed()){
            return cardStrategy(playerHand.getCardList(),lastPlayedCard);

        }else{
            return rules.drawAceClubCard(playerHand.getCardList());
        }
    }

    /**
     * This method goes through the card list of the player and selects the card that needs to be selected using the
     * current strategy.
     *
     * @param playerCards is the cards of the current player
     * @param lastPlayedCard is the card of the last player for validity purposes

     * @return selected card using the basic card strategy
     * */
    public Card cardStrategy(ArrayList<Card> playerCards, Card lastPlayedCard){
        ArrayList<Card> validCards = new ArrayList<>();
//        Separate the player's cards into valid ones so that a valid card is returned.
        for (Card card : playerCards) {
            if (rules.isValidPlay(card, lastPlayedCard)) {
                validCards.add(card);
            }
        }
        if (validCards.isEmpty()) {
            return null;  // No valid card, so return null to indicate a pass
        }
//        Return a random card from the list of valid cards.
        return validCards.get(random.nextInt(validCards.size())); // Return a random valid card
    }
}
