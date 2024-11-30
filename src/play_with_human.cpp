// 要求显示一个15*15的gui棋盘，然后两个玩家轮流下棋，每次后显示棋盘，直到有一方获胜或者和棋。
// 棋盘上的每个交叉点可以落子，每个玩家轮流落子，黑子先手。
// 落子后白棋显示白色圆形，黑棋显示黑色圆形，棋盘显示为灰色。
// 将棋盘的状态保存在一个二维数组中，每个元素的值为0，1，2，分别表示空，黑，白。
// 如果落子位置已经有子，则不能落子，并显示提示信息，需要重新选择位置。
// 如果落子位置违反规则，也不能落子，并显示提示信息，需要重新选择位置。
// 每次落子后将数据传回服务器，服务器判断是否有一方获胜，如果有则返回获胜方，否则返回继续游戏。
// 传出的数据包括棋盘状态，传到/api/board中，以及上一次落子的位置，是否有人胜利，是否有人违反规则，是否有人请求悔棋，是否有人请求求和，是否有人请求认输。
// 服务器返回的数据通过json格式传输，包括以下几个字段：victory，ruleViolation，undoRequest，drawRequest，surrenderRequest。
// 请注意，这个程序不需要实现人机对战，只需要实现人人对战即可。
#include "play_with_human.h"
#include "draw_board.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <jsoncpp/json/json.h>
#include <vector>
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

FBoard gameBoard(BOARD_SIZE);


void start_play_with_human() {
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS");
    ChessBoard *board = new ChessBoard(0, 0, window->w(), window->h());
    window->resizable(board); // Enable resizing for the board
    window->end();
    window->show();
}