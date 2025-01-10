#include "play_with_ai.h"
#include "front_board.h"
#include "draw_board.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <vector>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
using namespace std;

const int ButtonWidth = 500;
const int ButtonHeight = 100;

// AI模式的回调函数，创建一个新的窗口并显示棋盘
void ai_mode(Fl_Widget *w, void *data) {
    int color = *(int *)data;
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku AI");
    FBoard *inboard = new FBoard(BOARDSIZE);
    ChessBoard *board = new ChessBoard(0, 0, window->w(), window->h(), 2, color, inboard);
    window->resizable(board); // 使棋盘可调整大小
    window->end();
    window->show();
}

// 启动与AI对战的界面
void start_play_with_ai() {
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS AI");

    int *color_ptr1 = new int(BLACK); // 黑色棋子
    int *color_ptr2 = new int(WHITE); // 白色棋子
    
    // 创建开始游戏按钮（黑色）
    Fl_Button *start_button1 = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() / 4, ButtonWidth, ButtonHeight, "Start Game as Black");
    start_button1->callback(ai_mode, (void *)color_ptr1);
    start_button1->labelsize(25);
    
    // 创建开始游戏按钮（白色）
    Fl_Button *start_button2 = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() / 2, ButtonWidth, ButtonHeight, "Start Game as White");
    start_button2->callback(ai_mode, (void *)color_ptr2);
    start_button2->labelsize(25);
    
    // 创建退出按钮
    Fl_Button *exit_button = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() * 3 / 4, ButtonWidth, ButtonHeight, "Exit");
    exit_button->callback([](Fl_Widget *w, void *data) {
        Fl_Window *window = (Fl_Window *)data;
        window->hide();
    }, window);
    exit_button->labelsize(25);
    
    window->end();
    window->show();

    // 设置窗口关闭时的回调函数，释放内存
    window->callback([](Fl_Widget *w, void *data) {
        delete (int *)data;
    }, color_ptr1);
    window->callback([](Fl_Widget *w, void *data) {
        delete (int *)data;
    }, color_ptr2);
}