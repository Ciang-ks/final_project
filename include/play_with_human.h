
#ifndef PLAY_WITH_HUMAN_H
#define PLAY_WITH_HUMAN_H

#include <FL/Fl_Widget.H>

extern const int BOARD_SIZE;
extern int boardState[][15];
extern int currentPlayer;

void start_play_with_human();
void button_callback(Fl_Widget *w, void *data);

#endif // PLAY_WITH_HUMAN_H