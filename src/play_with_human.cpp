#include "front_board.h"
#include "draw_board.h"
#include <jsoncpp/json/json.h>
#include <vector>
using namespace std;






void start_play_with_human() {
    // 创建一个 800x800 的窗口，标题为 "Gomoku HumanVS"
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS");
    // 创建一个 FBoard 对象，棋盘大小为 BOARDSIZE
    FBoard *inboard = new FBoard(BOARDSIZE);
    // 创建一个 ChessBoard 对象，设置其位置和大小，并关联到窗口和 FBoard 对象
    ChessBoard *board = new ChessBoard(0, 0, window->w(), window->h(), 1, 0, inboard);
    // 使窗口大小可调整
    window->resizable(board);
    window->end();
    // 显示窗口
    window->show();
}