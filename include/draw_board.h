#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include "draw_board.h"
#include "play_with_human.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <jsoncpp/json/json.h>
#include <vector>

class ChessBoard : public Fl_Widget {
private:
    const int MARGIN_FACTOR = 15;
public:
    ChessBoard(int X, int Y, int W, int H);
    void draw();
    int handle(int event);
};

#endif // DRAW_BOARD_H