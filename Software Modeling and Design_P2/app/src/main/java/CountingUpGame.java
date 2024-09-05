// CountingUpGame.java
// Thursday 2.15pm Team 4; Alleena Haider Waseem (1204035), Mahamithra Sivagnanam (1225270),
// Maheen Abdul Khaliq Khan (1193813)

import ch.aplu.jcardgame.*;
import ch.aplu.jgamegrid.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.*;
import java.util.List;
import java.util.stream.Collectors;

@SuppressWarnings("serial")
public class CountingUpGame extends CardGame implements GGKeyListener {
    private boolean roundEnded = false;
    private Card lastPlayedCard = null;
    private boolean aceClubsPlayed = false;
    private Card selected;
    private CountingUpRules gameRules = new CountingUpRules(this);
    public enum Suit {
        SPADES ("S"), HEARTS ("H"), DIAMONDS ("D"), CLUBS ("C");
        private String suitShortHand = "";
        Suit(String shortHand) {
            this.suitShortHand = shortHand;
        }

        public String getSuitShortHand() {
            return suitShortHand;
        }
    }


    public enum Rank {
        // Reverse order of rank importance (see rankGreater() below)
        ACE (1, 10), KING (13, 10), QUEEN (12, 10),
        JACK (11, 10), TEN (10, 10), NINE (9, 9),
        EIGHT (8, 8), SEVEN (7, 7), SIX (6, 6),
        FIVE (5, 5), FOUR (4, 4), THREE (3, 3),
        TWO (2, 2);

        private int rankCardValue = 1;
        private int scoreCardValue = 1;
        Rank(int rankCardValue, int scoreCardValue) {
            this.rankCardValue = rankCardValue;
            this.scoreCardValue = scoreCardValue;
        }

        public int getRankCardValue() {
            return rankCardValue;
        }

        public int getScoreCardValue() { return scoreCardValue; }

        public String getRankCardLog() {
            return String.format("%d", rankCardValue);
        }
    }

    static public final int seed = 30008;
    public static final Random random = new Random(seed);
    private Properties properties;
    private StringBuilder logResult = new StringBuilder();
    private List<List<String>> playerAutoMovements = new ArrayList<>();

    private final String version = "1.0";
    public final int nbPlayers = 4;
    public final int nbStartCards = 13;
    private final int handWidth = 400;
    private final int trickWidth = 40;
    private final Deck deck = new Deck(Suit.values(), Rank.values(), "cover");
    private final Location[] handLocations = {
            new Location(350, 625),
            new Location(75, 350),
            new Location(350, 75),
            new Location(625, 350)
    };
    private final Location[] scoreLocations = {
            new Location(575, 675),
            new Location(25, 575),
            new Location(575, 25),
            // new Location(650, 575)
            new Location(575, 575)
    };
    private Actor[] scoreActors = {null, null, null, null};

    private String[] playerTypes = new String[]{""," ","",""};
    private final Location trickLocation = new Location(350, 350);
    private final Location textLocation = new Location(350, 450);
    private int thinkingTime = 2000;
    private int delayTime = 600;
    public CardListener cardListener;
    private Hand[] hands;
    private Location hideLocation = new Location(-500, -500);
    private int[] scores = new int[nbPlayers];
    private boolean passSelected = false;
    private int[] autoIndexHands = new int [nbPlayers];
    private boolean isAuto = false;

    private Font bigFont = new Font("Arial", Font.BOLD, 36);

    private void initScore() {
        for (int i = 0; i < nbPlayers; i++) {
            // scores[i] = 0;
            String text = "[" + String.valueOf(scores[i]) + "]";
            scoreActors[i] = new TextActor(text, Color.WHITE, bgColor, bigFont);
            addActor(scoreActors[i], scoreLocations[i]);
        }
    }

    private void calculateScoreEndOfRound(int player, List<Card> cardsPlayed) {
        int totalScorePlayed = 0;
        for (Card card: cardsPlayed) {
            Rank rank = (Rank) card.getRank();
            totalScorePlayed += rank.getScoreCardValue();
        }
        scores[player] += totalScorePlayed;
    }

