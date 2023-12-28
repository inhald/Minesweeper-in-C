#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 10
#define BOMBS 15


int countAdjacentBombs(char board[SIZE][SIZE], int x, int y){ 
  int count;

  /*each of the conditions board[][] == 'X' will evaluate to either 0 or 1
  and thus the sum of the result will give the number of adjacent bombs*/
  /*the first and last column  cases needed to be tweaked because of how multidimensional
  arrays are stored in memory*/
  if(y == 0){
    count = (board[x-1][y] == 'X') + (board[x-1][y+1] == 'X') + (board[x][y+1] == 'X') + (board[x+1][y+1] == 'X') + (board[x+1][y] == 'X');
  }
  if(y == 9){
    count = (board[x-1][y] == 'X') + (board[x+1][y] == 'X') + (board[x][y-1] == 'X') + (board[x+1][y-1] == 'X') + (board[x-1][y-1] == 'X');
  }
  else{
    count = (board[x-1][y-1]  == 'X') + (board[x-1][y] == 'X') + (board[x-1][y+1] == 'X') 
    + (board[x][y-1] == 'X') + (board[x][y+1] == 'X') + (board[x+1][y-1] == 'X') + (board[x+1][y] == 'X')
    + (board[x+1][y+1] == 'X');
  }
  /*returning count*/
  return count;
}

/*this function uses the srand function to randomly
place bombs on the board*/
void placeBombs(char board[SIZE][SIZE]){
  srand(time(0));
  
  int counter = 15;

  /*if the randomly generated coords
    already have bomb, then code will do
    another iteration of loop until 15
    unique bombs are placed across board*/
  for(int i =0; i < counter; ++i){
    int x = rand() % SIZE;
    int y = rand() % SIZE;

    if(board[x][y] == 'X'){
      counter++;
    }
    else{
      board[x][y] = 'X';
    }
  }
}

//this function will print the board in the terminal
void printBoard(char revealed[SIZE][SIZE]){  
  printf("  ");


   /*prints the labels above the grid
  for clarity */
  for(int i =0; i < SIZE; ++i){
    printf(" %d", i);
  }
  printf("\n");
  /*uses a nested for loop to iterate through
  each element*/
  for(int i =0; i < SIZE; ++i){
    printf(" %d", i);
    for(int j=0; j < SIZE; ++j){
      printf(" %c", revealed[i][j]);
    }
    printf("\n");
  }
  
}


void autosolve(char board[SIZE][SIZE], char revealed[SIZE][SIZE], int i, int j, int remaining) {
    // Base case: check if the cell is outside the board or has already been revealed
    if (i < 0 || i >= SIZE || j < 0 || j >= SIZE || revealed[i][j] != '-') {
        return;
    }

    // Reveal the current cell
    revealed[i][j] = countAdjacentBombs(board, i, j) + '0';
    remaining--;

    // If the current cell is '0', recursively reveal its neighbors
    if (revealed[i][j] == '0') {
        for (int x = i - 1; x <= i + 1; ++x) {
            for (int y = j - 1; y <= j + 1; ++y) {
                autosolve(board, revealed, x, y, remaining);
            }
        }
    }
}


int main(){

  char board[SIZE][SIZE];

  char revealed[SIZE][SIZE];


  //initialzing board
  for(int i =0; i < SIZE; ++i){
    for(int j =0; j < SIZE; ++j){
      board[i][j] = '-';
      revealed[i][j] = '-';
    }
  }

   /*board is initially empty 
  then '-' are replaced with bombs*/
  placeBombs(board);

  //game begins in terminal
  printf("Welcome to Minesweeper! \n");

  //printBoard(board);
  
  printBoard(revealed);

  
  /*game will continue so long as gameOver is false
  and there are non-bomb cells remaning */
  bool gameOver = false;
  int remaining = SIZE*SIZE - BOMBS;

  while (gameOver == false && remaining > 0){
    bool isValid = false;
    int x,y;

    while(isValid == false) {
      printf("Please enter coordinates (x,y) to reveal a cell: \n");
      scanf("%d %d", &x, &y);

      //to be valid they must be within the correct range
      //and must not have already been revealed
      if((0 <= x && x <= 9) && (0 <= y && y <= 9)) {
        if(revealed[x][y] == '-'){
          isValid = true;
          break;
        }
        else{
          printf("The inputted coordinates have already been revealed on the board. \n");
          continue;
        }
      }
      else {
        printf("The inputted coordinates are not within the range 0 <= x <= 9 and 0 <= y <= 9. \n");
        continue;
      } 
    }

    //checks if bomb has been hit
    if(board[x][y] == 'X'){
      /*ends game and prints revealed board
      with position of bombs */
      gameOver = true;
      printf("You hit a bomb! Game Over!\n");
      revealed[x][y] = 'X';

      for(int i = 0; i < SIZE; ++i){

        for(int j = 0; j < SIZE; ++j){
          if(board[i][j] == 'X'){
            revealed[i][j] = 'X';
          }
        }
      }

      printBoard(revealed);

      break;
    }

    //autosolve function is called recursively
    //to reveal chain of zeros and surroundings
    else{
      autosolve(board,revealed, x, y, remaining);
      printBoard(revealed);
      continue;
    }

    //win conditions and game ends
    if(remaining == 0){
      printf("Congrats! You have won the game!\n");
      printBoard(revealed);
      gameOver = true;
      break;
    }
  }
}