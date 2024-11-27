#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
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

class MainWindow : public Fl_Window {
public:
    MainWindow(int W, int H, const char* L = 0) : Fl_Window(W, H, L) {
        // Load background image
        bg_image = new Fl_JPEG_Image("assets/images/background.jpg");
        bg_box = new Fl_Box(0, 0, W, H);
        bg_box->image(bg_image);
        bg_box->box(FL_NO_BOX);
        bg_box->align(FL_ALIGN_CLIP); // Ensure the image is clipped to fit the box

        // Create buttons
        btn_play_with_ai = new Fl_Button(W / 2 - 50, H / 5, W / 2, H / 8, "Play with AI");
        btn_play_with_ai->callback(play_with_ai_callback);

        btn_play_with_human = new Fl_Button(W / 2 - 50, 2 * H / 2, W / 8, H / 8, "Play with Human");
        btn_play_with_human->callback(play_with_human_callback);

        btn_setting = new Fl_Button(W / 2 - 50, 3 * H / 5, W / 2, H / 8, "Setting");
        btn_setting->callback(setting_callback);

        btn_exit = new Fl_Button(W / 2 - 50, 4 * H / 5, W / 2, H / 8, "Exit");
        btn_exit->callback(exit_callback);

        // Ensure background box is behind other widgets
        this->begin();
        this->add(bg_box);
        this->end();
    }


private:
    Fl_Box *bg_box;
    Fl_JPEG_Image *bg_image;
    Fl_Button *btn_play_with_ai;
    Fl_Button *btn_play_with_human;
    Fl_Button *btn_setting;
    Fl_Button *btn_exit;
};

int main(int argc, char **argv)
{
    MainWindow *window = new MainWindow(500, 500, "Main Menu");
    window->show(argc, argv);
    return Fl::run();
}