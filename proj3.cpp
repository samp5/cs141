/**------------------------------------------
    Programming Assignment 3: MineSweeper
    Course: CS 141, Spring 2024
    Student Author: Sam Praneis
    Lab Section: Tuesday 10am
-------------------------------------------*/

#include <iostream>

using namespace std;

// The autograder will change the random seed
#ifndef SEED_VALUE
#define SEED_VALUE 0
#endif

const int width = 5;
const int height = 8;

const int UNFLAGGED_MINE = -1; // The cell is mine that has not been flagged yet
const int FLAGGED_MINE = -2;   // The cell has been correctly flagged as a mine
const int INCORRECT_FLAGGED_MINE =
    -3;                 // The cell has been incorrectly flagged as a mine
const int UNKNOWN = -4; // The initial state of the cell.

// The number of mine in the field
const int num_mine = 10;

int c00, c01, c02, c03, c04, c10, c11, c12, c13, c14, c20, c21, c22, c23, c24,
    c30, c31, c32, c33, c34, c40, c41, c42, c43, c44, c50, c51, c52, c53, c54,
    c60, c61, c62, c63, c64, c70, c71, c72, c73, c74 = UNKNOWN;

int num_incorrect_flagged_mine = 0; // number of incorrectly flagged cells
int num_correct_flagged_mine = 0;   // number of correctly flagged cells
bool explode = false;               // if the field has exploded or not

char getTag(int c, bool explode);
void displayField(bool explode);
void setCell(int i, int j, int value);
int getCell(int i, int j);
void setMine(int num_of_mine);

// implement
void initField(int num_of_mine);
bool isMine(int i, int j);
void reveal(int i, int j);
void executeCmd(char action, int row, int col);
bool in_bounds(int i, int j);
int number_adjacent_mines(int i, int j);

// The main function of the program
// You don't need to change the main function
int main() {

  srand(SEED_VALUE); // using the same seed generates the same random
                     // sequence! In general, to have different games at
                     // each run we need to use a seed that is different for
                     // that run, for example, we can select the seed as
                     // function of time.
  initField(num_mine);
  char cmd, row_ch, col_ch;

  displayField(false);
  while (!explode && (num_correct_flagged_mine < num_mine ||
                      num_incorrect_flagged_mine > 0)) {

    cout << "Enter cmd:" << endl;
    cin >> cmd >> row_ch >> col_ch;

    executeCmd(cmd, row_ch - '0', col_ch - '0');

    displayField(explode);
    cout << "Number of mine left: "
         << num_mine - (num_correct_flagged_mine + num_incorrect_flagged_mine)
         << endl;
  }
  if (!explode) {
    cout << ":)" << endl;

  } else {
    cout << ":(" << endl;
  }
  return 0;
}

// The game has two states: in-progress, explode (game over)
// When we display the field to the user depedning of the game state the output
// is different. The helper getTag function returns the appropriate character to
// be shown in each cell depending on the state of the game
// Input:
// c: the cell variable
// explode: the game state (false for in-progress, true for game-over)
// Output:
//	one character to be shown in the cell
char getTag(int c, bool explode) {
  char tag;
  switch (c) {
  case UNKNOWN:
    tag = '-';
    break;
  case UNFLAGGED_MINE:
    if (explode)
      tag = '*';
    else
      tag = '-';
    break;
  case FLAGGED_MINE:
    tag = '!';
    break;
  case INCORRECT_FLAGGED_MINE:
    if (explode)
      tag = '&';
    else
      tag = '!';
    break;
  default:
    tag = static_cast<char>(c) + '0';
  };
  return tag;
}

