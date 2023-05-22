#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <usart.h> // USB communication library
#include <stdio.h> // Standard input/output library
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include <mine.h> 
#include <leds.h> // Library for controlling the LEDs
#include <btns.h> // Library for controlling the buttons
#include <isr.h> // Library for setting up the interrupt service routines

#define BTN1 PC1
#define BTN2 PC2
#define BTN3 PC3

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3

int isGameStarted = 0;
int isDisabledInput = 0;
int roundNum = 0;
int puzzleWhole[10];
int puzzlePlayer[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int puzzlePlayerIndex = 0;
int btnsPressed[] = {0,0,0};

ISR( PCINT1_vect )
{
    for (int i = 0; i < 3; i++){
        if ( buttonPushed( i+1 ) ){
            if (isGameStarted == 0 && i == 0){
                isGameStarted = 1;
            } else if (isDisabledInput == 0){
                btnsPressed[i] = 1;
            } else {
                btnsPressed[i] = 0;
            }
        }
    }
}

void showRound(){
    printf("Showing round\n");
    blinkAllLeds(500);
    _delay_ms(500);
    for (int i = 0; i < roundNum; i++){
        printf("L: %d\n", puzzleWhole[i]+1);
        switch (puzzleWhole[i])
        {
        case 0:
            lightUpOneLed(LED1);
            break;
        case 1:
            lightUpOneLed(LED2);
            break;
        case 2:
            lightUpOneLed(LED3);
            break;
        default:
            break;
        }
        _delay_ms(800);
        lightDownAllLeds();
        _delay_ms(500);
    }
    blinkAllLeds(500);
}

void playerButtonPress(int btn){
    // add to puzzlePlayer and light it up
    printf("Player button press adding btn %d\n", btn);
    puzzlePlayer[puzzlePlayerIndex] = btn-1;
    puzzlePlayerIndex++;
    lightUpOneLed(btn-1);
    while (buttonPushed( btn ))
    {
        _delay_ms(10);
    }
    lightDownOneLed(btn-1);
}

bool checkPuzzlePlayerFill(){
    // printf("Checking puzzle player fill\n");
    for (int i = 0; i < roundNum; i++){
        if (puzzlePlayer[i] == -1){
            // printf("Puzzle player not filled\n");
            return false;
        }
    }
    printf("Puzzle player filled\n");
    return true;
}

bool checkPuzzlePlayerCorrect(){
    printf("Checking puzzle player correct\n");
    for (int i = 0; i < roundNum; i++){
        if (puzzlePlayer[i] != puzzleWhole[i]){
            printf("Puzzle player incorrect\n");
            return false;
        }
    }
    printf("Puzzle player correct\n");
    return true;
}

void resetPlayerPuzzle(){
    printf("Resetting player puzzle\n");
    for (int i = 0; i < roundNum; i++){
        puzzlePlayer[i] = -1;
    }
}

void endGame(){
    printf("Game Over!\n");
    isGameStarted = 0;
    isDisabledInput = 0;
    roundNum = 0;
    lightDownAllLeds();
}

void gameLoop(){
    printf("Game started\n");
    while (isGameStarted == 1){
        printf("Round %d start:\n", roundNum);
        isDisabledInput = 1;
        roundNum++;
        showRound();
        // _delay_ms(10);
        isDisabledInput = 0;
        while(!checkPuzzlePlayerFill()){
            // printf("Waiting for player to fill puzzle\n");
            for (int i = 0; i < 3; i++){
                if (btnsPressed[i] == 1){
                    playerButtonPress(i+1);
                    btnsPressed[i] = 0;
                }
            }
            _delay_ms(10);
        }
        if(checkPuzzlePlayerCorrect()){
            resetPlayerPuzzle();
            puzzlePlayerIndex = 0;
        } else {
            // End Game
            isGameStarted = 0;
        }
    }
    endGame();
}

void generatePuzzle(){
    printf("Generating puzzle\n");
    for (int i = 0; i < 10; i++){
        puzzleWhole[i] = rand() % 3;
    }
}

// Main Menu Loop
void mainMenu(){
    while(isGameStarted == 0){
        printf("Main Menu\n");
        blinkLed(LED4, 100);
        _delay_ms(200);
    }
    // Start Game
    generatePuzzle();
    gameLoop();
}

int main()
{
    initUSART();
    enableAllLeds();
    lightDownAllLeds();
    enableAllButtons();
    activateInterrupt(PCIE1);
    indicatePCMSK1(BTN1);
    indicatePCMSK1(BTN2);
    indicatePCMSK1(BTN3);
    enableInterrupts();

    while (1)
    {
        mainMenu();
    }
    

    printf("Bye world!\n");
    return 0;
}