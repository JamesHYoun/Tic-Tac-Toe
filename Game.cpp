#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <climits>
using namespace std;

const int NUM_ROWS = 3;
const int NUM_COLS = 3;
const char BLANK = ' ';
char board[NUM_ROWS][NUM_COLS];

char player;
char computer;

// PRINTING FUNCTIONS

// prints the +---+---+... sequence of the tic tac toe board
void printDivider()
{
	for (int i = 0; i < 4 * NUM_COLS + 1; i++) {
		if (i % 4 == 0) cout << '+';
		else cout << '-';
	}
}

void printRow(int row)
{
	int col = 0;
	for (int j = 0; j < 4 * NUM_COLS + 1; j++) {
		if (j % 4 == 0) cout << '|';
		else if ((j + 2) % 4 == 0) {
			cout << board[row][col++];
		}
		else cout << ' ';
	}
}

void printBoard()
{
	int row = 0;
	int col = 0;
	int shift = 2;
	for (int i = 0; i < 4 * NUM_ROWS + 1; i++) {
		if (i > 0 && (i + 2 + shift) % 4 == 0) {
			cout << col++;
		}
		else cout << ' ';
	}
	cout << endl;
	col = 0;
	for (int i = 0; i < 2 * NUM_ROWS + 1; i++) {
		if (i % 2 == 0) {
			cout << "  "; 
			printDivider();
		}
		else {
			cout << row << " "; 
			printRow(row);
			row++;
		}
		cout << endl;
	}
}

///////////////////////////

// TESTING FUNCTIONS

void randomBoard()
{
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			int oc = rand() % 3;
			if (oc == 0) board[i][j] = 'X';
			else if (oc == 1) board[i][j] = 'O';
			else board[i][j] = BLANK;
		}
	}
}

void test(int numTests)
{
	for (int i = 0; i < numTests; i++) {
		randomBoard();
		printBoard();
	}
}

///////////////////////////

// GAME SETTING UP FUNCTIONS

void init()
{
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			board[i][j] = ' ';
		}
	}
}

void setup(char& fst)
{
	cout << "Would you like to go first? (y, n): ";
	cin >> fst;
	cin.ignore();
	cout << "Choose character: ";
	char ch;
	cin >> ch;
	cin.ignore();
	if (ch == 'X') {
		player = 'X';
		computer = 'O';
	}
	else {
		player = 'O';
		computer = 'X';
	}
}

///////////////////////////

// CHECKING TERMINATION FUNCTIONS

bool isFull(char brd[NUM_ROWS][NUM_COLS] = board)
{
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (brd[i][j] == BLANK) {
				return false;
			}
		}
	}
	return true;
}

// Returns true if row is filled with same mark, false otherwise.
bool checkRow(int row, char brd[NUM_ROWS][NUM_COLS] = board) 
{
	for (int j = 1; j < NUM_COLS; j++) {
		if (brd[row][j] == BLANK || brd[row][j - 1] != brd[row][j]) return false;
	}
	return true;
}

// Returns true if col is filled with same mark, false otherwise.
bool checkCol(int col, char brd[NUM_ROWS][NUM_COLS] = board) 
{
	for (int i = 1; i < NUM_ROWS; i++) {
		if (brd[i][col] == BLANK || brd[i - 1][col] != brd[i][col]) return false;
	}
	return true;
}

// Returns true if at least one of the diagonals is filled with same mark, false otherwise.
bool checkDiag(char brd[NUM_ROWS][NUM_COLS] = board)
{
	// check left diagonal
	bool isStraight = true;
	for (int i = 1; i < NUM_ROWS; i++) {
		if (brd[i][i] == BLANK || brd[i - 1][i - 1] != brd[i][i]) {
			isStraight = false;
			break;
		}
	}
	if (isStraight) return true;
	// check right diagonal
	for (int i = 1; i < NUM_COLS; i++) {
		if (brd[i][NUM_COLS - i - 1] == BLANK || brd[i - 1][NUM_COLS - i] != brd[i][NUM_COLS - i - 1]) {
			return false;
		}
	}
	return true;
}