// The displayField function displays the mine field (including the indices of
// the columns and rows)
// Input:
// explode: indicates the state of the game (false for in-progress or true for
// game-over) output: None
void displayField(bool explode) {
  cout << "   0 1 2 3 4" << endl << endl;
  cout << "0  " << getTag(c00, explode) << " " << getTag(c01, explode) << " "
       << getTag(c02, explode) << " " << getTag(c03, explode) << " "
       << getTag(c04, explode) << endl;
  cout << "1  " << getTag(c10, explode) << " " << getTag(c11, explode) << " "
       << getTag(c12, explode) << " " << getTag(c13, explode) << " "
       << getTag(c14, explode) << endl;
  cout << "2  " << getTag(c20, explode) << " " << getTag(c21, explode) << " "
       << getTag(c22, explode) << " " << getTag(c23, explode) << " "
       << getTag(c24, explode) << endl;
  cout << "3  " << getTag(c30, explode) << " " << getTag(c31, explode) << " "
       << getTag(c32, explode) << " " << getTag(c33, explode) << " "
       << getTag(c34, explode) << endl;
  cout << "4  " << getTag(c40, explode) << " " << getTag(c41, explode) << " "
       << getTag(c42, explode) << " " << getTag(c43, explode) << " "
       << getTag(c44, explode) << endl;
  cout << "5  " << getTag(c50, explode) << " " << getTag(c51, explode) << " "
       << getTag(c52, explode) << " " << getTag(c53, explode) << " "
       << getTag(c54, explode) << endl;
  cout << "6  " << getTag(c60, explode) << " " << getTag(c61, explode) << " "
       << getTag(c62, explode) << " " << getTag(c63, explode) << " "
       << getTag(c64, explode) << endl;
  cout << "7  " << getTag(c70, explode) << " " << getTag(c71, explode) << " "
       << getTag(c72, explode) << " " << getTag(c73, explode) << " "
       << getTag(c74, explode) << endl;
}

// The setCell functions assign a value to the cell specified by its position
// (row and column) in the field, which makes our life easier while we don't
// know how to work with arrays!
// Input:
// i: the row index (starting from
// zero)
// j: the column index (start from zero)
// Ouput: None
void setCell(int i, int j, int value) {
  if (i == 0 && j == 0)
    c00 = value;
  else if (i == 0 && j == 1)
    c01 = value;
  else if (i == 0 && j == 2)
    c02 = value;
  else if (i == 0 && j == 3)
    c03 = value;
  else if (i == 0 && j == 4)
    c04 = value;
  else if (i == 1 && j == 0)
    c10 = value;
  else if (i == 1 && j == 1)
    c11 = value;
  else if (i == 1 && j == 2)
    c12 = value;
  else if (i == 1 && j == 3)
    c13 = value;
  else if (i == 1 && j == 4)
    c14 = value;
  else if (i == 2 && j == 0)
    c20 = value;
  else if (i == 2 && j == 1)
    c21 = value;
  else if (i == 2 && j == 2)
    c22 = value;
  else if (i == 2 && j == 3)
    c23 = value;
  else if (i == 2 && j == 4)
    c24 = value;
  else if (i == 3 && j == 0)
    c30 = value;
  else if (i == 3 && j == 1)
    c31 = value;
  else if (i == 3 && j == 2)
    c32 = value;
  else if (i == 3 && j == 3)
    c33 = value;
  else if (i == 3 && j == 4)
    c34 = value;
  else if (i == 4 && j == 0)
    c40 = value;
  else if (i == 4 && j == 1)
    c41 = value;
  else if (i == 4 && j == 2)
    c42 = value;
  else if (i == 4 && j == 3)
    c43 = value;
  else if (i == 4 && j == 4)
    c44 = value;
  else if (i == 5 && j == 0)
    c50 = value;
  else if (i == 5 && j == 1)
    c51 = value;
  else if (i == 5 && j == 2)
    c52 = value;
  else if (i == 5 && j == 3)
    c53 = value;
  else if (i == 5 && j == 4)
    c54 = value;
  else if (i == 6 && j == 0)
    c60 = value;
  else if (i == 6 && j == 1)
    c61 = value;
  else if (i == 6 && j == 2)
    c62 = value;
  else if (i == 6 && j == 3)
    c63 = value;
  else if (i == 6 && j == 4)
    c64 = value;
  else if (i == 7 && j == 0)
    c70 = value;
  else if (i == 7 && j == 1)
    c71 = value;
  else if (i == 7 && j == 2)
    c72 = value;
  else if (i == 7 && j == 3)
    c73 = value;
  else if (i == 7 && j == 4)
    c74 = value;
}

