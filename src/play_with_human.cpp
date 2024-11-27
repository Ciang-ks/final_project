#include "play_with_human.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
// 要求显示一个15*15的gui棋盘，然后两个玩家轮流下棋，每次后显示棋盘，直到有一方获胜或者和棋。
// 棋盘上的每个交叉点可以落子，每个玩家轮流落子，黑子先手。
// 落子后白棋显示白色圆形，黑棋显示黑色圆形，棋盘显示为灰色。
// 将棋盘的状态保存在一个二维数组中，每个元素的值为0，1，2，分别表示空，黑，白。
// 如果落子位置已经有子，则不能落子，并显示提示信息，需要重新选择位置。
// 如果落子位置违反规则，也不能落子，并显示提示信息，需要重新选择位置。
// 每次落子后将数据传回服务器，服务器判断是否有一方获胜，如果有则返回获胜方，否则返回继续游戏。
// 请注意，这个程序不需要实现人机对战，只需要实现人人对战即可。

const int BOARD_SIZE = 15;
const int MARGIN_FACTOR = 15;
int boardState[BOARD_SIZE][BOARD_SIZE] = {0}; // 0: empty, 1: black, 2: white
int currentPlayer = 1; // 1: black, 2: white

class ChessBoard : public Fl_Widget {
public:
    ChessBoard(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}

    void draw() {
        // 调整边距以及网格大小
        int margin = w() / MARGIN_FACTOR; // 调节合适的边距
        int grid_size = (w() - 2 * margin) / (BOARD_SIZE - 1);

        // 绘制背景
        fl_color(fl_rgb_color(0xE0, 0xA9, 0x6A)); // Light brown
        fl_rectf(x(), y(), w(), h());

        // 绘制网格
        fl_color(FL_BLACK);
        for (int i = 0; i < BOARD_SIZE; ++i) {
            int posX = margin + i * grid_size;
            fl_line(margin, posX, w() - margin, posX); // Horizontal lines
            fl_line(posX, margin, posX, h() - margin); // Vertical lines
        }

        // Draw stones
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (boardState[i][j] != 0) {
                    fl_color(boardState[i][j] == 1 ? FL_BLACK : FL_WHITE);
                    fl_pie(
                        margin + j * grid_size - grid_size / 2,
                        margin + i * grid_size - grid_size / 2,
                        grid_size, grid_size, 0, 360
                    );
                }
            }
        }
    }

    int handle(int event) {
        switch(event) {
            case FL_PUSH: {
                // Calculate margin and grid size based on current widget size
                int margin = w() / 15; // Same as in draw()
                int grid_size = (w() - 2 * margin) / (BOARD_SIZE - 1);

                int mx = Fl::event_x();
                int my = Fl::event_y();
                int col = (mx - margin + grid_size / 2) / grid_size;
                int row = (my - margin + grid_size / 2) / grid_size;

                if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
                    if (boardState[row][col] == 0) {
                        boardState[row][col] = currentPlayer;
                        currentPlayer = 3 - currentPlayer; // Switch player
                        redraw();
                        // Send data to server to check game status
                        // ...code to communicate with server...
                    } else {
                        // ...display message: position already occupied...
                    }
                }
                return 1;
            }
            default:
                return Fl_Widget::handle(event);
        }
    }
};

void start_play_with_human() {
    Fl_Window *window = new Fl_Window(800, 800, "Gomoku HumanVS");
    ChessBoard *board = new ChessBoard(0, 0, window->w(), window->h());
    window->resizable(board); // Enable resizing for the board
    window->end();
    window->show();
}