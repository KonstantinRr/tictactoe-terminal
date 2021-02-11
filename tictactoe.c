#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"

struct TicTacToe {
    int playerSelectionX;
    int playerSelectionY;
    int playerTurn;
    char board[3][3];
    char player1;
    char player2;
};

void clearGame(struct TicTacToe *game, int startPlayer)
{
    // clears the board by setting it to zero
    memset(game->board, (int)' ', 9);
    game->playerSelectionX = 1;
    game->playerSelectionY = 1;
    game->playerTurn = startPlayer;
    game->player1 = 'X';
    game->player2 = 'O';
}

void switchPlayer(struct TicTacToe *game)
{
    game->playerTurn = !game->playerTurn;
}

void moveCursorUp(struct TicTacToe *game)
{
    if (game->playerSelectionY > 0)
        game->playerSelectionY--;
}

void moveCursorDown(struct TicTacToe *game)
{
    if (game->playerSelectionY < 2)
        game->playerSelectionY++;
}

void moveCursorLeft(struct TicTacToe *game)
{
    if (game->playerSelectionX > 0)
        game->playerSelectionX--;
}

void moveCursorRight(struct TicTacToe *game)
{
    if (game->playerSelectionX < 2)
        game->playerSelectionX++;
}

void markField(struct TicTacToe *game)
{
    char *pos = &(game->board)[game->playerSelectionX][game->playerSelectionY];
    if (*pos == ' ') {
        if (game->playerTurn)
            *pos = game->player1;
        else
            *pos = game->player2;
        switchPlayer(game);
    }
}

int checkWin(struct TicTacToe *game, char p)
{
    char (*b)[3] = game->board;
    return
        ((b[0][0] == p && b[0][1] == p && b[0][2] == p) || // horizontal 1
        (b[1][0] == p && b[1][1] == p && b[1][2] == p) || // horizontal 2
        (b[2][0] == p && b[2][1] == p && b[2][2] == p) || // horizontal 3
        (b[0][0] == p && b[1][0] == p && b[2][0] == p) || // vertical 1
        (b[0][1] == p && b[1][1] == p && b[2][1] == p) || // vertical 2
        (b[0][2] == p && b[1][2] == p && b[2][2] == p) || // vertical 3
        (b[0][0] == p && b[1][1] == p && b[2][2] == p) || // diagonal 1
        (b[2][0] == p && b[1][1] == p && b[0][2] == p));  // diagonal 2
}

int checkDraw(struct TicTacToe *game)
{
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++)
            if ((game->board)[x][y] == ' ')
                return 0;
    }
    return 1;
}

void printGame(struct TicTacToe *game)
{
    // uses strings because it is UTF-8 encoded
    const char* selectionUp[3] = { " ", " ", " " };
    const char* selectionRight[3] = { " ", " ", " " };
    selectionUp[game->playerSelectionX] = "˅";
    selectionRight[game->playerSelectionY] = "˂";

    // Unicode that is UTF-8 encoded
    printf(
        "  %s   %s   %s\n"
        "┌───┬───┬───┐\n"
        "│ %c │ %c │ %c │ %s\n"
        "├───┼───┼───┤\n"
        "│ %c │ %c │ %c │ %s\n"
        "├───┼───┼───┤\n"
        "│ %c │ %c │ %c │ %s\n"
        "└───┴───┴───┘\n",
        selectionUp[0], selectionUp[1], selectionUp[2],
        game->board[0][0], game->board[1][0], game->board[2][0],
        selectionRight[0],
        game->board[0][1], game->board[1][1], game->board[2][1],
        selectionRight[1],
        game->board[0][2], game->board[1][2], game->board[2][2],
        selectionRight[2]
    );
}

int userInput(struct TicTacToe *game)
{
    // Takes the next user input
    int input = getChar();
    switch (input) {
    case EOF:
        printf("Unexpected EOF, terminating...\n");
        return 0;
    case 'q':
        printf("Quitting game...\n");
        return 0;
    case 'w': case 'i':
        moveCursorUp(game);
        break;
    case 's': case 'k':
        moveCursorDown(game);
        break;
    case 'a': case 'j':
        moveCursorLeft(game);
        break;
    case 'd': case 'l':
        moveCursorRight(game);
        break;
    case 0x0A: // Enter key code
        markField(game);
        break;
    }
    return 1;
}

int main()
{
    struct TicTacToe game;
    clearGame(&game, 1);

    while (1) {
        clearScreen();
        printf(
            "Use WASD  or IJKL to move the cursor!\n"
            "Use Enter to mark your field!\n"
            "Player 1: %c\n"
            "Player 2: %c\n",
            game.player1, game.player2
        );
        printGame(&game);
        printf("It is player %d turn!\n", game.playerTurn + 1);
        
        // Checks for the different end conditions
        if (checkWin(&game, game.player1)) {
            printf("Player 1 won the round!\n");
            return 0;
        }
        if (checkWin(&game, game.player2)) {
            printf("Player 2 won the round!\n");
            return 0;
        }
        if (checkDraw(&game)) {
            printf("The game is a draw!\n");
            return 0;
        }

        if (!userInput(&game))
            break; 
    }

    return 0;
}