// The getCell function return the value of the cell specified by its row and
// column index in the field.
// Input:
// i: the row index (starting from 0)
// j: the column index (starting from 0)
// Ouput:
// The cell value
int getCell(int i, int j) {
  int value = -1;
  if (i == 0 && j == 0)
    value = c00;
  else if (i == 0 && j == 1)
    value = c01;
  else if (i == 0 && j == 2)
    value = c02;
  else if (i == 0 && j == 3)
    value = c03;
  else if (i == 0 && j == 4)
    value = c04;
  else if (i == 1 && j == 0)
    value = c10;
  else if (i == 1 && j == 1)
    value = c11;
  else if (i == 1 && j == 2)
    value = c12;
  else if (i == 1 && j == 3)
    value = c13;
  else if (i == 1 && j == 4)
    value = c14;
  else if (i == 2 && j == 0)
    value = c20;
  else if (i == 2 && j == 1)
    value = c21;
  else if (i == 2 && j == 2)
    value = c22;
  else if (i == 2 && j == 3)
    value = c23;
  else if (i == 2 && j == 4)
    value = c24;
  else if (i == 3 && j == 0)
    value = c30;
  else if (i == 3 && j == 1)
    value = c31;
  else if (i == 3 && j == 2)
    value = c32;
  else if (i == 3 && j == 3)
    value = c33;
  else if (i == 3 && j == 4)
    value = c34;
  else if (i == 4 && j == 0)
    value = c40;
  else if (i == 4 && j == 1)
    value = c41;
  else if (i == 4 && j == 2)
    value = c42;
  else if (i == 4 && j == 3)
    value = c43;
  else if (i == 4 && j == 4)
    value = c44;
  else if (i == 5 && j == 0)
    value = c50;
  else if (i == 5 && j == 1)
    value = c51;
  else if (i == 5 && j == 2)
    value = c52;
  else if (i == 5 && j == 3)
    value = c53;
  else if (i == 5 && j == 4)
    value = c54;
  else if (i == 6 && j == 0)
    value = c60;
  else if (i == 6 && j == 1)
    value = c61;
  else if (i == 6 && j == 2)
    value = c62;
  else if (i == 6 && j == 3)
    value = c63;
  else if (i == 6 && j == 4)
    value = c64;
  else if (i == 7 && j == 0)
    value = c70;
  else if (i == 7 && j == 1)
    value = c71;
  else if (i == 7 && j == 2)
    value = c72;
  else if (i == 7 && j == 3)
    value = c73;
  else if (i == 7 && j == 4)
    value = c74;
  return value;
}

// The function setMine randomly assigns a predefined number of mines to the
// field
// Input:
// num_of_mine: The number of mines to be assigned to the field
// Output:
//	None
void setMine(int num_of_mine) {
  int current_mine = 0; // The number of assigned mines so far.
  while (current_mine < num_of_mine) {
    int i = rand() % height;
    int j = rand() % width;

    if (getCell(i, j) != UNFLAGGED_MINE) {
      setCell(i, j, UNFLAGGED_MINE);
      current_mine++;
    }
  }
}

// The initField function initilizes the field with UNKOWN cells and then
// randomly sets some of the cells to be mine (using the setMin function)
// Input:
//	num_of_mine: The number of mines in the field
// Output:
//	None
void initField(int num_of_mine) {
  // Set all cells to UNKOWN
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      setCell(i, j, UNKNOWN);
    }
  }

  // Set the specified number of cells to mines
  setMine(num_of_mine);
}

