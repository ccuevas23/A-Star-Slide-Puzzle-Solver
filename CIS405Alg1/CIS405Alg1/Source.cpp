//Christian Cuevas Algorithm 1
//CIS 405

using namespace std;
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <functional>
#include <stdlib.h>
#include <ctime>


struct Move {			//Struct to store a move, easy to put into priority queue and map, keep track of direction
	int evalFunction;
	char moveState[3][3];
	char dir;
};

auto cmp = [](Move a, Move b) {					//Compare function to correctly put the right move at the top of priority queue
	return a.evalFunction > b.evalFunction;
};

priority_queue<Move, vector<Move>, decltype(cmp)> Q(cmp);		//Main priority queue


//Class for the gameboard and all of the functions needed to solve it
class GameBoard {
public:
	char goalState[3][3] = { {'1', '2', '3'}, {'8', '-', '4'}, {'7', '6', '5'} };		//Initiating the initial board state and the goal state we want to reach
	char initState[3][3] = { {'7', '1', '4'}, {'-', '3', '5'}, {'2', '8', '6'} };		//Along with all other variables
	char tieHelp[3][3];

	bool end = false;
	bool exp = false;
	int numMoves = 1;
	int pathCost = 0;
	int tiePath = 0;
	vector<Move> exploredSet;