bool isFinished(char brd[NUM_ROWS][NUM_COLS] = board)
{

	if (isFull(brd)) return true;

	// check if rows have a straight
	for (int i = 0; i < NUM_ROWS; i++) {
		if (checkRow(i, brd)) return true;
	}

	// check if cols have a straight
	for (int i = 0; i < NUM_ROWS; i++) {
		if (checkCol(i, brd)) return true;
	}

	// check if either diagonals contain a straight
	return checkDiag(brd);
}

///////////////////////////

//GAME PLAYING FUNCTIONS

int eval()
{
	//if row is straight, then return whoever won
	for (int i = 0; i < NUM_ROWS; i++) {
		if (checkRow(i)) {
			if (board[i][0] == computer) return 1;
			return -1;
		}
	}

	//if row is straight, then return whoever won
	for (int i = 0; i < NUM_ROWS; i++) {
		if (checkCol(i)) {
			if (board[0][i] == computer) return 1;
			return -1;
		}
	}

	//check if either diagonal is straight, then return whoever won
	if (checkDiag()) {
		if (board[NUM_ROWS / 2][NUM_COLS / 2] == computer) return 1;
		return -1;
	}

	return 0;
}

int minimax(char, int, int);

// Simulates every possible move of compuer, returns best value from computer's move and sets moveR and moveC to that move
int moveComputer(int& moveR, int& moveC, int beta, int depth)
{
	int best = -1 * INT_MAX;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (board[i][j] == BLANK) {
				board[i][j] = computer;
				int val = minimax(player, best, depth + 1);
				board[i][j] = BLANK;
				if (val >= 0 && val >= beta) return INT_MAX; //alpha beta pruning
				if (best <= 0 && val > best) {
					best = val;
					moveR = i; moveC = j;
				}
				else if (best > 0 && val > 0 && val < best) {
					best = val;
					moveR = i; moveC = j;
				}
			}
		}
	}
	return best;
}

// Same as moveComputer, except simulates the player's moves for the computer to determine its best move.
int movePlayer(int& moveR, int& moveC, int alpha, int depth)
{
	int best = INT_MAX;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (board[i][j] == BLANK) {
				board[i][j] = player;
				int val = minimax(computer, best, depth + 1);
				board[i][j] = BLANK;
				if (val <= 0 && val <= alpha) return INT_MIN; //alpha beta pruning
				if (best >= 0 && val < best) {
					best = val;
					moveR = i; moveC = j;
				}
				else if (best < 0 && val < 0 && val > best) {
					best = val;
					moveR = i; moveC = j;
				}
			}
		}
	}
	return best;
}

int minimax(char cp, int ab, int depth) //ab stands for alpha/beta
{
	if (isFinished()) {
		return eval() * depth;
	}
	int moveR = -1; int moveC = -1;
	int best;
	if (cp == computer) {
		best = moveComputer(moveR, moveC, ab, depth);
		if (depth == 0) board[moveR][moveC] = computer;
	}
	else {
		best = movePlayer(moveR, moveC, ab, depth);
		if (depth == 0) board[moveR][moveC] = player;
	}
	return best;
}

bool isMoveValid(int r, int c)
{
	return board[r][c] == BLANK && 0 <= r && r < NUM_ROWS && 0 <= c && c < NUM_COLS;
}

void playerTurn()
{
	int pR; int pC;
	bool valid;
	do { //ensures player makes move on blank square
		cout << "Make move (row# space col#): ";
		cin >> pR >> pC;
		cin.ignore();
		valid = isMoveValid(pR, pC);
		if (!valid) cout << "Not valid move!!\n";
	} while (!valid);
	board[pR][pC] = player;
}

///////////////////////////

int main()
{
	char cont;
	do { //keeps on playing game while user wants to
		char fst;
		init();
		setup(fst);
		bool finished = false;
		do { //plays the game until finish
			if (fst == 'y') { //Player goes first
				printBoard();
				playerTurn();
				minimax(computer, INT_MAX, 0);
				finished = isFinished();
			}
			else { //Computer goes first
				minimax(computer, INT_MAX, 0);
				finished = isFinished();
				if (finished) break;
				printBoard();
				playerTurn();
			}
		} while (!finished);
		printBoard();
		cout << "Play again? (y, n): ";
		cin >> cont;
	} while (cont == 'y');
}
