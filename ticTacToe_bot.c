#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    CELL_EMPTY = 2,
    CELL_X = 1,
    CELL_O = 0
} Cell;

typedef enum {
    RESULT_NONE,
    RESULT_X,
    RESULT_O,
    RESULT_TIE
} Result;

typedef enum {
    PLAYER_HUMAN,
    PLAYER_AI
} PlayerType;

typedef struct {
    int n;
    Cell **board;
    int moves;
    Cell current_player;   // CELL_X or CELL_O
    PlayerType player_x;
    PlayerType player_o;
} Game;

Cell **initializeBoard(int n);
void freeBoard(Cell **board, int n);
void initGame(Game *g, int n);
void destroyGame(Game *g);
void printBoard(const Game *g);
bool play_pos(Game *g, int x, int y);
Result get_game_result(const Game *g);
void clear_input(void);
int read_int_in_range(const char *prompt, int min, int max);
void get_ai_move(Game *g, int *out_row, int *out_col);

Cell **initializeBoard(int n) {
    Cell **board = malloc(sizeof(Cell*) * n);
    if (!board) {
        fprintf(stderr, "Memory allocation failed for board rows.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; ++i) {
        board[i] = malloc(sizeof(Cell) * n);
        if (!board[i]) {
            fprintf(stderr, "Memory allocation failed for board row %d.\n", i);
            for (int k = 0; k < i; ++k) {
                free(board[k]);
            }
            free(board);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < n; ++j) {
            board[i][j] = CELL_EMPTY;
        }
    }
    return board;
}

void freeBoard(Cell **board, int n) {
    if (!board) return;
    for (int i = 0; i < n; ++i) {
        free(board[i]);
    }
    free(board);
}

void initGame(Game *g, int n) {
    g->n = n;
    g->board = initializeBoard(n);
    g->moves = 0;
    g->current_player = CELL_X; // X starts
    g->player_x = PLAYER_HUMAN;
    g->player_o = PLAYER_HUMAN;
}

void destroyGame(Game *g) {
    freeBoard(g->board, g->n);
    g->board = NULL;
}

void printBoard(const Game *g) {
    int n = g->n;
    Cell **board = g->board;

    printf("   ");
    for (int j = 0; j < n; ++j) {
        printf("%d", j + 1);
        if (j < n - 1) printf("|");
    }
    printf("\n");

    for (int i = 0; i < n; ++i){
        if (i == 0) {
            printf("  ");
            for (int j = 0; j < n; ++j){
                printf("-");
                if (j < n - 1) printf("+");
            }
            printf("\n");
        }

        printf("%d ", i + 1);
        for (int j = 0; j < n; ++j){
            char c;
            if (board[i][j] == CELL_O) {
                c = 'O';
            } else if (board[i][j] == CELL_X) {
                c = 'X';
            } else {
                c = ' ';
            }
            printf("%c", c);
            if (j < n - 1) {
                printf("|");
            }
        }
        printf("\n");

        if (i < n - 1) {
            printf("  ");
            for (int j = 0; j < n; ++j){
                printf("-");
                if (j < n - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// x, y are 1-based
bool play_pos(Game *g, int x, int y) {
    int n = g->n;
    int row = x - 1;
    int col = y - 1;

    if (row < 0 || row >= n || col < 0 || col >= n) {
        printf("Invalid coordinates! Try again.\n");
        return false;
    }

    if (g->board[row][col] != CELL_EMPTY) {
        printf("Invalid play! That position is already taken. Try again.\n");
        return false;
    }

    g->board[row][col] = g->current_player;
    return true;
}

Result get_game_result(const Game *g) {
    int n = g->n;
    Cell **board = g->board;

    // rows
    for (int i = 0; i < n; i++) {
        bool rowWin = true;
        Cell first = board[i][0];
        if (first == CELL_EMPTY) continue;
        for (int j = 1; j < n; j++) {
            if (board[i][j] != first) {
                rowWin = false;
                break;
            }
        }
        if (rowWin) {
            return (first == CELL_X) ? RESULT_X : RESULT_O;
        }
    }

    // cols
    for (int j = 0; j < n; j++) {
        bool colWin = true;
        Cell first = board[0][j];
        if (first == CELL_EMPTY) continue;
        for (int i = 1; i < n; i++) {
            if (board[i][j] != first) {
                colWin = false;
                break;
            }
        }
        if (colWin) {
            return (first == CELL_X) ? RESULT_X : RESULT_O;
        }
    }

    // main diag
    bool diagWin = true;
    Cell first = board[0][0];
    if (first != CELL_EMPTY) {
        for (int i = 1; i < n; i++) {
            if (board[i][i] != first) {
                diagWin = false;
                break;
            }
        }
        if (diagWin) {
            return (first == CELL_X) ? RESULT_X : RESULT_O;
        }
    }

    // anti diag
    diagWin = true;
    first = board[0][n - 1];
    if (first != CELL_EMPTY) {
        for (int i = 1; i < n; i++) {
            if (board[i][n - 1 - i] != first) {
                diagWin = false;
                break;
            }
        }
        if (diagWin) {
            return (first == CELL_X) ? RESULT_X : RESULT_O;
        }
    }

    if (g->moves >= n * n) {
        return RESULT_TIE;
    }

    return RESULT_NONE;
}

void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

int read_int_in_range(const char *prompt, int min, int max) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) != 1) {
            clear_input();
            printf("Invalid input, please enter a number.\n");
            continue;
        }
        if (x < min || x > max) {
            printf("Please enter a value between %d and %d.\n", min, max);
            continue;
        }
        return x;
    }
}

// simple AI, works for any n
void get_ai_move(Game *g, int *out_row, int *out_col) {
    int n = g->n;
    Cell me = g->current_player;
    Cell opp = (me == CELL_X) ? CELL_O : CELL_X;

    // 1. try to win in one move
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g->board[i][j] == CELL_EMPTY) {
                g->board[i][j] = me;
                Result r = get_game_result(g);
                g->board[i][j] = CELL_EMPTY;
                if ((me == CELL_X && r == RESULT_X) || (me == CELL_O && r == RESULT_O)) {
                    *out_row = i;
                    *out_col = j;
                    return;
                }
            }
        }
    }

    // 2. block opponent's win
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g->board[i][j] == CELL_EMPTY) {
                g->board[i][j] = opp;
                Result r = get_game_result(g);
                g->board[i][j] = CELL_EMPTY;
                if ((opp == CELL_X && r == RESULT_X) || (opp == CELL_O && r == RESULT_O)) {
                    *out_row = i;
                    *out_col = j;
                    return;
                }
            }
        }
    }

    // 3. small heuristic for 3x3: center then corners
    if (n == 3) {
        if (g->board[1][1] == CELL_EMPTY) {
            *out_row = 1;
            *out_col = 1;
            return;
        }
        int corners[4][2] = { {0,0}, {0,2}, {2,0}, {2,2} };
        for (int k = 0; k < 4; ++k) {
            int i = corners[k][0];
            int j = corners[k][1];
            if (g->board[i][j] == CELL_EMPTY) {
                *out_row = i;
                *out_col = j;
                return;
            }
        }
    }

    // 4. fallback: first empty cell, works for any n
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g->board[i][j] == CELL_EMPTY) {
                *out_row = i;
                *out_col = j;
                return;
            }
        }
    }

    // should not happen if moves < n*n
    *out_row = 0;
    *out_col = 0;
}