    private void calculateNegativeScoreEndOfGame(int player, List<Card> cardsInHand) {
        int totalScorePlayed = 0;
        for (Card card: cardsInHand) {
            Rank rank = (Rank) card.getRank();
            totalScorePlayed -= rank.getScoreCardValue();
        }
        scores[player] += totalScorePlayed;
    }

    private void updateScore(int player) {
        removeActor(scoreActors[player]);
        int displayScore = scores[player] >= 0 ? scores[player] : 0;
        String text = "P" + player + "[" + String.valueOf(displayScore) + "]";
        scoreActors[player] = new TextActor(text, Color.WHITE, bgColor, bigFont);
        addActor(scoreActors[player], scoreLocations[player]);
    }

    private void initScores() {
        for (int i = 0; i < nbPlayers; i++) {
            scores[i] = 0;
        }
    }

    @Override
    public boolean keyPressed(KeyEvent keyEvent) {
        if (keyEvent.getKeyChar() == '\n') {
            gameRules.UpdateKeyPressed();
            passSelected = true;
            return true;
        }
        return false;
    }

    @Override
    public boolean keyReleased(KeyEvent keyEvent) {
        return false;
    }

    private void initGame() {
        hands = new Hand[nbPlayers];
        for (int i = 0; i < nbPlayers; i++) {
            hands[i] = new Hand(deck);
        }
        dealingOut(hands, nbPlayers, nbStartCards);
        for (int i = 0; i < nbPlayers; i++) {
            hands[i].sort(Hand.SortType.SUITPRIORITY, false);
        }
        // Set up human player for interaction
        /*CardListener */cardListener = new CardAdapter()  // Human Player plays card
        {
            public void leftDoubleClicked(Card card) {
                selected = card;
                gameRules.setSelectedCard(card);

                hands[0].setTouchEnabled(false);
            }
        };
        hands[0].addCardListener(cardListener);
        // graphics
        RowLayout[] layouts = new RowLayout[nbPlayers];
        for (int i = 0; i < nbPlayers; i++) {
            layouts[i] = new RowLayout(handLocations[i], handWidth);
            layouts[i].setRotationAngle(90 * i);
            // layouts[i].setStepDelay(10);
            hands[i].setView(this, layouts[i]);
            hands[i].setTargetArea(new TargetArea(trickLocation));
            hands[i].draw();
        }
    }


    // return random Enum value
    public static <T extends Enum<?>> T randomEnum(Class<T> clazz) {
        int x = random.nextInt(clazz.getEnumConstants().length);
        return clazz.getEnumConstants()[x];
    }

    // return random Card from ArrayList
    public static Card randomCard(ArrayList<Card> list) {
        int x = random.nextInt(list.size());
        return list.get(x);
    }

    private Rank getRankFromString(String cardName) {
        String rankString = cardName.substring(0, cardName.length() - 1);
        Integer rankValue = Integer.parseInt(rankString);

        for (Rank rank : Rank.values()) {
            if (rank.getRankCardValue() == rankValue) {
                return rank;
            }
        }

        return Rank.ACE;
    }

    private Suit getSuitFromString(String cardName) {
        String suitString = cardName.substring(cardName.length() - 1);

        for (Suit suit : Suit.values()) {
            if (suit.getSuitShortHand().equals(suitString)) {
                return suit;
            }
        }
        return Suit.CLUBS;
    }

    private Card getCardFromList(List<Card> cards, String cardName) {
        Rank cardRank = getRankFromString(cardName);
        Suit cardSuit = getSuitFromString(cardName);
        for (Card card: cards) {
            if (card.getSuit() == cardSuit
                    && card.getRank() == cardRank) {
                return card;
            }
        }

        return null;
    }

