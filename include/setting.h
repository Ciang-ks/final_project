#ifndef SETTING_H
#define SETTING_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_message.H>
#include <string>
#include <jsoncpp/json/json.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void play_audio(const char* file);
void stop_audio();

class SettingWindow : public Fl_Window {
public:
    SettingWindow(int W, int H, const char* L = 0);

private:
    const int BUTTON_WIDTH = 400;
    const int BUTTON_HEIGHT = 150;
    const int MARGIN = 100;
    Fl_Button* btn_load;
    Fl_Button* btn_adjust;
    Fl_Button* btn_exit;

    void show_adjust_window();
    static void load_callback(Fl_Widget* w, void* data);
    static void adjust_callback(Fl_Widget* w, void* data);
    static void exit_callback(Fl_Widget* w, void* data);
    static void bgm_choice_callback(Fl_Widget* w, void* data);
};

#endif // SETTING_H