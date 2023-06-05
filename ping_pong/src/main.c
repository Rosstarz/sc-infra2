// IMPORTS
// Libraries that are allowed: string.h, stdlib.h, avr/interrupt.h, avr/io.h, usart.h, util/delay.h
#include <avr/io.h>
// #define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <usart.h>
#include <display.h>
#include <avr/interrupt.h>
#include <stdlib.h>
// My libraries
#include <btns.h>
#include <isr.h>
#include <leds.h>
#include <display.h>
#include <timer.h>
#include <buzzer.h>

// MACROS
#define BTN1 PC1
#define BTN2 PC2
#define BTN3 PC3
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define TABLE_MAX 25
#define TABLE_MIN 5
#define BALL_SPEED_MAX 700
#define BALL_SPEED_MIN 100

// STRUCTS
typedef struct
{
    int score;
    int wins;
    char *name;
} PLAYER;
// Use an array for the score of the players
// int *playerScore;
PLAYER *players;

// GLOBAL VARIABLES
int appOn = 1;
int menuOn = 1;
int gameOn = 0;
int settingsOn = 0;
int tableSize = 10;
int tableSizeIndex = 2;
int ballSpeedDelay = 300;
int ballSpeedDelayIndex = 3;
char *table;
int ballMovingRight = 1;
int p1TooSoon = 0;
int p2TooSoon = 0;
int blinkCount = 0;
int gameOver = 0;
int pWinner = 0;

// TODO:
// ADC set initial speed of ball using potenciometer

void setBallSpeed()
{
    ballSpeedDelay -= 200;
    ballSpeedDelayIndex++;
    if (ballSpeedDelay < BALL_SPEED_MIN)
    {
        ballSpeedDelay = BALL_SPEED_MAX;
        ballSpeedDelayIndex = 1;
    }
}

void setTableSize()
{
    tableSize += 5;
    tableSizeIndex++;
    if (tableSize > TABLE_MAX)
    {
        tableSize = TABLE_MIN;
        tableSizeIndex = 1;
    }
}

// INTERRUPTS
ISR(PCINT1_vect)
{
    if (menuOn)
    {
        if (buttonPushed(1))
        {
            menuOn = 0;
            gameOn = 1;
        }
        else if (buttonPushed(2))
        {
            menuOn = 0;
            settingsOn = 1;
        }
        else if (buttonPushed(3))
        {
            menuOn = 0;
            appOn = 0;
        }
    }
    else if (settingsOn)
    {
        if (buttonPushed(1))
        {
            setBallSpeed();
        }
        else if (buttonPushed(2))
        {
            setTableSize();
        }
        else if (buttonPushed(3))
        {
            settingsOn = 0;
            menuOn = 1;
        }
    }
    else if (gameOn)
    {
        if (buttonPushed(2))
        {
        //     gameOn = 0;
        //     menuOn = 1;
        // }
        // else if (buttonPushed(1) && buttonPushed(3))
        // {
            setBallSpeed();
        }
    }
}

void showScreenInfo()
{
    writeCharToSegment(0, 'S');
    writeNumberToSegment(1, ballSpeedDelayIndex);
    writeCharToSegment(2, 'H');
    writeNumberToSegment(3, tableSizeIndex);
}

ISR(TIMER0_COMPA_vect)
{
    if (menuOn)
    {
        showScreenInfo();
    }
    else if (settingsOn)
    {
        if (blinkCount < 100)
        {
            showScreenInfo();
        }
        if (blinkCount == 259)
        {
            blinkCount = 0;
        }
        blinkCount++;
    }
    else if (gameOn)
    {
        showScreenInfo();
    }
    else if (gameOver && !gameOn)
    {
        if (pWinner == 0)
        {
            writeString("DRAW");
        }
        else
        {
            writeCharToSegment(0, 'P');
            writeNumberToSegment(1, pWinner);
            if (players[pWinner-1].score > 9)
            {
                writeNumberToSegment(2, players[pWinner-1].score / 10);
                writeNumberToSegment(3, players[pWinner-1].score % 10);
            }
            else
            {
                writeNumberToSegment(3, players[pWinner-1].score % 10);
            }
        }
    }
    blankSegment(3); // Otherwise the last segment is much brighter than the rest
}