    private void dealingOut(Hand[] hands, int nbPlayers, int nbCardsPerPlayer) {
        Hand pack = deck.toHand(false);
        int[] cardsDealtPerPlayer = new int[nbPlayers];

        for (int i = 0; i < nbPlayers; i++) {
            String initialCardsKey = "players." + i + ".initialcards";
            String initialCardsValue = properties.getProperty(initialCardsKey);
            if (initialCardsValue == null) {
                continue;
            }
            String[] initialCards = initialCardsValue.split(",");
            for (String initialCard: initialCards) {
                if (initialCard.length() <= 1) {
                    continue;
                }
                Card card = getCardFromList(pack.getCardList(), initialCard);
                if (card != null) {
                    card.removeFromHand(false);
                    hands[i].insert(card, false);
                }
            }
        }

        for (int i = 0; i < nbPlayers; i++) {
            int cardsToDealt = nbCardsPerPlayer - hands[i].getNumberOfCards();
            for (int j = 0; j < cardsToDealt; j++) {
                if (pack.isEmpty()) return;
                Card dealt = randomCard(pack.getCardList());
                dealt.removeFromHand(false);
                hands[i].insert(dealt, false);
            }
        }
    }

    private int playerIndexWithAceClub() {
        for (int i = 0; i < nbPlayers; i++) {
            Hand hand = hands[i];
            List<Card> cards = hand.getCardsWithRank(Rank.ACE);
            if (cards.size() == 0) {
                continue;
            }
            for (Card card: cards) {
                if (card.getSuit() == Suit.CLUBS) {
                    return i;
                }
            }
        }

        return 0;
    }

    private void addCardPlayedToLog(int player, Card selectedCard) {
        if (selectedCard == null) {
            logResult.append("P" + player + "-SKIP,");
        } else {
            Rank cardRank = (Rank) selectedCard.getRank();
            Suit cardSuit = (Suit) selectedCard.getSuit();
            ArrayList<Card> playedCards = new ArrayList<>();
            playedCards.add(selectedCard);
            logResult.append("P" + player + "-" + cardRank.getRankCardLog() + cardSuit.getSuitShortHand() + ",");
        }
    }

    private void addRoundInfoToLog(int roundNumber) {
        logResult.append("Round" + roundNumber + ":");
    }

    private void addEndOfRoundToLog() {
        logResult.append("Score:");
        for (int i = 0; i < scores.length; i++) {
            logResult.append(scores[i] + ",");
        }
        logResult.append("\n");
    }

    private void addEndOfGameToLog(List<Integer> winners) {
        logResult.append("EndGame:");
        for (int i = 0; i < scores.length; i++) {
            logResult.append(scores[i] + ",");
        }
        logResult.append("\n");
        logResult.append("Winners:" + String.join(", ", winners.stream().map(String::valueOf).collect(Collectors.toList())));
    }

    private void playGame() {
        // End trump suit
        Hand playingArea = null;
        int winner = 0;
        int roundNumber = 1;
        for (int i = 0; i < nbPlayers; i++) updateScore(i);
        boolean isContinue = true;
        int skipCount = 0;
        List<Card>cardsPlayed = new ArrayList<>();
        playingArea = new Hand(deck);
        addRoundInfoToLog(roundNumber);

        int nextPlayer = playerIndexWithAceClub();
//        Card lastPlayedCard = null; // Initialize lastPlayedCard to null
        lastPlayedCard = null;
        while(isContinue) {
            selected = null;
            boolean finishedAuto = false;
            if (isAuto) {
                finishedAuto = isFinishedAuto(nextPlayer, finishedAuto);
            }
            if (!isAuto || finishedAuto){
//                If the game is not auto or the auto is finished then we do this.
//                lastPlayedCard = null;
                handlePlayerTurn(nextPlayer, lastPlayedCard);
                aceClubsPlayed = true;
            }

            // Follow with selected card

            playingArea.setView(this, new RowLayout(trickLocation, (playingArea.getNumberOfCards() + 2) * trickWidth));
            playingArea.draw();
            addCardPlayedToLog(nextPlayer, selected);
            if (selected != null) {
                skipCount = 0;
                cardsPlayed.add(selected);
                selected.setVerso(false);  // In case it is upside down
                lastPlayedCard = selected;
                selected.transfer(playingArea, true); // transfer to trick (includes graphic effect)
                delay(delayTime);
            } else {
                skipCount++;
            }

            if (skipCount == nbPlayers - 1) {
                playingArea.setView(this, new RowLayout(hideLocation, 0));
                playingArea.draw();
                winner = (nextPlayer + 1) % nbPlayers;
                lastPlayedCard = null;
                skipCount = 0;
                calculateScoreEndOfRound(winner, cardsPlayed);
                updateScore(winner);
//                round has ended, set round ended to true.
                setRoundEnded();
                addEndOfRoundToLog();
                roundNumber++;
                addRoundInfoToLog(roundNumber);
                cardsPlayed = new ArrayList<>();
                delay(delayTime);
                playingArea = new Hand(deck);
            }

            isContinue = hands[0].getNumberOfCards() > 0 && hands[1].getNumberOfCards() > 0 &&
                    hands[2].getNumberOfCards() > 0 && hands[3].getNumberOfCards() > 0;

            if (!isContinue) {
                winner = nextPlayer;
                calculateScoreEndOfRound(winner, cardsPlayed);
                addEndOfRoundToLog();
            } else {
                nextPlayer = (nextPlayer + 1) % nbPlayers;
            }
            delay(delayTime);
        }

        calculateFinalDisplayScores();
    }

