#include "front_board.h"
using namespace std;
const int BOARD_SIZE = 15;

FBoard::FBoard(int size) : boardState(size, vector<int>(size, 0)), currentPlayer(1) {}

    
bool FBoard::placeStone(int row, int col) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && boardState[row][col] == 0) {
        boardState[row][col] = currentPlayer;
        currentPlayer = 3 - currentPlayer; // Switch player
        return true;
    }
    return false;
}