int main(void) {
    printf("Welcome to variable Tic Tac Toe game!\n\n");
    printf("Player 1 = X\n");
    printf("Player 2 / Bot = O\n\n");

    int mode = read_int_in_range(
        "Choose mode:\n1) Player vs Player\n2) Player vs Bot (you = X, bot = O)\nChoice: ",
        1, 2
    );

    int n = read_int_in_range(
        "What is the size of the board you want to play? Enter a number between 1 and 1000: ",
        1, 1000
    );

    Game game;
    initGame(&game, n);

    game.player_x = PLAYER_HUMAN;
    game.player_o = (mode == 1) ? PLAYER_HUMAN : PLAYER_AI;

    Result res = RESULT_NONE;

    while (res == RESULT_NONE) {
        printBoard(&game);

        bool current_is_human =
            (game.current_player == CELL_X)
                ? (game.player_x == PLAYER_HUMAN)
                : (game.player_o == PLAYER_HUMAN);

        if (current_is_human) {
            if (game.current_player == CELL_X) {
                printf("Player 1 turn (X)\n\n");
            } else {
                printf("Player 2 turn (O)\n\n");
            }

            int x, y;
            while (1) {
                x = read_int_in_range("Enter row: ", 1, game.n);
                y = read_int_in_range("Enter column: ", 1, game.n);
                if (play_pos(&game, x, y)) {
                    break;
                }
            }
        } else {
            printf("Bot's turn (O)\n\n");
            int row, col;
            get_ai_move(&game, &row, &col);
            printf("Bot plays at row %d, column %d\n", row + 1, col + 1);
            play_pos(&game, row + 1, col + 1);
        }

        game.moves++;
        res = get_game_result(&game);

        if (res == RESULT_NONE) {
            game.current_player = (game.current_player == CELL_X) ? CELL_O : CELL_X;
        }
    }

    printBoard(&game);
    printf("\n");

    if (res == RESULT_X) {
        printf("CONGRATS Player 1 WON THE GAME!\n");
    } else if (res == RESULT_O) {
        if (game.player_o == PLAYER_AI) {
            printf("The bot (O) won.\n");
        } else {
            printf("CONGRATS Player 2 WON THE GAME!\n");
        }
    } else {
        printf("It's a tie!\n");
    }

    destroyGame(&game);
    return 0;
}
