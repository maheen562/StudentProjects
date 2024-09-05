
import org.junit.Before;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.Scanner;

import static org.junit.Assert.*;
public class SpaceInvaderTest {
    class AlienTestData {
        String type;
        int rowIndex;
        int colIndex;
        String aliveStatus;
        int locationX;
        int locationY;

        public AlienTestData(String type, int rowIndex, int colIndex, String aliveStatus,
                             int locationX, int locationY) {
            this.type = type;
            this.rowIndex = rowIndex;
            this.colIndex = colIndex;
            this.aliveStatus = aliveStatus;
            this.locationX = locationX;
            this.locationY = locationY;
        }
    }

    private ArrayList<AlienTestData> convertFromLogLine(String line) {
        line = line.replace("Alien locations: ", "");
        String[] aliensString = line.split("#");
        ArrayList<AlienTestData> aliensTestData = new ArrayList<>();
        for (String alienString : aliensString) {
            String[] alienData = alienString.split("@");
            String[] indexData = alienData[1].split("-");
            String[] locationData = alienData[3].split("-");
            String type = alienData[0];
            String aliveStatus = alienData[2];
            AlienTestData alienTestData = new AlienTestData(type,
                    Integer.parseInt(indexData[0]), Integer.parseInt(indexData[1]), aliveStatus,
                    Integer.parseInt(locationData[0]), Integer.parseInt(locationData[1]));
            aliensTestData.add(alienTestData);
        }

        return aliensTestData;
    }

    private int[] countOccurences(String mainString, String findString) {
        int lastFoundIndex = -1;
        int lastIndex = 0;
        int count = 0;

        while (lastIndex != -1) {

            lastIndex = mainString.indexOf(findString, lastIndex);

            if (lastIndex != -1) {
                count++;
                lastFoundIndex = lastIndex;
                lastIndex += findString.length();
            }
        }
        return new int[] {count, lastFoundIndex};
    }

    @Test(timeout = 20000)
    public void testSpaceInvaderOriginal() {
        int numberColumns = 4;
        int alien1RowIndex = 2;
        int alien1ColIndex = 1;
        int alien2RowIndex = 1;
        int alien2ColIndex = 1;
        int alien3RowIndex = 0;
        int alien3ColIndex = 1;
        String testOriginal = "properties/test1.properties";
        final Properties properties = PropertiesLoader.loadPropertiesFile(testOriginal);
        SpaceInvader spaceInvader = new SpaceInvader(properties);
        String logResult = spaceInvader.runApp(true);
        assertTrue("alien 1 should be hit",
                logResult.contains(String.format("An alien has been hit.alien@%d-%d", alien1RowIndex, alien1ColIndex)));
        assertTrue("alien 2 should be hit",
                logResult.contains(String.format("An alien has been hit.alien@%d-%d", alien2RowIndex, alien2ColIndex)));
        assertTrue("alien 3 should be hit",
                logResult.contains(String.format("An alien has been hit.alien@%d-%d", alien3RowIndex, alien3ColIndex)));
        assertTrue("aliens should not move fast after 10 shoots for simple level",
                !logResult.contains("Aliens start moving fast"));

        String lastHitMessage = "An alien has been hit.alien@0-1";
        int alien01Index = logResult.indexOf(lastHitMessage);
        String remainingString = logResult.substring(alien01Index + lastHitMessage.length());
        Scanner scanner = new Scanner(remainingString);
        assertTrue(scanner.hasNextLine());
        while (scanner.hasNextLine()) {
            String nextLine = scanner.nextLine();
            if (!nextLine.contains("Alien locations: ")) {
                continue;
            }

            List<AlienTestData> aliensTestData = convertFromLogLine(nextLine);
            int alien1Index = alien1RowIndex * numberColumns + alien1ColIndex;
            int alien2Index = alien2RowIndex * numberColumns + alien2ColIndex;
            int alien3Index = alien3RowIndex * numberColumns + alien3ColIndex;

            assertEquals("Dead", aliensTestData.get(alien1Index).aliveStatus);
            assertEquals("Dead", aliensTestData.get(alien2Index).aliveStatus);
            assertEquals("Dead", aliensTestData.get(alien3Index).aliveStatus);
            assertEquals("Alive", aliensTestData.get(0).aliveStatus);
            break;
        }
    }

    @Test(timeout = 20000)
    public void testSpaceInvaderPowerfulAlien() {
        int numberColumns = 5;
        int alien1RowIndex = 0;
        int alien1ColIndex = 2;
        String testOriginal = "properties/test2.properties";
        final Properties properties = PropertiesLoader.loadPropertiesFile(testOriginal);
        SpaceInvader spaceInvader = new SpaceInvader(properties);
        String logResult = spaceInvader.runApp(true);
        System.out.println("logResult = " + logResult);
        String findStr = String.format("An alien has been hit.powerful@%d-%d", alien1RowIndex, alien1ColIndex);
        int[] lastIndexCount = countOccurences(logResult, findStr);
        assertEquals(5, lastIndexCount[0]);

        String remainingString = logResult.substring(lastIndexCount[1] + findStr.length());
        Scanner scanner = new Scanner(remainingString);
        assertTrue(scanner.hasNextLine());
        while (scanner.hasNextLine()) {
            String nextLine = scanner.nextLine();
            if (!nextLine.contains("Alien locations: ")) {
                continue;
            }

            List<AlienTestData> aliensTestData = convertFromLogLine(nextLine);
            int alien1Index = alien1RowIndex * numberColumns + alien1ColIndex;

            assertEquals("Dead", aliensTestData.get(alien1Index).aliveStatus);
            assertEquals("Alive", aliensTestData.get(0).aliveStatus);
            break;
        }
    }

    @Test(timeout = 20000)
    public void testSpaceInvaderFastMoving() {
        String testOriginal = "properties/test3.properties";
        final Properties properties = PropertiesLoader.loadPropertiesFile(testOriginal);
        SpaceInvader spaceInvader = new SpaceInvader(properties);
        String logResult = spaceInvader.runApp(true);
        System.out.println("logResult = " + logResult);
        String keyMessage = "Aliens start moving fast";

        assertTrue("Aliens should start moving fast", logResult.contains(keyMessage));

        int keyMessageIndex = logResult.indexOf(keyMessage);
        String remainingString = logResult.substring(keyMessageIndex + keyMessage.length());
        Scanner scanner = new Scanner(remainingString);

        assertTrue(scanner.hasNextLine());
        String firstLine = null;
        String secondLine = null;
        while (scanner.hasNextLine()) {
            String nextLine = scanner.nextLine();
            if (!nextLine.contains("Alien locations: ")) {
                continue;
            }
            if (firstLine == null) {
                firstLine = nextLine;
                continue;
            }
            if (secondLine == null && !nextLine.equals(firstLine)) {
                secondLine = nextLine;
                break;
            }
        }

        List<AlienTestData> aliens1TestData = convertFromLogLine(firstLine);
        List<AlienTestData> aliens2TestData = convertFromLogLine(secondLine);
        AlienTestData alien1TestData = aliens1TestData.get(0);
        AlienTestData alien2TestData = aliens2TestData.get(0);
        int locationDiff = Math.abs(alien1TestData.locationX - alien2TestData.locationX);

        assertEquals(10, locationDiff);
    }
}