// This ISR runs every time TCNT0 equals the TOP value (255)
ISR(TIMER0_OVF_vect)
{
    // counter++;
    // PORTB |= _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);
}

// FUNCTIONS
void init()
{
    initUSART();
    enableAllLedsDown();
    enableButton(1);
    enableButton(2);
    enableButton(3);
    activateInterrupt(PCIE1);
    indicatePCMSK1(BTN1);
    indicatePCMSK1(BTN2);
    indicatePCMSK1(BTN3);
    initDisplay();
    initTimerInterrupts(1);
    enableInterrupts();
}

void mainMenu()
{
    // TODO game name and rules
    printf("\nMain Menu\n(1) Start Game\n(2) Settings\n(3) Exit\n");
    while (menuOn)
    {
        _delay_ms(1000);
    }
}

void countdown()
{
    printf("Countdown\n");
    enableBuzzer();
    float frequencies[] = {C5, E5, G5, C6, C7};
    const int delay = 600;
    const int toneDuration = 200;

    lightUpAllLeds();
    playToneFQ(frequencies[0], toneDuration);
    _delay_ms(delay);
    lightDownAllLeds();

    lightUpMultipleLeds2(1, 1, 1, 0);
    playToneFQ(frequencies[1], toneDuration);
    _delay_ms(delay);
    lightDownAllLeds();

    lightUpMultipleLeds2(1, 1, 0, 0);
    playToneFQ(frequencies[2], toneDuration);
    _delay_ms(delay);
    lightDownAllLeds();

    lightUpOneLed(LED1);
    playToneFQ(frequencies[3], toneDuration);
    _delay_ms(delay);
    lightDownAllLeds();

    playToneFQ(frequencies[4], toneDuration * 4);
}

void initPlayers()
{
    PLAYER player1;
    PLAYER player2;
    players = calloc(2, sizeof(PLAYER));
    player1.name = "Player";
    player1.wins = 0;
    player1.score = 0;
    player2.name = "Player";
    player2.wins = 0;
    player2.score = 0;
    *(players + 0) = player1;
    *(players + 1) = player2;
    // playerScore = calloc(2, sizeof(int));
    // *(playerScore + 0) = 0;
    // *(playerScore + 1) = 0;
}

void initTable()
{
    // printf("Init Table\n");
    table = calloc(tableSize, sizeof(int));
    for (int i = 0; i < tableSize; i++) // Walking through the array using a variable pointer
    {
        *(table + i) = '-';
        // table[i] = '-';
        // printf( " %c ", table[i] );  //initial value
    }
    table[tableSize / 2] = 'o';
}

void printTable()
{
    // printf("Print Table\n");
    printf("     [");
    for (int i = 0; i < tableSize; i++) // Walking through the array using a variable pointer
    {
        printf("%c", *(table + i));
    }
    printf("]     ");
}

void moveBall()
{
    // printf("Move Ball\n");
    for (int i = 0; i < tableSize; i++) // Walking through the array using a variable pointer
    {
        if (table[i] == 'o')
        {
            table[i] = '-';
            if (ballMovingRight)
            {
                table[i + 1] = 'o';
            }
            else
            {
                table[i - 1] = 'o';
            }
            break;
        }
    }
}

