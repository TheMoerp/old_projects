import java.util.*;
/**
 * Beschreiben Sie hier die Klasse Field.
 * 
 * @author (Ihr Name) 
 * @version (eine Versionsnummer oder ein Datum)
 */
public class game
{
    Scanner s = new Scanner(System.in);
    final int SHIP_NUMBER = 1;
    final int SHIP_2_NUMBER = 1;
    final int SHIP_3_NUMBER = 0;
    final int SHIP_4_NUMBER = 0;
    int xK, yK, shipLength, playerWin;
    char shipDirection;
    int[][] FIELD1;
    int[][] FIELD2;
    int failCeck = 0;
    char[] border = {'A','B','C','D','E','F','G','H','I','J'};
    int player = 1;
    int shipSinkHelper = 0;

    public game()
    {
        field();
        battleFunktion();
    }
    public void field()
    {
        shipNumberTest();
        FIELD1 = new int [10][10];
        FIELD2 = new int [10][10];
        for(int i = 0; i < 2; i++)
        {
            do
            {
                System.out.println("");
                System.out.print("Spieler ");
                System.out.print(player);
                System.out.println(" muss jetzt seine Schiffe positionieren");
                failCeck = 0;
                for(int shipCounter = 0; shipCounter < SHIP_NUMBER && failCeck == 0; shipCounter++)
                {
                    shipattributes();
                    shipInit();
                }
            }
            while(failCeck == 1);
            showField();
            player = 2;
            shipSinkHelper = 0;
        }
        player = 1;
    }

    public void shipNumberTest()
    {
        if(SHIP_2_NUMBER + SHIP_3_NUMBER + SHIP_4_NUMBER != SHIP_NUMBER)
        {
            System.out.println("Die Anzahl der zu setzenden Schiffe stimmt nicht mit der Anzahl der Schiffstypen überein. Bitte ändern sie dies und starten sie das Programm neu!");
            do
            {
            }
            while(true);
        }
    }

    public void shipattributes()
    {
        shipSinkHelper++;
        int caseNumber = 0;
        do{
            System.out.print("Geben sie die x-Koordinate des Schiffes ein: ");
            xK = s.nextInt();
            System.out.println("");
            System.out.print("Geben sie die y-Koordinate des Schiffes ein: ");
            yK = s.nextInt();
            System.out.println("");
            System.out.println("");
            System.out.print("Verfügbare Schiffe: 2er: "+ SHIP_2_NUMBER +", 3er: "+ SHIP_3_NUMBER + ", 4er: "+ SHIP_4_NUMBER);
            System.out.println(""); 
            System.out.print("Geben sie die Länge des Schiffes welches sie setzen möchten ein: ");
            shipLength = s.nextInt();
            do{
                failCeck = 0;
                System.out.println("");
                System.out.print("Geben sie, um die Ausrichtung des Schiffes festzugelegen, für Wagerecht ein w und für Senkrecht ein s ein: ");
                shipDirection = s.next().charAt(0);
                System.out.println("");
                if(shipDirection == 'w' || shipDirection == 's')
                {
                    if(shipDirection == 'w')
                    {
                        caseNumber = 1;
                    }
                    else
                    {
                        caseNumber = 2;
                    }
                }
                else
                {
                    System.out.println("Dieses Zeichen wurde nicht erkannt!");
                    failCeck = 1;
                }
            }
            while(failCeck == 1);
            switch(caseNumber)
            {
                case 1:
                if(xK + shipLength > 9)
                {
                    System.out.println("Das Schiff übertritt, auf der rechten oder linken seite, den Rand!");
                    failCeck = 1;
                }
                case 2:
                if(yK + shipLength > 9)
                {
                    System.out.println("Das Schiff übertritt, oben oder unten, den Rand!");
                    failCeck = 1;
                }
            }
            System.out.println("");
        }
        while(failCeck == 1);
        System.out.println("");
        System.out.println("Das Schiff wurde positioniert");
        System.out.println("");
    }

