#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "play_with_human.h"

// Callback functions
void play_with_ai_callback(Fl_Widget *w, void *data)
{
    // ...implement play with AI functionality...
}

void play_with_human_callback(Fl_Widget *w, void *data)
{
    start_play_with_human();
}

void setting_callback(Fl_Widget *w, void *data)
{
    // ...implement settings functionality...
}

void exit_callback(Fl_Widget *w, void *data)
{
    exit(0);
}

int main(int argc, char **argv)
{
    Fl_Window *window = new Fl_Window(400, 300, "Main Menu");
    window->begin();

    Fl_Button *btn_play_with_ai = new Fl_Button(150, 50, 100, 40, "Play with AI");
    btn_play_with_ai->callback(play_with_ai_callback);

    Fl_Button *btn_play_with_human = new Fl_Button(150, 100, 100, 40, "Play with Human");
    btn_play_with_human->callback(play_with_human_callback);

    Fl_Button *btn_setting = new Fl_Button(150, 150, 100, 40, "Setting");
    btn_setting->callback(setting_callback);

    Fl_Button *btn_exit = new Fl_Button(150, 200, 100, 40, "Exit");
    btn_exit->callback(exit_callback);

    window->end();
    window->show(argc, argv);
    return Fl::run();
}