
#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
#include "front_board.h"

void play_with_ai_callback(Fl_Widget *w, void *data);
void play_with_human_callback(Fl_Widget *w, void *data);
void setting_callback(Fl_Widget *w, void *data);
void exit_callback(Fl_Widget *w, void *data);

class MainWindow : public Fl_Window {
public:
    MainWindow(int W, int H, const char* L = 0);
    void set_board(FBoard* board);
    FBoard* board = nullptr;

private:
    Fl_Box *bg_box;
    Fl_JPEG_Image *bg_image;
    Fl_Image *bg_resize_image;
    Fl_Button *btn_play_with_ai;
    Fl_Button *btn_play_with_human;
    Fl_Button *btn_setting;
    Fl_Button *btn_exit;
    const int MARGIN = 5;
    const int BUTTON_WIDTH = 250;
    const int BUTTON_HEIGHT = 90;
};

#endif // MAINPAGE_H