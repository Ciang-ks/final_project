#include "draw_board.h"
#include "front_board.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <jsoncpp/json/json.h>
#include <vector>


ChessBoard::ChessBoard(int X, int Y, int W, int H, int ingamecase, int inuser_color) : Fl_Widget(X, Y, W, H), gameBoard(BOARD_SIZE)
{
    this->gamecase = ingamecase;
    this->user_color = inuser_color;
}

void ChessBoard::draw()
{
    // 调整边距以及网格大小
    int margin = w() / MARGIN_FACTOR; // 调节合适的边距
    int grid_size = (w() - 2 * margin) / (BOARD_SIZE - 1);

    // 绘制��景
    fl_color(fl_rgb_color(0xE0, 0xA9, 0x6A)); // Light brown
    fl_rectf(x(), y(), w(), h());

    // 绘制网格
    fl_color(FL_BLACK);
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        int posX = margin + i * grid_size;
        fl_line(margin, posX, w() - margin, posX); // Horizontal lines
        fl_line(posX, margin, posX, h() - margin); // Vertical lines
    }

    // Draw stones
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (gameBoard.boardState[i][j] != 0)
            {
                fl_color(gameBoard.boardState[i][j] == BLACK ? FL_BLACK : FL_WHITE);
                fl_pie(
                    margin + j * grid_size - grid_size / 2,
                    margin + i * grid_size - grid_size / 2,
                    grid_size, grid_size, 0, 360);
            }
        }
    }
}

int ChessBoard::handle(int event)
{
    switch (event)
    {
    case FL_PUSH:
    {
        // Calculate margin and grid size based on current widget size
        int margin = w() / 15; // Same as in draw()
        int grid_size = (w() - 2 * margin) / (BOARD_SIZE - 1);
        int mx = Fl::event_x();
        int my = Fl::event_y();
        int col = (mx - margin + grid_size / 2) / grid_size;
        int row = (my - margin + grid_size / 2) / grid_size;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        {
            if (gamecase == 1)
            {
                if (gameBoard.placeStone(row, col))
                {
                    redraw();
                    // Send data to server to check game status
                    // ...code to communicate with server...
                }
            }

            else if (gamecase == 2 && gameBoard.currentPlayer == user_color)
            {
                if (gameBoard.placeStone(row, col))
                {
                    redraw();
                    // Send data to server to check game status
                    // ...code to communicate with server...
                    // AI move
                    // Send data to server to check game status
                    // ...code to communicate with server...
                }
            }
            else
            {
                // Get move from ai
                int x = 0, y = 0;
                if (gameBoard.placeStone(x, y))
                {
                    redraw();
                    // Send data to server to check game status
                    // ...code to communicate with server...
                }
            }
        }
        return 1;
    }
    default:
        return Fl_Widget::handle(event);
    }
}

