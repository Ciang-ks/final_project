#ifndef FRONT_BOARD_H
#define FRONT_BOARD_H
#include <FL/Fl_Widget.H>
#include <vector>
const int EMPTY = 0;
const int BLACK = 1;
const int WHITE = 2;
const int notOver = 200;
const int flatFlag = 100;
const int BOARDSIZE = 15;

using namespace std;

class FBoard {
public:
    vector<vector<int>> boardState;
    int currentPlayer;

    FBoard(int size);
    bool placeStone(int row, int col);
};

#endif // FRONT_BOARD_H