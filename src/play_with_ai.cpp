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

void ai_move() {
    // ...code to communicate with server to get AI move...
    // Example: gameBoard.placeStone(ai_row, ai_col);
}

void ai_mode(Fl_Widget *w, void *data) {
    int color = *(int *)data;
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku AI");
    FBoard *inboard = new FBoard(BOARDSIZE);
    ChessBoard *board = new ChessBoard(0, 0, window->w(), window->h(), 2, color, inboard);
    window->resizable(board); // Enable resizing for the board
    window->end();
    window->show();
}

void start_play_with_ai() {
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS AI");

    int *color_ptr1 = new int(BLACK);
    int *color_ptr2 = new int(WHITE);
    
    Fl_Button *start_button1 = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() / 4, ButtonWidth, ButtonHeight, "Start Game as Black");
    start_button1->callback(ai_mode, (void *)color_ptr1);
    start_button1->labelsize(25);
    Fl_Button *start_button2 = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() / 2, ButtonWidth, ButtonHeight, "Start Game as White");
    start_button2->callback(ai_mode, (void *)color_ptr2);
    start_button2->labelsize(25);
    Fl_Button *exit_button = new Fl_Button((window->w() - ButtonWidth) / 2, window->h() * 3 / 4, ButtonWidth, ButtonHeight, "Exit");
    exit_button->callback([](Fl_Widget *w, void *data) {
        Fl_Window *window = (Fl_Window *)data;
        window->hide();
    }, window);
    exit_button->labelsize(25);
    window->end();
    window->show();
}