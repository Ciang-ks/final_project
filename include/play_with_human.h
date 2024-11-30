#ifndef PLAY_WITH_HUMAN_H
#define PLAY_WITH_HUMAN_H

#include <FL/Fl_Widget.H>
#include <vector>

class FBoard {
public:
    std::vector<std::vector<int>> boardState;
    int currentPlayer;
    int nowRow, nowCol;

    FBoard(int size);
    bool placeStone(int row, int col);
};

extern FBoard gameBoard;

void start_play_with_human();
void button_callback(Fl_Widget *w, void *data);

#endif // PLAY_WITH_HUMAN_H