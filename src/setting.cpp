#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Widget.H>
#include <string>

class SettingWindow : public Fl_Window {
public:
    SettingWindow(int W, int H, const char* L = 0) : Fl_Window(W, H, L) {
        this->callback(close_callback, this);

        btn_load = new Fl_Button((W - BUTTON_WIDTH) / 2, H / 3, BUTTON_WIDTH, BUTTON_HEIGHT, "Load");
        btn_load->box(FL_ROUND_UP_BOX);
        btn_adjust = new Fl_Button((W - BUTTON_WIDTH) / 2, H / 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Adjust");
        btn_adjust->box(FL_ROUND_UP_BOX);
        btn_exit = new Fl_Button((W - BUTTON_WIDTH) / 2, 2 * H / 3, BUTTON_WIDTH, BUTTON_HEIGHT, "Exit");
        btn_exit->box(FL_ROUND_UP_BOX);

        btn_load->callback(load_callback, this);
        btn_adjust->callback(adjust_callback, this);
        btn_exit->callback(exit_callback, this);
    }

    static void load_callback(Fl_Widget* w, void* data) {
        // 读取存档的逻辑
    }

    static void adjust_callback(Fl_Widget* w, void* data) {
        SettingWindow* window = (SettingWindow*)data;
        window->show_adjust_window();
    }

    static void exit_callback(Fl_Widget* w, void* data) {
        exit(0);
    }

    static void close_callback(Fl_Widget* w, void* data) {
        SettingWindow* window = (SettingWindow*)data;
        window->hide();
        // 将参数传递给主窗口的逻辑
    }

private:
    const int MARGIN = 5;
    const int BUTTON_WIDTH = 100;
    const int BUTTON_HEIGHT = 30;
    Fl_Button* btn_load;
    Fl_Button* btn_adjust;
    Fl_Button* btn_exit;

    void show_adjust_window() {
        Fl_Window* adjust_window = new Fl_Window(300, 200, "Adjust");

        Fl_Choice* bgm_choice = new Fl_Choice(100, 10, 150, 30, "BGM:");
        bgm_choice->add("BGM 1|BGM 2|BGM 3");

        Fl_Spinner* board_size_spinner = new Fl_Spinner(100, 50, 150, 30, "Choose Board Size:");
        board_size_spinner->range(5, 20);
        board_size_spinner->value(10);

        Fl_Button* color_button = new Fl_Button(100, 90, 150, 30, "Choose Color");
        color_button->callback(color_callback, this);

        adjust_window->end();
        adjust_window->show();
    }

    static void color_callback(Fl_Widget* w, void* data) {
        double r, g, b;
        if (fl_color_chooser("Choose", r, g, b)) {
            // 设置背景颜色的逻辑
        }
    }
};