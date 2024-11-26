#include "play_with_human.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <cstdint>
// 要求显示一个15*15的gui棋盘，然后两个玩家轮流下棋，每次后显示棋盘，直到有一方获胜或者和棋。
// 棋盘上的每个交叉点可以落子，每个玩家轮流落子，黑子先手。
// 落子后白棋显示白色圆形，黑棋显示黑色圆形，棋盘显示为灰色。
// 将棋盘的状态保存在一个二维数组中，每个元素的值为0，1，2，分别表示空，黑，白。
// 如果落子位置已经有子，则不能落子，并显示提示信息，需要重新选择位置。
// 如果落子位置违���规则，也不能落子，并显示提示信息，需要重新选择位置。
// 每次落子后将数据传回服务器，服务器判断是否有一方获胜，如果有则返回获胜方，否则返回继续游戏。
// 请注意，这个程序不需要实现人机对战，只需要实现人人对战即可。

const int BOARD_SIZE = 15;
int boardState[BOARD_SIZE][BOARD_SIZE] = {0}; // 0: 空，1: 黑子，2: 白子
int currentPlayer = 1; // 1: 黑子，2: 白子

void button_callback(Fl_Widget *w, void *data) {
    int index = (intptr_t)data;
    int i = index / BOARD_SIZE;
    int j = index % BOARD_SIZE;

    // 如果当前位置已有棋子，显示提示信息
    if (boardState[i][j] != 0) {
        // ...显示提示信息代码...
        return;
    }

    // 更新棋盘状态
    boardState[i][j] = currentPlayer;

    Fl_Button *btn = (Fl_Button *)w;
    if (currentPlayer == 1) {
        btn->label("@#000 ●"); // 黑子
        currentPlayer = 2;
    } else {
        btn->label("@#FFF ○"); // 白子
        currentPlayer = 1;
    }

    // 将数据传回服务器，判断游戏状态
    // ...与服务器通信的代码...
}

void start_play_with_human() {
    Fl_Window *window = new Fl_Window(600, 600, "五子棋 - 人人对战");
    window->begin();

    Fl_Button *boardButtons[BOARD_SIZE][BOARD_SIZE];

    int btn_size = 40;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int x = j * btn_size;
            int y = i * btn_size;
            boardButtons[i][j] = new Fl_Button(x, y, btn_size, btn_size);
            boardButtons[i][j]->box(FL_UP_BOX); // 改为带边框的样式
            boardButtons[i][j]->color(fl_rgb_color(169, 169, 169)); // 灰色棋盘
            boardButtons[i][j]->callback(button_callback, (void *)(intptr_t)(i * BOARD_SIZE + j));
        }
    }

    window->end();
    window->show();
}