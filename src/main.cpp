#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
#include "play_with_human.h"
#include "setting.cpp"
#include "play_with_ai.h"

// Callback functions
void play_with_ai_callback(Fl_Widget *w, void *data)
{
    start_play_with_ai();
}

void play_with_human_callback(Fl_Widget *w, void *data)
{
    start_play_with_human();
}

void setting_callback(Fl_Widget *w, void *data)
{
    SettingWindow *setting_window = new SettingWindow(800, 800, "Setting");
    setting_window->end();
    setting_window->show();
}

void exit_callback(Fl_Widget *w, void *data)
{
    exit(0);
}

class MainWindow : public Fl_Window {
public:
    MainWindow(int W, int H, const char* L = 0) : Fl_Window(W, H, L) {
        // Load background image
        bg_image = new Fl_JPEG_Image("assets/images/background.jpg");
        bg_resize_image = bg_image->copy(W, H);
        bg_box = new Fl_Box(0, 0, W, H);
        bg_box->image(bg_resize_image);
        bg_box->box(FL_NO_BOX);
        bg_box->align(FL_ALIGN_CLIP); // Ensure the image is clipped to fit the box

        // Create buttons
        btn_play_with_ai = new Fl_Button(MARGIN, 13 * H / 17, BUTTON_WIDTH, BUTTON_HEIGHT, "Play with AI");
        btn_play_with_ai->color(fl_rgb_color(128, 128, 128));
        btn_play_with_ai->labelsize(25);
        btn_play_with_ai->callback(play_with_ai_callback);

        btn_play_with_human = new Fl_Button(MARGIN, 15 * H / 17, BUTTON_WIDTH, BUTTON_HEIGHT, "Play with Human");
        btn_play_with_human->color(fl_rgb_color(108, 108, 108));
        btn_play_with_human->labelsize(25);
        btn_play_with_human->callback(play_with_human_callback);

        btn_setting = new Fl_Button(W - 250 - MARGIN, 13 * H / 17, BUTTON_WIDTH, BUTTON_HEIGHT, "Setting");
        btn_setting->color(fl_rgb_color(88, 88, 88));
        btn_setting->labelsize(25);
        btn_setting->callback(setting_callback);

        btn_exit = new Fl_Button(W - 250 - MARGIN, 15 * H / 17, BUTTON_WIDTH, BUTTON_HEIGHT, "Exit");
        btn_exit->color(fl_rgb_color(68, 68, 68));
        btn_exit->labelsize(25);
        btn_exit->callback(exit_callback);

        // Ensure background box is behind other widgets
        
    }


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

int main(int argc, char **argv)
{
    MainWindow *window = new MainWindow(800, 800, "Main Menu");
    window->show(argc, argv);
    return Fl::run();
}