    public void shipInit()
    {
        for(int fieldPos = 0; fieldPos < shipLength; fieldPos++)
        {
            if(shipDirection == 's')
            {   
                if(player == 1)
                {
                    if(FIELD1[yK - 1 + fieldPos][xK - 1] == 0)
                    {
                        FIELD1[yK - 1 + fieldPos][xK - 1] = shipSinkHelper;
                    }
                    else
                    {
                        failCeck = 1;
                    }
                }
                else
                {
                    if(FIELD2[yK - 1 + fieldPos][xK - 1] == 0)
                    {
                        FIELD2[yK - 1 + fieldPos][xK - 1] = shipSinkHelper;
                    }
                    else
                    {
                        failCeck = 1;
                    }
                }
            }
            else
            {
                if(player == 1)
                {
                    if(FIELD1[yK - 1][xK - 1 + fieldPos] == 0)
                    {
                        FIELD1[yK - 1][xK - 1 + fieldPos] = shipSinkHelper;
                    }
                    else
                    {
                        failCeck = 1;
                    }
                }
                else
                {
                    if(FIELD2[yK - 1][xK - 1 + fieldPos] == 0)
                    {
                        FIELD2[yK - 1][xK - 1 + fieldPos] = shipSinkHelper;
                    }
                    else
                    {
                        failCeck = 1;
                    }
                }
            } 
        }
        if(failCeck == 1)
        {
            System.out.println("Die Positionen ihrer Schiffe überschneiden sich!");
        }
        System.out.println("");
    }

    public void showField()
    {
        for (int i=0;i<11;i++)
        {
            System.out.print(i + " ");
        }
        System.out.println("");
        for(int i=0;i<10;i++)
        { 
            System.out.print(border[i] + " ");
            for(int j=0;j<10;j++)
            { 
                if(player == 1)
                {
                    System.out.print(FIELD1[i][j] + " ");
                }
                else
                {
                    System.out.print(FIELD2[i][j] + " ");
                }
            }
            System.out.println("");
        }
    }

    public void battleFunktion()
    {
        do
        {
            fire();
            if(player == 1)
            {
                player = 2;
            }
            else
            {
                player = 1;
            }
        }
        while(playerWin == 0);
        if(playerWin == 1)
        {
            System.out.println("SPIELER "+ player +" HAT GEWONNEN!");
        }
    }

    public void fire()
    {
        int shipSearch = 0;
        System.out.println("Spieler "+ player + " muss jetzt schießen!");
        System.out.println("");
        System.out.print("Gebe die x Koordiante ein: ");
        xK = s.nextInt();
        System.out.println("");
        System.out.print("Gebe die y Koordiante ein: ");
        yK = s.nextInt();
        System.out.println("");
        System.out.println("Sie schießen auf die Koordinaten ( "+ xK +" | "+ yK +" ).");
        if(player == 2)
        {
            if(FIELD1[xK][yK] != 0)
            {
                System.out.println("Sie haben ein feindliches Schiff getroffen!");
                shipSinkHelper = FIELD1[xK][yK];
                FIELD1[xK][yK] = 0;
                playerWin = 1;
                for(int i=0;i<10 && shipSearch == 0;i++)
                { 
                    for(int j=0;j<10 && shipSearch == 0;j++)
                    { 
                        if(FIELD1[i][j] == shipSinkHelper)
                        {
                            shipSearch = 1;
                        }
                        if(FIELD1[i][j] != 0)
                        {
                            playerWin = 0;
                        }
                    }
                }
                if(shipSearch == 0)
                {
                    System.out.println("Sie haben ein Schiff von Spieler 2 versenkt!");
                }
            }
            else
            {
                System.out.println("Sie haben leider nicht getroffen.");
            }
        }
        else
        {
            if(FIELD2[xK][yK] != 0)
            {
                System.out.println("Sie haben ein feindliches Schiff getroffen!");
                shipSinkHelper = FIELD2[xK][yK];
                FIELD2[xK][yK] = 0;
                playerWin = 2;
                for(int i=0;i<10 && shipSearch == 0;i++)
                { 
                    for(int j=0;j<10 && shipSearch == 0;j++)
                    { 
                        if(FIELD2[i][j] == shipSinkHelper)
                        {
                            shipSearch = 1;
                        }
                        if(FIELD2[i][j] != 0)
                        {
                            playerWin = 0;
                        }
                    }
                }
                if(shipSearch == 0)
                {
                    System.out.println("Sie haben ein Schiff von Spieler 1 versenkt!");
                }
            }
            else
            {
                System.out.println("Sie haben leider nicht getroffen.");
            }
        }

    }
}
