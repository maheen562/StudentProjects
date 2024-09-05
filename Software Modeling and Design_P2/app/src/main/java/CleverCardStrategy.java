// CleverCardStrategy.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.Card;
import ch.aplu.jcardgame.Hand;
import java.util.ArrayList;

import static ch.aplu.util.BaseTimer.delay;

public class CleverCardStrategy implements CardStrategy{

    private CountingUpRules rules = new CountingUpRules();
    private final int KING_SCORE = 13;

    /**This method selects a valid card selected by the clever player. It uses the following
     * inputs/ arguments to the function. It also displays the relevant prompt.
     *
     * @param playerHand is the hand of the current player
     * @param lastPlayedCard is the last played card in the game, used for validity checks
     * @param nextPlayer used to determine what is the player index of the current player
     *
     * @return the card selected by the player
     * */
    public Card playCard(Hand playerHand, Card lastPlayedCard, int nextPlayer){
        rules.setStatus("Player " + nextPlayer + " thinking...");
        delay(rules.gameThinkingTime());

//        if the ace of clubs has been played then we pick another card.
        if (rules.CheckAceClubPlayed()){
            return cardStrategy(playerHand.getCardList(),lastPlayedCard);
        }

//        the ace of clubs hasn't been played yet, so we have to play it first.
        else{
            return rules.drawAceClubCard(playerHand.getCardList());
        }
    }

    /**
     *This method goes through the card list of the player and selects the card that needs to be
     * selected based on the clever strategy. It uses the following arguments/ inputs to the method:
     *
     * @param playerCards is the cards of the current/ clever player
     * @param lastPlayedCard is the card that was played last for the purpose of checking validity

     * @return selected card using the clever strategy
     * */
    public Card cardStrategy(ArrayList<Card> playerCards, Card lastPlayedCard) {
        Card selectedCard = null;
        int kingCounter = pictureCardCount(playerCards, KING_SCORE);

//       If we have an ace, we get rid of it asap as they have the lowest value, but can
//       cause a big negative score as they're worth -10 points if we still have them when the game ends.
        selectedCard = chooseAces(playerCards, lastPlayedCard);
        if (selectedCard != null) {
            return selectedCard;
        }


//       The king is the most powerful card so we want to save it until everyone else has played their kings. In order
//       to hurry this along, we sacrifice our queens and jacks to draw out the other players into using up their
//       high-powered cards-especially their kings.
        if(!rules.getCardsPlayed().containsKey("K") || ((Integer) rules.getCardsPlayed().get("K")) <= 2 ||
                kingCounter <= 1){
            //            only play queen or jack to draw out the other kings.
            selectedCard = chooseQueenJack(playerCards, lastPlayedCard);
            if(selectedCard != null){
                return selectedCard;
            }
        }

//      If we have a king then we only want to play it if others have played their kings.
        if(rules.getCardsPlayed().containsKey("K")){
            selectedCard = chooseKing(playerCards, lastPlayedCard, kingCounter);
            if(selectedCard != null){
                return selectedCard;
            }
        }

//      If none of the above strategies are able to find a card, then we want to play our lowest value card
//      to draw out the round so that we can play for as long as possible in the round and prevent our
//      turn from being skipped due to a lack of valid cards. Playing the smallest cards allows other players to
//      build on top of ours.
        else{
            selectedCard = minCard(playerCards, lastPlayedCard);
            if(selectedCard != null){
                return selectedCard;
            }

        }

//      If none of the above strategies were able to find a card, it's best for us to skip our turn at the moment.
        return selectedCard;
    }

    /** This method goes through the entire hand of cards that the clever player has and chooses an Ace
     * if we have one. It uses the following arguments/ inputs
     *
     * @param playerCards this is a list of the cards the clever player has
     * @param lastPlayedCard this is the last played card, used to check validity
     *
     * @return if an ace is found, it is returned. Otherwise, null is returned.
     * **/
    public Card chooseAces(ArrayList<Card> playerCards, Card lastPlayedCard){
        Card aceCard = null;
        for (Card card : playerCards) {
//            Found an ace card, which we return.
            int ACE_SCORE = 1;
            if (rules.getRankCardValue(card) == ACE_SCORE && rules.isValidPlay(card, lastPlayedCard)) {
                aceCard = card;
                return aceCard;
            }
        }
//        An ace wasn't found so we return null.
        return aceCard;

    }

