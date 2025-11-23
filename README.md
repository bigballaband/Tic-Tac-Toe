# Variable Tic Tac Toe

A simple C program that lets you play Tic Tac Toe on any board size.  
You can play against another person or against a basic bot.

## Features

- Choose any board size from **1 to 1000**
- **Human vs Human** mode
- **Human vs Bot** mode  
  - Bot checks for instant winning moves  
  - Blocks your winning moves  
  - Uses a fallback strategy after that
- Clean win detection for rows, columns, and both diagonals
- Board prints with row and column labels
- No memory leaks

## Build

Compile with `gcc`:

```bash
gcc tictactoe.c -o tictactoe

```

Run the game: ./tictactoe

## How to Play

1. Choose a mode:
   - `1` for Player vs Player  
   - `2` for Player vs Bot  

2. Enter your board size `n`.

3. Make your moves by entering:
   - Row number  
   - Column number  

4. **X** goes first.  
   **O** is the second player or the bot.

5. The game ends when:
   - A row, column, or diagonal is filled by one player  
   - The board is full (tie)

## Notes
- Boards larger than ~20x20 work but may not look clean in a terminal.
- The bot is simple. It plays reasonable moves but is not designed to be perfect for big boards.