    private void calculateFinalDisplayScores() {
        for (int i = 0; i < nbPlayers; i++) {
            calculateNegativeScoreEndOfGame(i, hands[i].getCardList());
            updateScore(i);
        }
    }


    private boolean isFinishedAuto(int nextPlayer, boolean finishedAuto) {
        int nextPlayerAutoIndex = autoIndexHands[nextPlayer];
        List<String> nextPlayerMovement = playerAutoMovements.get(nextPlayer);
        String nextMovement = "";
        if (nextPlayerMovement.size() > nextPlayerAutoIndex) {
            nextMovement = nextPlayerMovement.get(nextPlayerAutoIndex);
            nextPlayerAutoIndex++;

            autoIndexHands[nextPlayer] = nextPlayerAutoIndex;
            Hand nextHand = hands[nextPlayer];

            if (nextMovement.equals("SKIP")) {
                setStatusText("Player " + nextPlayer + " skipping...");
                delay(thinkingTime);
                selected = null;
            } else {
                setStatusText("Player " + nextPlayer + " thinking...");
                delay(thinkingTime);
                selected = getCardFromList(nextHand.getCardList(), nextMovement);
                if (selected.getSuit() == CountingUpGame.Suit.CLUBS && selected.getRank() == CountingUpGame.Rank.ACE){
                    aceClubsPlayed = true;
                }
                lastPlayedCard = selected;
            }
        } else {
            finishedAuto = true;
        }
        return finishedAuto;
    }

    private void setupPlayerAutoMovements() {
        String player0AutoMovement = properties.getProperty("players.0.cardsPlayed");
        String player1AutoMovement = properties.getProperty("players.1.cardsPlayed");
        String player2AutoMovement = properties.getProperty("players.2.cardsPlayed");
        String player3AutoMovement = properties.getProperty("players.3.cardsPlayed");

        String[] playerMovements = new String[] {"", "", "", ""};
        if (player0AutoMovement != null) {
            playerMovements[0] = player0AutoMovement;
        }

        if (player1AutoMovement != null) {
            playerMovements[1] = player1AutoMovement;
        }

        if (player2AutoMovement != null) {
            playerMovements[2] = player2AutoMovement;
        }

        if (player3AutoMovement != null) {
            playerMovements[3] = player3AutoMovement;
        }

        for (int i = 0; i < playerMovements.length; i++) {
            String movementString = playerMovements[i];
            List<String> movements = Arrays.asList(movementString.split(","));
            playerAutoMovements.add(movements);
        }
    }

