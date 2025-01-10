#include "setting.h"
#include "draw_board.h"
#include "front_board.h"
#include "mainpage.h"
#include <fstream>
#include <iostream>

Mix_Music* g_music; // 全局音乐变量
int choice_index = 0; // 选择的BGM索引
int volume = 80; // 音量

// 窗口设置
SettingWindow::SettingWindow(int W, int H, const char* L) : Fl_Window(W, H, L) {
    // 创建按钮
    btn_load = new Fl_Button((W - BUTTON_WIDTH) / 2, MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT, "Load");
    btn_load->labelsize(50);
    btn_load->box(FL_ROUND_UP_BOX);

    btn_adjust = new Fl_Button((W - BUTTON_WIDTH) / 2, H / 2 - BUTTON_HEIGHT / 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Adjust");
    btn_adjust->box(FL_ROUND_UP_BOX);
    btn_adjust->labelsize(50);

    btn_exit = new Fl_Button((W - BUTTON_WIDTH) / 2, H - BUTTON_HEIGHT - MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT, "Close");
    btn_exit->labelsize(50);
    btn_exit->box(FL_ROUND_UP_BOX);

    // 为按钮设置callback functions
    btn_load->callback(load_callback, this);
    btn_adjust->callback(adjust_callback, this);
    btn_exit->callback(exit_callback, this);
}

// 设置load按钮功能
void SettingWindow::load_callback(Fl_Widget* w, void* data) {
    // 加载savegame.json存档文件
    std::ifstream file("savegame.json");
    if (file.is_open()) {
        Json::Value root;
        file >> root;
        file.close();

        // 利用存档初始化棋盘
        FBoard* board = new FBoard(BOARDSIZE);
        board->currentPlayer = root["currentPlayer"].asInt();
        for (int i = 0; i < BOARDSIZE; ++i) {
            for (int j = 0; j < BOARDSIZE; ++j) {
                board->boardState[i][j] = root["boardState"][i][j].asInt();
            }
        }
        std::cout << "Load game successfully" << std::endl;

        // 开始游戏
        Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS");
        ChessBoard *load_board = new ChessBoard(0, 0, window->w(), window->h(), 1, 0, board);
        window->resizable(load_board);
        window->end();
        window->show();
    }
    else {
        // 抛出异常
        fl_message("No save file available.");
    }
}

//设置adjust按钮功能
void SettingWindow::adjust_callback(Fl_Widget* w, void* data) {
    SettingWindow* window = (SettingWindow*)data;
    window->show_adjust_window();
}

// 设置exit按钮功能
void SettingWindow::exit_callback(Fl_Widget* w, void* data) {
    SettingWindow* window = (SettingWindow*)data;
    window->hide();
}

// 显示adjust窗口
void SettingWindow::show_adjust_window() {
    Fl_Window* adjust_window = new Fl_Window(500, 500, "Adjust");

    // 创建并设置BGM选择元素
    Fl_Choice* bgm_choice = new Fl_Choice(120, 50, 300, 80, "BGM:");
    bgm_choice->add("BGM 1|BGM 2|BGM 3|BGM 4");
    choice_index = bgm_choice->value();
    bgm_choice->labelsize(25);
    bgm_choice->textsize(25);
    bgm_choice->callback([](Fl_Widget* w, void* data) {
        Fl_Choice* choice = (Fl_Choice*)w;
        choice_index = choice->value();
    }, nullptr);

    // 创建并设置音量调节元素
    Fl_Spinner* volume_spinner = new Fl_Spinner(120, 150, 300, 80, "Volume:");
    volume_spinner->minimum(0);
    volume_spinner->maximum(MIX_MAX_VOLUME);
    volume_spinner->value(5);
    volume_spinner->labelsize(25);
    volume_spinner->textsize(25);
    volume_spinner->callback([](Fl_Widget* w, void* data) {
        Fl_Spinner* spinner = (Fl_Spinner*)w;
        volume = spinner->value();
    }, nullptr);
    
    // 创建并设置播放按钮
    Fl_Button* btn_play = new Fl_Button(150, 250, 200, 80, "Play");
    btn_play->callback(bgm_choice_callback);
    btn_play->labelsize(50);
    btn_play->box(FL_ROUND_UP_BOX);

    // 创建并设置关闭按钮
    Fl_Button* btn_close = new Fl_Button(150, 360, 200, 80, "Close");
    btn_close->callback([](Fl_Widget* w, void* data) {
        Fl_Window* window = (Fl_Window*)data;
        window->hide();
    }, adjust_window);
    btn_close->labelsize(50);
    btn_close->box(FL_ROUND_UP_BOX);

    adjust_window->end();
    adjust_window->show();
}

// BGM选择回调函数
void SettingWindow::bgm_choice_callback(Fl_Widget* w, void* data) {
    const char* file = nullptr;
    std::cout << choice_index << std::endl;
    switch (choice_index) {
        case 0:
            file = "assets/audio/bgm1.mp3";
            break;
        case 1:
            file = "assets/audio/bgm2.mp3";
            break;
        case 2:
            file = "assets/audio/bgm3.mp3";
            break;
        case 3:
            file = "assets/audio/bgm4.mp3";
        default:
            file = "assets/audio/bgm.mp3";
            break;
    }

    play_audio(file);
}

// 播放音频文件
void play_audio(const char* file) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    if (g_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(g_music);
        g_music = nullptr;
    }
    g_music = Mix_LoadMUS(file);
    if (!g_music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(g_music, -1);
    
}

// 停止音频播放
void stop_audio() {
    if (g_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(g_music);
        g_music = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}