void ballCheck()
{
    // printf("Ball Check\n");
    if (table[0] == 'o' || table[tableSize - 1] == 'o')
    {
        ballMovingRight = !ballMovingRight;
    }
    if (table[0] == 'o')
    {
        if (buttonPushed(1) && !p1TooSoon)
        {
            players[0].score++;
            printf("[P1 : GOOD : %d] [P2 : ---- : %d]", players[0].score, players[1].score);
        }
        else
        {
            players[1].wins++;
            printf("[P1 : MISS : %d] [P2 : ---- : %d]", players[0].score, players[1].score);
            gameOver = 1;
        }
    }
    else if (table[tableSize - 1] == 'o')
    {
        if (buttonPushed(3) && !p2TooSoon)
        {
            players[1].score++;
            printf("[P1 : ---- : %d] [P2 : GOOD : %d]", players[0].score, players[1].score);
        }
        else
        {
            players[0].wins++;
            printf("[P1 : ---- : %d] [P2 : MISS : %d]", players[0].score, players[1].score);
            gameOver = 1;
        }
    }
    if ((table[0] != 'o' || table[1] != 'o' || table[2] != 'o') && buttonPushed(1))
    {
        p1TooSoon = 1;
    }
    else
    {
        p1TooSoon = 0;
    }
    if ((table[tableSize - 1] != 'o' || table[tableSize - 2] != 'o' || table[tableSize - 3] != 'o') && buttonPushed(3))
    {
        p2TooSoon = 1;
    }
    else
    {
        p2TooSoon = 0;
    }
}

void ballDelay()
{
    int ms = ballSpeedDelay;
    while (0 < ms)
    {
        _delay_ms(100);
        ms -= 100;
    }
}

void printSetting()
{
    printf("\r[SPEED: %d] [TABLE: %d]", ballSpeedDelayIndex, tableSizeIndex);
}

void printGameEnd()
{
    printf("\n\nGame Over!\n");
    printf("Player 1: %d\n", players[0].score);
    printf("Player 2: %d\n", players[1].score);
    pWinner = 0;
    if (players[0].score > players[1].score)
    {
        pWinner = 1;
    }
    else if (players[0].score < players[1].score)
    {
        pWinner = 2;
    }
    gameOn = 0;
    if (pWinner == 0){
        printf("It's a DRAW!\n");
    } else {
        printf("Player %d Wins! That's their win n°%d today!\n", pWinner, players[pWinner-1].wins);
    }
    printf("Press any button to continue..\n");

    // Winning melody
    // int winningMelody[] = {G3, C, E, G, C5, E5, G5, E5, PAUSE,
    //                        Gb3, C, Db, Gb, C5, Db5, Gb5, Db5, PAUSE,
    //                        Ab3, D, F, Ab, D5, Ab5, Ab5, Ab5, Ab5, C6};
    
    int winningMelody[] = {Ab5, Ab5, Ab5, Ab5, C6};
    // const int tempo[] = {DL2, DL2, DL2, DL2, DL2, DL2, DL5, DL5, DL5,
    //                      DL2, DL2, DL2, DL2, DL2, DL2, DL5, DL5, DL5,
    //                      DL2, DL2, DL2, DL2, DL2, DL5, DL1, DL1, DL1, DL10};
    const int tempo[] = {DL5, DL1, DL1, DL1, DL10};
    for (int i = 0; i < 5; i++)
    {
        playToneFQ(winningMelody[i], tempo[i]);
        _delay_ms(50);
    }
    // Waiting for input
    while (1)
    {
        if (buttonPushed(1) || buttonPushed(2) || buttonPushed(3))
        {
            break;
        }
    }
}

void mainGame()
{
    // printf("Main Game %d\n", gameOn);
    initTable();
    // TODO reset player score
    players[0].score = 0;
    players[1].score = 0;
    gameOver = 0;
    _delay_ms(1000);
    // printf("Table: %d\n", table[5]);
    while (1)
    {
        printSetting();
        printTable();
        ballCheck();
        if (gameOver)
        {
            break;
        }
        moveBall();
        ballDelay();
        fflush(stdout);
    }
    // printf("\nGameOn: %d", gameOn);
    // TODO end game screen
    printGameEnd();
    free(table);
    // free(playerScore);
}

void settings()
{
    printf("Settings\n(1) Change Ball Speed\n(2) Change Table Size\n(3) Go Back\n");
    while (settingsOn)
    {
        printSetting();
        _delay_ms(100);
        fflush(stdout);
    }
}

