// CountingUpRules.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.*;

import java.util.ArrayList;
import java.util.HashMap;

public class CountingUpRules {
    private HashMap<String, Integer> cardsPlayed = new HashMap<>();

    private static CountingUpGame game; /*This creates a static instance of a game*/

    private static boolean keyPressed = false;/*This is used to determine if the '\n' key has been pressed,
                                                    determining an interaction has taken place
                                                    static because there will be only one instance of a keypressed at
                                                    a time*/
    private static Card selected = null; /*This is the card that is selected by the current player.
                                                        This is made static because there will only be one selected
                                                        card at a time*/

    private static boolean aceClubPlayed; /*Checks if ace of clubs is played. static because there is only
                                                           on ace of Club in the fame*/


    public CountingUpRules(CountingUpGame countingUpGame){
        this.game = countingUpGame;
    }


    /**
     * Retrieves (gets) the rank of a card.
     *@param playerCard is the current card for which the rank needs to be determined
     *
     * @return the rank of the card.
     */
    public int getRankCardValue(Card playerCard){
        CountingUpGame.Rank cardRank = (CountingUpGame.Rank) playerCard.getRank();

        return cardRank.getRankCardValue();

    }

    /**
     * This updates the keyPressed value whenever a desired key is pressed.
     * It detects the key from the CountingUpGame class.
     * */
    public void UpdateKeyPressed(){
        this.keyPressed = !keyPressed;
    }


    /**
     * Retrieves (gets) the keyPressed value.
     *
     * @return the keyPressed value.
     */
    public boolean getKeyPressed(){return keyPressed;}

    public CountingUpRules(){}

    /**
     * Checks if the Ace of clubs has been played
     * @return true or false depending on it the ace of clubs has been played
     * */
    public boolean CheckAceClubPlayed(){
        return game.getAceClubsPlayed();
    }


    /**
     * @param card current card being drawn
     * @param lastPlayedCard the last card that was drawn
     * It checks for the validity of the card based on the last played card.
     * @return true If the current card is the same rank and
     *                       greater in value or is it is the same value but a different rank
     * @return false it neither of the above stated.
     * */
    public boolean isValidPlay(Card card, Card lastPlayedCard) {
//        If we're at the start of a new round, any card can be picked.
        if(game.getRoundEnded()){
//            Now we change roundEnded to false.
            game.setRoundEnded();
            return true;
        }
//        The turn is being passed.
        if(card == null) {
            return false;
        }
//        No previous card has been played, this is the first move in the game so, it can be anything.
        if (lastPlayedCard == null) {
            return true;
        }

        CountingUpGame.Rank cardRank = (CountingUpGame.Rank) card.getRank();
        CountingUpGame.Suit cardSuit = (CountingUpGame.Suit) card.getSuit();
        CountingUpGame.Rank lastPlayedRank = (CountingUpGame.Rank) lastPlayedCard.getRank();
        CountingUpGame.Suit lastPlayedSuit = (CountingUpGame.Suit) lastPlayedCard.getSuit();

        // Check if the card has the same rank but a different suit
        if (cardRank == lastPlayedRank && cardSuit != lastPlayedSuit) {
            return true;
        }

        // Check if the card has the same suit but a higher rank
        if(cardSuit == lastPlayedSuit && (cardRank.getRankCardValue() > lastPlayedRank.getRankCardValue())){
            return true;
        }

        return false;
    }

    /**
     * @param playerCards cards of the player
     * draws the ace of cards if available in the current set of player cards.
     * It first checks if the Ace of clubs has been played. If played by this method, it sets the isAceClubPlayed to
     *                    true
     * @return ace card if it is in the player's stack, otherwise null is returned.
     * */
    public Card drawAceClubCard(ArrayList<Card> playerCards){
        if (!aceClubPlayed) {
            for (Card card : playerCards) {
                if (card.getSuit() == CountingUpGame.Suit.CLUBS && card.getRank() == CountingUpGame.Rank.ACE) {
                     // Play the valid card
                    this.aceClubPlayed = true;
                    return card;
                }
            }
        }
        return null;
    }

    /**
     * Sets the current status in the game.
     * @param text is the text that will be displayed in the status
     *
     * Nothing is returned.
     * */
    public void setStatus(String text){
        game.setStatusText(text);
    }

    /**
     * Retrieves (gets) the card that has been selected.
     *
     * @return the selected card.
     */
    public Card getSelectedCard(){
        return selected;
    }

    /**
     * Sets the value of selected to be the card that has been selected.
     * @param selected this is the card that has been selected and what must be the new
     *                 value of selected.
     *
     * Nothing is returned.
     */
    public void setSelectedCard(Card selected){
        this.selected = selected;
    }

    /**
     * Retrieves/ gets the thinking time allocated in the game.
     *
     * @return the thinking time that is allocated in the game.
     * */
    public int gameThinkingTime(){
        return game.getThinkingTime();
    }

    /**
     * Retrieves (gets) the dictionary that clever player is using to
     * record the cards that all the other players have played.
     *
     * @return the dictionary.
     */
    public HashMap getCardsPlayed(){return cardsPlayed;}


    /**
     * Updates the dictionary that the clever player is using to keep track of
     * which cards have been played by the other players.
     *
     * @param card This is the card that was just played and so it's counter is
     *             either initialised or incremented.
     *
     * Nothing is returned.
     */
    public void setCardsPlayed(Card card){
        // If the card is already in the map, get its current count and increment by 1
        if (cardsPlayed.containsKey(card.getRank())) {
            int currentCount = cardsPlayed.get(card.getRank());
            cardsPlayed.put(String.valueOf(card.getRank()), currentCount + 1);
        } else {
            // If the card isn't in the map yet, initialize its count to 1
            cardsPlayed.put(String.valueOf(card.getRank()), 1);
        }
    }
}

