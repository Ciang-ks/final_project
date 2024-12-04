#include "draw_board.h"
#include "front_board.h"
#include "httplib.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <jsoncpp/json/json.h>
#include <vector>
#include <curl/curl.h>

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

    // 绘制背景
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

int sendGameState(const FBoard& gameBoard, int col, int row) {
    Json::Value jsonData;
    jsonData["currentPlayer"] = gameBoard.currentPlayer;
    jsonData["col"] = col;
    jsonData["row"] = row;
    
    Json::Value board(Json::arrayValue);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        Json::Value jsonRow(Json::arrayValue);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            jsonRow.append(gameBoard.boardState[i][j]);
        }
        board.append(jsonRow);
    }
    jsonData["board"] = board;

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonData);

    httplib::Client cli("localhost", 8080);
    auto res = cli.Post("/api/board", jsonString, "application/json");
    if (!res || res->status != 200) {
        std::cerr << "Failed to send game state: " << (res ? res->status : 0) << std::endl;
    } else {
        Json::Value responseJson;
        Json::Reader reader;

        if (reader.parse(res->body, responseJson)) {
            int isGameOver = responseJson.asInt();
            std::cout << "Response: " << isGameOver << std::endl;
            return isGameOver;
        } else {
            std::cerr << "Failed to parse response JSON" << std::endl;
        }
    }
    return -1;
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
                    cout << sendGameState(gameBoard, col, row) << endl;
                }
            }
            else if (gamecase == 2 && gameBoard.currentPlayer == user_color)
            {
                if (gameBoard.placeStone(row, col))
                {
                    redraw();
                    if (sendGameState(gameBoard, col, row) % 100 == 0)
                    {
                        // Game over
                        // ...code to display game over message...
                    };
                    // AI move
                    // ...code to get AI move...
                    int ai_col = 0, ai_row = 0; // Replace with actual AI move
                    if (gameBoard.placeStone(ai_row, ai_col))
                    {
                        redraw();
                        if (sendGameState(gameBoard, ai_col, ai_row) % 100 == 0)
                        {
                            // Game over
                            // ...code to display game over message...
                        };
                    }
                }
            }
        }
        return 1;
    }
    default:
        return Fl_Widget::handle(event);
    }
}