	GameBoard() {		//Constructor to put first move in the explored set map
		Move init;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				init.moveState[i][j] = initState[i][j];
			}
		}

		exploredSet.push_back(init);
	}


	vector<char> findMoves(char iniState[3][3]) {		//Function that returns a vector of the characters that could move from the current state of the puzzle
		int x, y;
		vector<char> moves;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (iniState[i][j] == '-') {
					x = i;
					y = j;
				}
			}
		}

		if (x == 0) {				//Testing all possible movements in the board
			if (y == 0) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y + 1]);
			}
			else if (y == 1) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y + 1]);
				moves.push_back(iniState[x][y - 1]);
			}
			else if (y == 2) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y - 1]);
			}
		}
		else if (x == 1) {
			if (y == 0) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y + 1]);
				moves.push_back(iniState[x - 1][y]);
			}
			else if (y == 1) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y + 1]);
				moves.push_back(iniState[x][y - 1]);
				moves.push_back(iniState[x - 1][y]);
			}
			else if (y == 2) {
				moves.push_back(iniState[x + 1][y]);
				moves.push_back(iniState[x][y - 1]);
				moves.push_back(iniState[x - 1][y]);
			}
		}
		else if (x == 2) {
			if (y == 0) {
				moves.push_back(iniState[x - 1][y]);
				moves.push_back(iniState[x][y + 1]);
			}
			else if (y == 1) {
				moves.push_back(iniState[x - 1][y]);
				moves.push_back(iniState[x][y + 1]);
				moves.push_back(iniState[x][y - 1]);
			}
			else if (y == 2) {
				moves.push_back(iniState[x - 1][y]);
				moves.push_back(iniState[x][y - 1]);
			}
		}


		return moves;
	}

	int findHeur(char nextState[3][3]) {		//Finds the heuristic function of nextState board
		char temp;
		int x, y;
		int rowSub, colSub;
		int result = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp = nextState[i][j];

				if (temp != '-') {
					for (int t = 0; t < 3; t++) {
						for (int r = 0; r < 3; r++) {
							if (temp == goalState[t][r]) {
								x = t;
								y = r;
								break;
							}
						}
					}

					if ((i >= x) && (j >= y)) {
						rowSub = i - x;
						colSub = j - y;

						result += (rowSub * 1) + (colSub * 2);
					}
					else if ((i >= x) && (y >= j)) {
						rowSub = i - x;
						colSub = y - j;

						result += (rowSub * 1) + (colSub * 2);
					}
					else if ((x >= i) && (y >= j)) {
						rowSub = x - i;
						colSub = y - j;

						result += (rowSub * 3) + (colSub * 2);
					}
					else if ((x >= i) && (j >= y)) {
						rowSub = x - y;
						colSub = j - y;

						result += (rowSub * 3) + (colSub * 2);
					}
				}

			}
		}

		if (numMoves == 1) { result = result - 3; }

		return result;
	}



	int evalFunc(char currState[3][3], char direction) {		//Finds the total evaluation function
		int eval = findHeur(currState);
		if (direction == 'e' || direction == 'w') {
			eval += 1;
		}
		else if (direction == 's') {
			eval += 1;
		}
		else {
			eval += 1;
		}

		return eval;
	}

	void makeMove(vector<char> moves) {			//Makes the next move, decides which is the best move
		int currRow, currCol, emptRow, emptCol;
		char direction;
		char tempBoard[3][3];

		Move frontier;


		for (int t = 0; t < moves.size(); t++) {	//For all possible moves

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					frontier.moveState[i][j] = initState[i][j];

					if (initState[i][j] == moves[t]) {
						currRow = i;
						currCol = j;
					}
					else if (initState[i][j] == '-') {
						emptRow = i;
						emptCol = j;
					}
				}
			}

			if (currRow < emptRow) {		//Declaring the direction
				direction = 's';

			}
			else if (currRow > emptRow) {
				direction = 'n';

			}
			else if (currCol < emptCol) {
				direction = 'e';

			}
			else {
				direction = 'w';

			}

			frontier.moveState[emptRow][emptCol] = moves[t];
			frontier.moveState[currRow][currCol] = '-';

			frontier.evalFunction = evalFunc(frontier.moveState, direction);

			frontier.dir = direction;

			if (!checkExplored(frontier)) {
				Q.push(frontier);
			}

		}

		//Making the actual move
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				initState[i][j] = Q.top().moveState[i][j];
			}
		}


		tiePath = pathCost;

		if (Q.top().dir == 's') {		//Updating the path cost

			pathCost += 1;

		}
		else if (Q.top().dir == 'e' || Q.top().dir == 'w') {
			pathCost += 1;
		}
		else {
			pathCost += 1;
		}

		if (Q.top().dir == 's') {		//Using the tie path cost to help with outputting the correct path cost
			tiePath += 1;
		}
		else if (Q.top().dir == 'e' || Q.top().dir == 'w') {
			tiePath += 1;
		}
		else {
			tiePath += 1;
		}

		numMoves++;

		exploredSet.push_back(Q.top());
		Q.pop();

		printBoard(initState);


	}

	void solvePuzzle() {	//Function to solve entire puzzle
		vector<char> moves;

		while (!checkGoal() && !end) {
			moves = findMoves(initState);

			makeMove(moves);
		}

	}

	bool compareBoard(char left[3][3], char right[3][3]) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (left[i][j] != right[i][j]) {
					return false;
				}
			}
		}
		return true;
	}


	bool checkExplored(Move m) {	//Checks if the move has been used yet
		for (int t = 0; t < exploredSet.size(); t++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {

					if (compareBoard(exploredSet[t].moveState, m.moveState)) {
						return true;

					}

				}

			}
		}

		return false;
	}

	bool checkGoal() {		//Checks if the game board is at the goal state
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (initState[i][j] != goalState[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	bool checkGoal(char thisBoard[3][3]) {		//Checks if the game board is at the goal state
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (thisBoard[i][j] != goalState[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	void printBoard(char thisBoard[3][3]) {		//Prints thisBoard matrix

		cout << "________" << endl;
		for (int i = 0; i < 3; i++) {
			cout << "|";
			for (int j = 0; j < 3; j++) {
				cout << thisBoard[i][j] << " ";
			}
			cout << "|" << endl;
		}

		cout << "________" << endl;
		if (numMoves != 2) {
			cout << pathCost << " | " << findHeur(thisBoard) << endl;
		}
		else {
			cout << pathCost << " | " << findHeur(thisBoard) - 3 << endl;
		}
		cout << "#" << numMoves << endl << endl;

	}
};


int main() {		

	GameBoard a = GameBoard();

	a.printBoard(a.initState);
	clock_t start = clock();
	a.solvePuzzle();
	clock_t end = clock();
	double time = 1000.0 * (end - start) / CLOCKS_PER_SEC;
	cout << "CPU time used: " << time << " ms"<< endl;
	cout << "Space Used: " << a.exploredSet.size() + Q.size() << " 3x3 arrays saved in memory" << endl;


}