// BasicCardStrategy.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.Card;
import ch.aplu.jcardgame.Hand;

import java.util.ArrayList;

import static ch.aplu.util.BaseTimer.delay;

public class BasicCardStrategy implements CardStrategy{
    private CountingUpRules rules = new CountingUpRules();

    /**
     * This code determines if the ace of clubs is played, and if it has it picks a card using the basic card strategy,
     * which is the lowest valid card. It also displays the relevant prompt.
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

        }
        else{
            return rules.drawAceClubCard(playerHand.getCardList());
        }
    }

    /**
     * @param playerCards is the cards of the current player
     * @param lastPlayedCard is the card of the last player for validity purposes
     * This method goes through the card list of the player and selects the card that needs to be selected using the
     *                       current strategy
     * @return selected card using the basic card strategy
     * */
    public Card cardStrategy(ArrayList<Card> playerCards, Card lastPlayedCard){
        Card minvalcard = null;

        for (Card card : playerCards) {
            if (rules.isValidPlay(card, lastPlayedCard)) {
                if (minvalcard !=null && card !=null){
                    if(rules.getRankCardValue(minvalcard) > rules.getRankCardValue(card)){
                        minvalcard = card;
                    }
                }
                else if(minvalcard == null && card !=null) {
                    minvalcard = card;
                }
            }
        }

        return minvalcard;
    }
}