    /**It gets the player types from the properties file*/
    public void getPlayerTypes(){
        String player0Type = (properties.getProperty("players.0"));
        String player1Type = (properties.getProperty("players.1"));
        String player2Type = (properties.getProperty("players.2"));
        String player3Type = (properties.getProperty("players.3"));

        if (player0Type != null) {
            playerTypes[0] = player0Type;
        }
        if (player1Type != null) {
            playerTypes[1] = player1Type;
        }
        if (player2Type != null) {
            playerTypes[2] = player2Type;
        }
        if (player0Type != null) {
            playerTypes[3] = player3Type;
        }
    }

    public String runApp() {
        setTitle("CountingUpGame (V" + version + ") Constructed for UofM SWEN30006 with JGameGrid (www.aplu.ch)");
        setStatusText("Initializing...");
        initScores();
        initScore();
        addKeyListener(this);
        setupPlayerAutoMovements();
        getPlayerTypes();
        initGame();
        playGame();

        for (int i = 0; i < nbPlayers; i++) updateScore(i);
        int maxScore = 0;
        for (int i = 0; i < nbPlayers; i++) if (scores[i] > maxScore) maxScore = scores[i];
        List<Integer> winners = new ArrayList<Integer>();
        for (int i = 0; i < nbPlayers; i++) if (scores[i] == maxScore) winners.add(i);
        String winText;
        if (winners.size() == 1) {
            winText = "Game over. Winner is player: " +
                    winners.iterator().next();
        } else {
            winText = "Game Over. Drawn winners are players: " +
                    String.join(", ", winners.stream().map(String::valueOf).collect(Collectors.toList()));
        }
        addActor(new Actor("sprites/gameover.gif"), textLocation);
        setStatusText(winText);
        refresh();
        addEndOfGameToLog(winners);

        return logResult.toString();
    }

    public CountingUpGame(Properties properties) {
        super(700, 700, 30);
        this.properties = properties;
        isAuto = Boolean.parseBoolean(properties.getProperty("isAuto"));
        if (isAuto) {
            thinkingTime = 50;
            delayTime = 10;
        }
    }

    /**
     * Retrieves (gets) the thinking time allocated.
     *
     * @return the thinking time.
     */
    public int getThinkingTime(){
        return thinkingTime;
    }

    /**This method handles the logic for each player's turn.
     *
     * @param nextPlayer :This is the player whose turn it is.
     * @param lastPlayedCard : This is the last card that was played.
     *
     * Nothing is returned.**/
    private void handlePlayerTurn(int nextPlayer, Card lastPlayedCard) {
//        Logic if the player is human.
        if (playerTypes[nextPlayer].equals("human") ) {
            //hands[nextPlayer].setTouchEnabled(true);
            HumanCardStrategy humanCard = new HumanCardStrategy();
            selected = humanCard.playCard(hands[nextPlayer],lastPlayedCard,nextPlayer);
        }
//        Robot player logic.
        else {
            if(playerTypes[nextPlayer].equals("basic") ){
                BasicCardStrategy basicCard = new BasicCardStrategy();
                selected = basicCard.playCard(hands[nextPlayer],lastPlayedCard,nextPlayer);
            }
            else if(playerTypes[nextPlayer].equals( "clever")){
                CleverCardStrategy cleverCard = new CleverCardStrategy();
                selected = cleverCard.playCard(hands[nextPlayer],lastPlayedCard,nextPlayer);

            }
            else{
                RandomCardStrategy randomCard = new RandomCardStrategy();
                selected = randomCard.playCard(hands[nextPlayer],lastPlayedCard,nextPlayer);
            }
        }
    }


    /**
     * Retrieves (gets) the current status of roundEnded.
     *
     * @return true if the round has ended, otherwise false.
     */
    public boolean getRoundEnded(){
        return roundEnded;
    }

    /**
     * Retrieves (gets) the current status of aceClubsPlayed.
     *
     * @return true if the ace of clubs has been played, otherwise false.
     */
    public boolean getAceClubsPlayed(){
        return aceClubsPlayed;
    }

    /**
     * Sets the roundEnded status by inverting the provided value.
     * If the value of roundEnded is true, roundEnded will be set to
     * false, and vice versa.
     *
     */
    public void setRoundEnded(){
        this.roundEnded = !this.roundEnded;
    }


}