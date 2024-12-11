#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include "front_board.h"
#include "draw_board.h"
#include "httplib.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_message.H>
#include <jsoncpp/json/json.h>
#include <vector>
#include <stack>
const int BOARD_SIZE = 15;

void showGameOverMessage(int winner);

class ChessBoard : public Fl_Widget
{
private:
    const int MARGIN_FACTOR = 15;

public:
    int gamecase;
    int user_color;
    FBoard gameBoard;
    Fl_Button *undo_button;
    Fl_Button *save_button;
    Fl_Button *close_button;

    ChessBoard(int X, int Y, int W, int H, int gamecase, int user_color, FBoard* inboard);
    void draw();
    int handle(int event);
};

#endif // DRAW_BOARD_H