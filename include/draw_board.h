#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include "front_board.h"
#include "draw_board.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <jsoncpp/json/json.h>
#include <vector>
const int BOARD_SIZE = 15;

class ChessBoard : public Fl_Widget
{
private:
    const int MARGIN_FACTOR = 15;

public:
    int gamecase;
    int user_color;
    FBoard gameBoard = FBoard(BOARD_SIZE);

    ChessBoard(int X, int Y, int W, int H, int gamecase, int user_color);
    void draw();
    int handle(int event);
};

#endif // DRAW_BOARD_H