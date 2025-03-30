#include "names.h"

#include <stdio.h>
#include <stdlib.h>
//Sets up the different dice actions
typedef enum { DOT, LEFT, CENTER, RIGHT } Position;
const Position die[6] = { DOT, DOT, DOT, LEFT, CENTER, RIGHT };
typedef struct {
    const char *name;
    int chips;
} Player;
//Main function
//void playGame(Player players[], int num_players, int *center, int playerIndex);
//int checkWinner(Player players[], int num_players);
//Checks to see if there is only one player left with chips in the game
//If so, that person wins

int checkWinner(Player players[], int num_players) {
    int lastPerson = -1;
    int withChips = 0;
    for (int i = 0; i < num_players; i++) {
        if (players[i].chips > 0) {
            withChips++;
            lastPerson = i;
        }
    }
    return withChips == 1 ? lastPerson : -1;
}
//runs the actual game of LRC
void playGame(Player players[], int num_players, int *center, int playerIndex) {
    int roll = players[playerIndex].chips > 3 ? 3 : players[playerIndex].chips;
    printf("%s", players[playerIndex].name);
    //rolls the dice
    for (int i = 0; i < roll; i++) {
        int roll = (random() % 6);
        if (players[playerIndex].chips > 0) {
            switch (die[roll]) {
            //gives a chip to the player to the left
            //removes chip from current player
            case LEFT:
                players[playerIndex].chips -= 1;
                players[(playerIndex + num_players + 1) % num_players].chips++;
                break;
            //gives a chip to the player to the right
            //removes chip from current player
            case RIGHT:
                players[playerIndex].chips -= 1;
                players[(playerIndex - 1 + num_players) % num_players].chips++;
                break;
            //gives a chip to the center pot
            //removes chip from curr3ent player
            case CENTER:
                players[playerIndex].chips -= 1;
                (*center)++;
                break;
            case DOT: break;
            }
        }
    }
    printf(": ends her turn with %d\n", players[playerIndex].chips);
}
int main(void) {
    //Check the input that the user gives
    int num_players = 3;
    printf("Number of players (3 to 10)? ");
    int scanf_result = scanf("%d", &num_players);
    if (scanf_result < 1 || num_players < 3 || num_players > 10) {
        fprintf(stderr, "Invalid number of players. Using 3 instead.\n");
        num_players = 3;
    }
    unsigned seed = 4823;
    printf("Random-number seed? ");
    scanf_result = scanf("%u", &seed);
    if (scanf_result < 1 || seed == 0) {
        fprintf(stderr, "Invalid seed. Using 4823 instead.\n");
        seed = 4823;
    }
    Player players[num_players];
    for (int i = 0; i < num_players; i++) {
        players[i].name = player_name[i];
        players[i].chips = 3;
    }
    srandom(seed);
    int center = 0;
    int currPlayer = 0;
    while (1) {
        if (players[currPlayer].chips > 0) {
            playGame(players, num_players, &center, currPlayer);
        }
        int winnerIndex = checkWinner(players, num_players);
        if (winnerIndex != -1) {
            printf("%s won!\n", players[winnerIndex].name);
            break;
        }
        currPlayer = (currPlayer + 1) % num_players;
    }
    return 0;
}
