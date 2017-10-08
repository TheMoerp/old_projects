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
    final int FIELD_LENGTH = 10;
    final int SHIP2_NUMBER = 1;
    final int SHIP3_NUMBER = 1;
    final int SHIP4_NUMBER = 1;
    final int SHIP_NUMBER = SHIP2_NUMBER + SHIP3_NUMBER + SHIP4_NUMBER;

    final int SHIP2_CODE = 0;
    final int SHIP3_CODE = 1;
    final int SHIP4_CODE = 2;

    // erster Index: Spieler, zweiter Index: Schiffsart
    int[][] shipNumbers = {{SHIP2_NUMBER, SHIP3_NUMBER, SHIP4_NUMBER}, {SHIP2_NUMBER, SHIP3_NUMBER, SHIP4_NUMBER}};

    int xK, yK, shipLength, playerWin;
    char shipDirection;
    int[][] field0;
    int[][] field1;
    boolean failCeck = false;
    char[] border = {'A','B','C','D','E','F','G','H','I','J'};
    int shipSinkHelper = 0;

    public game()
    {
        field();
        battleFunktion();
    }

    public int visualPlayerNumber(int internalPlayerNumber)
    {
        return internalPlayerNumber + 1;
    }

    public void field()
    {
        shipSinkHelper = 0;
        int curPlayer = 0;
        field0 = new int [FIELD_LENGTH][FIELD_LENGTH];
        field1 = new int [FIELD_LENGTH][FIELD_LENGTH];
        for(int i = 0; i < 2; i++)
        {
            do
            {
                shipNumbers[curPlayer][0] = SHIP2_NUMBER;
                shipNumbers[curPlayer][1] = SHIP3_NUMBER;
                shipNumbers[curPlayer][2] = SHIP4_NUMBER;
                System.out.println();
                System.out.println("- Spieler "+ visualPlayerNumber(curPlayer) +" muss jetzt seine Schiffe positionieren -");
                System.out.println();
                failCeck = false;
                for(int shipCounter = 0; shipCounter < SHIP_NUMBER && failCeck == false; shipCounter++)
                {
                    shipattributes(curPlayer);
                    shipInit(curPlayer);
                }
            }
            while(failCeck);
            showField(curPlayer);
            curPlayer = 1;
            shipSinkHelper = 0;
        }
        curPlayer = 0;
    }

    public boolean remainingShips(int curPlayer)
    {
        boolean localShipFail = false;

        if (shipNumbers[curPlayer][shipLength - 2] == 0) 
        {
            localShipFail = true;
        } 
        else 
        {
            shipNumbers[curPlayer][shipLength - 2] --;
        }

        return localShipFail;
    }

    public void shipattributes(int curPlayer)
    {
        int caseNumber = 0;
        shipSinkHelper++;
        do{
            System.out.print("Geben sie die x-Koordinate des Schiffes ein: ");
            xK = s.nextInt();
            System.out.println();
            System.out.print("Geben sie die y-Koordinate des Schiffes ein: ");
            yK = s.nextInt();
            System.out.println();

            System.out.println("Verfügbare Schiffe: 2er: "+ shipNumbers[curPlayer][0] +", 3er: "+ shipNumbers[curPlayer][1] + ", 4er: "+ shipNumbers[curPlayer][2]);

            boolean shipFail;
            do
            {
                System.out.println(); 
                System.out.print("Geben sie die Länge des Schiffes welches sie setzen möchten ein: ");
                shipLength = s.nextInt();
                shipFail = remainingShips(curPlayer);
                if(shipFail)
                {
                    System.out.println("Diese Art von Schiffen ist nicht mehr verfügbar!");
                }
            }
            while(shipFail);
            do{
                failCeck = false;
                System.out.println();
                System.out.print("Geben sie, um die Ausrichtung des Schiffes festzugelegen, für Wagerecht ein w und für Senkrecht ein s ein: ");
                shipDirection = s.next().charAt(0);
                System.out.println();
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
                    failCeck = true;
                }
            }
            while(failCeck);
            switch(caseNumber)
            {
                case 1:
                if(xK + shipLength > FIELD_LENGTH + 1)
                {
                    System.out.println("Das Schiff übertritt, auf der rechten oder linken seite, den Rand!");
                    failCeck = true;
                }
                case 2:
                if(yK + shipLength > FIELD_LENGTH + 1)
                {
                    System.out.println("Das Schiff übertritt, oben oder unten, den Rand!");
                    failCeck = true;
                }
            }
            System.out.println();
        }
        while(failCeck);
        System.out.println();
        System.out.println("Das Schiff wurde positioniert");
        System.out.println();
    }

    private void setShipS(int[][] curField, int curXK, int curYK, int curFieldPos)
    {
        if(curField[curYK - 1 + curFieldPos][curXK - 1] == 0)
        {
            curField[curYK - 1 + curFieldPos][curXK - 1] = shipSinkHelper;
        }
        else
        {
            failCeck = true;
        }
    }

    private void setShipW(int[][] curField, int curXK, int curYK, int curFieldPos)
    {
        if(curField[curYK - 1][curXK - 1 + curFieldPos] == 0)
        {
            curField[curYK - 1][curXK - 1 + curFieldPos] = shipSinkHelper;
        }
        else
        {
            failCeck = true;
        }
    }

    public void shipInit(int curPlayer)
    {
        for(int fieldPos = 0; fieldPos < shipLength; fieldPos++)
        {
            if(shipDirection == 's')
            {   
                if(curPlayer == 0)
                {
                    setShipS(field0, xK, yK, fieldPos);
                }
                else
                {
                    setShipS(field1, xK, yK, fieldPos);
                }
            }
            else
            {
                if(curPlayer == 0)
                {
                    setShipW(field0, xK, yK, fieldPos);
                }
                else
                {
                    setShipW(field1, xK, yK, fieldPos);
                }
            } 
        }
        if(failCeck)
        {
            System.out.println("Die Positionen ihrer Schiffe überschneiden sich!");
        }
        System.out.println();
    }

    public void showField(int curPlayer)
    {
        for (int i = 0; i < FIELD_LENGTH + 1; i++)
        {
            System.out.print(i + " ");
        }
        System.out.println();
        for(int i = 0; i < FIELD_LENGTH; i++)
        { 
            System.out.print(border[i] + " ");
            for(int j = 0; j < FIELD_LENGTH; j++)
            { 
                if(curPlayer == 0)
                {
                    System.out.print(field0[i][j] + " ");
                }
                else
                {
                    System.out.print(field1[i][j] + " ");
                }
            }
            System.out.println();
        }
    }

    public void battleFunktion()
    {
        int curPlayer = 0;
        do
        {
            fire(curPlayer);
            curPlayer = (curPlayer == 0) ? 1 : 0;
        }
        while(playerWin == -1);

        if(playerWin != -1)
        {
            System.out.println("SPIELER "+ visualPlayerNumber((curPlayer == 0) ? 1 : 0) +" HAT GEWONNEN!");
        }
    }

    private void innerFire(int curPlayer, int[][] curField, int bombX, int bombY) {
        int localShipSearch = 0;
        if(curField[bombY-1][bombX-1] != 0)
        {
            System.out.println("Sie haben ein feindliches Schiff getroffen!");
            int localShipSinkHelper = curField[bombY-1][bombX-1];
            //System.out.println("*** localssh "+localShipSinkHelper);
            curField[bombY-1][bombX-1] = 0;
            playerWin = curPlayer;
            for(int i = 0; i < FIELD_LENGTH && localShipSearch == 0; i++)
            { 
                for(int j = 0; j < FIELD_LENGTH && localShipSearch == 0; j++)
                { //System.out.println("*** analysing "+i+","+j+":"+curField[i][j]);
                    if(curField[i][j] == localShipSinkHelper)
                    {
                        localShipSearch = 1;
                        //System.out.println("***1");
                    }
                    if(curField[i][j] != 0)
                    {
                        playerWin = -1;
                        //System.out.println("***2");
                    }
                }
            }
            if(localShipSearch == 0)
            {
                System.out.println("Sie haben ein Schiff von Spieler " + visualPlayerNumber((curPlayer == 0) ? 1 : 0) + " versenkt!");
            }
        }
        else
        {
            System.out.println("Sie haben leider nicht getroffen.");
        }

    }

    public void fire(int curPlayer)
    {
        int shipSearch = 0;
        System.out.println("Spieler "+ visualPlayerNumber(curPlayer) + " muss jetzt schießen!");
        System.out.println();
        System.out.print("Gebe die x Koordiante ein: ");
        int xK = s.nextInt();
        System.out.println();
        System.out.print("Gebe die y Koordiante ein: ");
        int yK = s.nextInt();
        System.out.println();
        System.out.println("Sie schießen auf die Koordinaten ( "+ xK +" | "+ yK +" ).");
        if(curPlayer == 1)
        {
            innerFire(curPlayer, field0, xK, yK);
        }
        else
        {
            innerFire(curPlayer, field1, xK, yK);
        }
    }
}