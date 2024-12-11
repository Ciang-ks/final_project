#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include "play_with_human.h"
#include "front_board.h"
#include "setting.h"
#include "play_with_ai.h"
#include "mainpage.h" // Include the new header file



int main(int argc, char **argv)
{
    MainWindow *window = new MainWindow(800, 800, "Main Menu");
    window->show(argc, argv);
    return Fl::run();
}