// MAIN
int main()
{
    init();
    initPlayers();
    while (appOn)
    {
        while (menuOn)
        {
            mainMenu();
        }
        while (settingsOn)
        {
            settings();
        }
        while (gameOn)
        {
            printf("Game started\n");
            countdown();
            mainGame();
            menuOn = 1;
            gameOn = 0;
        }
    }
    free(players);
}

// OPERATION:
//// Both players operate a button on the Arduino. You can choose which 2 buttons you assign. The “0” represents a ping pong ball that moves left and right. The game ends when the ball hits an edge (represented by “[“ and “]”). -------------DONE---------------
//
//// In the beginning, show the velocity (the speed at which the ping pong ball rolls back and forth) on your screen (= serial monitor and on your LED Display) so that the users can adjust it if necessary. When adjusted, this must be immediately visible on the screen (serial monitor) and on the LED display. -------------DONE---------------
//// Determine the initial speed (velocity) of the ball with the potentiometer. ~~~~~~~~~~~~~~~~~~~TODO~~~~~~~~~~~~~~~~~~~
////    Note: you can also adjust the velocity during the game. -------------DONE---------------
//// The game starts when 1 of the 2 players presses his/her button. From that moment on: -------------DONE---------------
////    before the game has started you will hear an ascending sound, which will stop when the game has started. -------------DONE---------------
////    you first show 4 flashing lights via the leds, then 3, then 2 and then 1 (to count down, so to speak). -------------DONE---------------
////    the ping pong ball rolls towards the opponent. -------------DONE---------------

//// Only when the ball comes close to the opponent can the opponent use his button to shoot the ball back. -------------DONE---------------
////    "Near" is 5 lines from the edge at start-up. -------------DONE---------------
////    Attention: check if the opponent has released his button. -------------DONE---------------
//// Show the speed on the LED Display. -------------DONE---------------
//// Keep a score of how many times you have been able to rebound the ball correctly per player. -------------DONE---------------
//// Just like in the situation sketch above, make sure you see the ball rolling from left to right and vice versa. -------------DONE---------------
//// Tip: do not use a newline but stay on the same line. -------------DONE---------------
//// The game ends when the ball touches either side. -------------DONE---------------
//// At that moment you show on your LED Display the player who has won (eg P1 or P2 - use the 2 leftmost LED displays) and the points that the player has achieved (show that on the 2 rightmost LED displays). -------------DONE---------------
//// Produce a victory sound on the buzzer. -------------DONE---------------
//// Show the score in the serial monitor, together with the winner, for example pingpong-2.png ????????????????????????????????????????????
//// You may come up with extras to provide a pleasant experience to the players.

// TODO:
//// The solution must at least include all specifications under the title “Description / Operation”.
//// Use at least one timer -------------DONE---------------
//// Use at least one interrupt -------------DONE---------------
//// Demonstrate somewhere in your application that you understand how pointers work. You provide a parameter “by value” or “by reference”. -------------DONE---------------

// MINIMUM REQUIREMENTS:
//// Your own, self-written library/libraries for all generic functions of the expansion shield / the arduino (don’t include any application-specific code in the libraries) -------------DONE---------------
//// The use of the arduino.h library is not allowed
//
//// Split your code into separate functions as efficiently as possible and use both call-by-value and call-by-reference. -------------DONE---------------
//// Parameter “by value” and “by reference”
//
//// Use of macros, interrupts, structs, timers, arrays, strings -------------DONE---------------
//// Use of pointers and dynamic memory allocation on the heap -------------DONE---------------
//
//// Control of at least 1 LED (D1 - D4) -------------DONE---------------
//// Detection of a button press on at least one of the buttons (S1 - S3) -------------DONE---------------
//// Use of the buzzer -------------DONE---------------
//// Use of the buzzer with various notes, OR dimming the LEDs -------------DONE---------------
//// Use of the ADC ~~~~~~~~~~~~~~~~~~~TODO~~~~~~~~~~~~~~~~~~~
//// Use of the serial communication with the connected computer (use of the KdG libraries is allowed) -------------DONE---------------
//// Control of the 8-segment display (use of the KdG libraries is allowed) -------------DONE---------------