// The isMine function checks whether a cell is a mine or not
// In our game design a cell is a mine if its state is FLAGGED_MINE or
// UNFLAGGED_MINE
// Input:
// i: The row index of the cell (starting from 0)
// j: The column index of the cell (starting from 0)
// Output:
// Returns true if the cell has a mine (UNFLAGGED_MINE or FLAGGED_MINE)
bool isMine(int i, int j) {
  bool is_mine = false;

  int value = getCell(i, j);

  if (value == FLAGGED_MINE || value == UNFLAGGED_MINE) {
    is_mine = true;
  }
  return is_mine;
}

// The in_bounds function checks whether a cell is a sqaure on the board
// Input:
// i: The row index of the cell (starting from 0)
// j: The
// column index of the cell (starting from 0)
// Output:
// Returns true if the cell
// is within the bounds of the board
bool in_bounds(int i, int j) {
  if (i < height && i > -1 && j < width && j > -1) {
    return true;
  }
  return false;
}

// The number_adjacent_mines function counts how many mines are in the adjacent
// tiles
// Input:
// i: The row index of the cell (starting from 0)
// j: The column index of the cell (starting from 0)
// Output:
// The number of mines in the
// adjacent 8 squares
int number_adjacent_mines(int i, int j) {
  int num_mines = 0;

  // Loop through the adjacent 8 sqaures
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      // if we are looking at the current square
      if (x == 0 && y == 0) {
        continue;
      }

      // if we are out of bounds
      if (!in_bounds(i + x, j + y)) {
        continue;
      }

      if (isMine(i + x, j + y)) {
        num_mines += 1;
      }
    }
  }
  return num_mines;
}

// The reveal function set an appropriate values to each cell based on the
// player move. The function is called when the player decides that a cell is
// not a mine and wants to reveal it. Based on the game description, if the cell
// is not a mine, the user will see the number of mines in the surronding cells
// as the value of the selected cell In a more advanced version, if there is no
// mine in the surrounding cells the program reveals all surrounding cells as
// well.
// Input 	i: the row index 	j: the column index
// Output 	None
void reveal(int i, int j) {

  // Potential recursive case:
  if (number_adjacent_mines(i, j) == 0) {
    setCell(i, j, 0);

    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {

        // if we are looking at the current square
        if (x == 0 && y == 0) {
          continue;
        }

        // if we are out of bounds
        if (!in_bounds(i + x, j + y)) {
          continue;
        }

        // if this square has already been checked
        if (getCell(i + x, j + y) >= 0) {
          continue;
        }

        // If all conditions are met, call reveal on this square
        reveal(i + x, j + y);
      }
    }
  } else {
    // Set the cell with the number of adjacent mines
    setCell(i, j, number_adjacent_mines(i, j));
  }
}

// The executeCmd function takes a command as (action, row, and col) and
// change the state of game and the cells accordingly. the executeCmd may
// change the global variables
// Input:
// action: the command. Fore xample, f, r, or u
// row: the row in which action takes place
// col: the col in which action takes place Output:
// Ouput:
//	None
void executeCmd(char action, int row, int col) {
  // get cell_value
  int cell_value = getCell(row, col);

  switch (action) {
  case 'f':
    if (cell_value == UNFLAGGED_MINE) {
      setCell(row, col, FLAGGED_MINE);
      num_correct_flagged_mine += 1;
    } else if (cell_value == UNKNOWN) {
      setCell(row, col, INCORRECT_FLAGGED_MINE);
      num_incorrect_flagged_mine += 1;
    }
    if (num_incorrect_flagged_mine + num_correct_flagged_mine == num_mine) {
      if (num_incorrect_flagged_mine) {
        explode = true;
      }
    }
    break;

  case 'r':
    if (cell_value == UNFLAGGED_MINE || cell_value == FLAGGED_MINE) {
      explode = true;
    } else {
      reveal(row, col);
    }
    break;

  case 'u':
    if (cell_value == FLAGGED_MINE) {
      setCell(row, col, UNFLAGGED_MINE);
      num_correct_flagged_mine -= 1;
    } else if (cell_value == INCORRECT_FLAGGED_MINE) {
      setCell(row, col, UNKNOWN);
      num_incorrect_flagged_mine -= 1;
    }
  default:
    return;
  }
}