    /** This method goes through the entire hand of cards that the clever player has and chooses to play
     * a King only if certain conditions are met. It uses the following arguments/ inputs
     *
     * @param playerCards this is a list of the cards the clever player has
     * @param lastPlayedCard this is the last played card, used to check validity
     * @param kingCount this is a count of how many Kings are in the clever player's hand.
     *
     * @return if a king is found and the conditions are met, it is returned. Otherwise, null is returned.
     * **/
    public Card chooseKing(ArrayList<Card> playerCards, Card lastPlayedCard, int kingCount){
        Card kingCard = null;
        for (Card card : playerCards) {
//            if we have either 3 king cards or 4, then play it as we'll win the round no matter what.
            if (rules.getRankCardValue(card) == KING_SCORE && (kingCount == 3 || kingCount == 4) &&
                    rules.isValidPlay(card, lastPlayedCard)) {
                rules.setCardsPlayed(card);
                kingCard = card;
                return kingCard;
            }
//            if we have 2 kings in our hand and 2 kings have been played then we play our king as we win
//            no matter what.
            else if (rules.getRankCardValue(card) == KING_SCORE && (kingCount == 2) &&
                    rules.getCardsPlayed().get("K").equals(2) &&
                    rules.isValidPlay(card, lastPlayedCard)){
                rules.setCardsPlayed(card);
                kingCard = card;
                return kingCard;
            }
//            if we have only a single king card then we only want to play it if all other kings have been played
//            to ensure we win the round.
            else if (rules.getRankCardValue(card) == KING_SCORE && (kingCount == 1) &&
                    rules.getCardsPlayed().get("K").equals(3) &&
                    rules.isValidPlay(card, lastPlayedCard)) {
                rules.setCardsPlayed(card);
                kingCard = card;
                return kingCard;
            }
        }
//        No king has been found that meets the conditions so we return null.
        return kingCard;

    }

    /** This method goes through the entire hand of cards that the clever player has and chooses to play
     * a Queen or Jack only if certain conditions are met, so that we can draw out the high-powered cards of the
     * other players, specifically the Kings of the other players. It uses the following arguments/ inputs:
     *
     * @param playerCards this is a list of the cards the clever player has
     * @param lastPlayedCard this is the last played card, used to check validity
     *
     * @return if a queen or jack is found and the conditions are met, it is returned. Otheriwse null is returned.
     * **/
    public Card chooseQueenJack(ArrayList<Card> playerCards, Card lastPlayedCard){
        Card selectedCard = null;
        int JACK_SCORE = 11;
        int jackCount = pictureCardCount(playerCards, JACK_SCORE);
        int QUEEN_SCORE = 12;
        int queenCount = pictureCardCount(playerCards, QUEEN_SCORE);

//        if there is a king that hasn't been played yet by the other players then we want to draw it out.
        if(rules.getCardsPlayed().containsKey("K") && ((Integer) rules.getCardsPlayed().get("K")) < 4){
//          If we have a Jack we want to sacrifice it before the Queen as the Queen is more powerful.
            if (jackCount > 0){
                for (Card card: playerCards){
                    if(rules.getRankCardValue(card) == JACK_SCORE && rules.isValidPlay(card, lastPlayedCard)){
                        selectedCard = card;
                        return selectedCard;
                    }
                }
            }
//          If we have a Queen and no Jack, we want to sacrifice it to draw out the Kings/ Queens of other players.
            else if(queenCount > 0){
                for (Card card: playerCards){
                    if(rules.getRankCardValue(card) == QUEEN_SCORE && rules.isValidPlay(card, lastPlayedCard)){
                        selectedCard = card;
                        return selectedCard;
                    }
                }
            }
        }
//  Null is returned as neither a Jack nor Queen was found that also met our conditions.
        return selectedCard;
    }

    /** This method goes through the entire hand of cards that the clever player has and counts how many
     * of a specific card we have. It uses the following arguments/ inputs:
     *
     * @param playerCards this is a list of the cards the clever player has
     * @param cardScore this is the card that we want to count (i.e., King, Queen, Jack)
     *
     * @return the number of cards that were found of that score.
     * **/
    public Integer pictureCardCount(ArrayList<Card> playerCards, int cardScore){
        int count = 0;
        for (Card card: playerCards){
            if (rules.getRankCardValue(card) == cardScore){
                count ++;
            }
        }
        return count;

    }

    /** This method goes through the card list of the player and selects the lowest ranked card using
     * the basic strategy. It uses the following inputs/ arguments:
     *
     * @param playerCards is the cards of the current player
     * @param lastPlayedCard is the card of the last player for validity purposes

     * @return the selected card using the basic card strategy
     * */
    public Card minCard(ArrayList<Card> playerCards, Card lastPlayedCard){
        Card minvalcard = null;
        for (Card card : playerCards) {
            if (rules.isValidPlay(card, lastPlayedCard)) {
                if (minvalcard !=null && card !=null){
//                    Find the lowest ranked card.
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
