#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int** initializeBoard(int n) {
    int** board = malloc(sizeof(int)*n * 2);
    for (int i = 0; i < n; ++i){
        board[i] = malloc(sizeof(int)*n*2);
    }
    for (int i = 0; i < 2*n; ++i) {
        for (int j = 0; j < 2*n; ++j){
            if (j == 0 || j == 2*n-1){
                board[i][j] = -1;
            }
            else if (j % 2 == 0){
                board[i][j] = 3;
            }
            board[i][j] = 2;
        }
    }
    return board;
}

void printBoard (int **board, int n) {
    for (int i = 0; i < 2*n; ++i){
        for (int j = 0; j < n; ++j){
            if (board[i][j] == 0) {
                printf("O");
            }
            else if (board[i][j] == 1) {
                printf("X");
            }
            else if (board[i][j] == 2){
                printf(" ");
            }
            else if (board [i] [j] ==  3) {
                printf("|");
            }
            else if (board[i][j] ==  -1){
                continue;
            }

        }
        printf("\n");
        for (int j = 0; j < n; ++j){
            printf("-");
            printf(" ");
        }
        printf("\n");
    }
}

void play_pos(int** board, int n, int x, int y, bool player){
    
    if (board[2*x - 2][2*y - 2] != 2) {
        printf("Invalid play! Try again.");
    }
    else {
        if (player) {
            board[2*x - 2][2*y - 2] = 0;
        }
        else  {
            board[2*x - 2][2*y - 2] = 1;
        }
    }
}

void gameCompleteCheck (int** board, bool complete, int n) {
    bool row = true;
    bool column = true;
    bool slantTop = true;
    bool slantBot =  true;
    for (int i = 2; i < 2 * n; i += 2) {
        for (int j = 2; j < 2 * n; j += 2) {
            if (board[i - 2][j] != board[i - 2][j - 2]) {
                row = false;
            }
            if (board[i][j] != board[i][j - 2]) {
                row = false;
            }
            if (board[j - 2][i] != board[j - 2][i - 2]) {
                column = false;
            }
            if (board[j][i] != board[j][i - 2]) {
                column = false;
            }
        }
        if (board[i][i] != board[i - 2][i - 2]) {
            slantTop = false;
        }
        if (board[i][2 * n - i] != board[i - 2][2 * n - i - 2]) {
            slantBot = false;
        }
    }
    complete = slantBot || slantTop || column || row;
}


int main (void) {
    printf("Welcome to variable Tic Tac Toe game!\n\n");
    printf("Player 1 = X\n\n");
    printf("Player 2 = O\n\n ");
    while (1){
         printf("What is the size of the board you want to play? Enter a number between 1 and 1000");
    int n;
    scanf("%d", &n);
    printf("\n\n");
    if (n >= 1 && n <= 1000){
        int** board = initializeBoard(n);
        bool complete = false; 
        bool player2 =  false;
        while (!complete){
            if (player2){
                printf("Player 2 turn\n\n");
            }
            if (player2){
                printf("Player 1 turn\n\n");
            }
            int x;
            printf("Enter x coordinate: ");
            scanf("%d", &x);
            if (x < 1 || x > n){
                printf("Invalid x coordinate, try again.\n");
                continue;
            }
            int y;
            printf("Enter y coordinate: ");
            scanf("%d", &y);
            if (y < 1 || y > n){
                printf("Invalid x coordinate, try again.\n");
                continue;
            }
            play_pos(board, n, x, y, player2);
            gameCompleteCheck(board, complete, n);
            if (complete){
                if (player2){
                    printf("CONGRATS Player 2 WON THE GAME!");
                    return 0;
                }
                else {
                    printf("CONGRATS Player 1 WON THE GAME!");
                    return 0;
                }
                break;
            }
            player2 = !player2;
        }
        for (int i = 0; i < 2 * n; ++i) {
                free(board[i]);
            }
            free(board);

            break;
        }
    else {
        printf("Invalid integer, try again\n");
    }
    }
    return 0;
   
}