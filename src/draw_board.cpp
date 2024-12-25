#include "draw_board.h"
std::stack<std::pair<int, int>> moves;


ChessBoard::ChessBoard(int X, int Y, int W, int H, int ingamecase, int inuser_color, FBoard* inboard) : Fl_Widget(X, Y, W, H), gameBoard(BOARD_SIZE)
{
    this->gamecase = ingamecase;
    this->user_color = inuser_color;
    this->gameBoard = *inboard;
    this->lock = false;

    // Add buttons
    int button_width = 100;
    int button_height = 30;

    undo_button = new Fl_Button(X + 5, Y + 5, button_width, button_height, "Undo");
    undo_button->callback([](Fl_Widget *w, void *data) {
        ChessBoard *board = (ChessBoard *)data;
        int nowrow = moves.top().first;
        int nowcol = moves.top().second;
        moves.pop();
        board->gameBoard.boardState[nowrow][nowcol] = EMPTY;
        board->gameBoard.currentPlayer = board->gameBoard.currentPlayer == BLACK ? WHITE : BLACK;
        board->redraw();
    }, this);
    undo_button->labelsize(25);
    undo_button->box(FL_ROUND_UP_BOX);

    save_button = new Fl_Button(X + W - button_width - 5, Y + 5, button_width, button_height, "Save");
    save_button->callback([](Fl_Widget *w, void *data) {
    ChessBoard *board = (ChessBoard *)data;
    Json::Value jsonData;
    jsonData["currentPlayer"] = board->gameBoard.currentPlayer;

    Json::Value boardState(Json::arrayValue);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        Json::Value row(Json::arrayValue);
        for (int j = 0; j < BOARD_SIZE; ++j) {
        row.append(board->gameBoard.boardState[i][j]);
        }
        boardState.append(row);
    }
    jsonData["boardState"] = boardState;

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonData);

    std::ofstream file("savegame.json");
    if (file.is_open()) {
        file << jsonString;
        file.close();
        fl_message("Game saved successfully!");
    } else {
        fl_message("Failed to save game.");
    }
    }, this);
    save_button->labelsize(25);
    save_button->box(FL_ROUND_UP_BOX);

    close_button = new Fl_Button(X + W - button_width - 5, Y + H - button_height - 5, button_width, button_height, "Close");
    close_button->callback([](Fl_Widget *w, void *data) {
        // Close button callback
        Fl_Window *win = (Fl_Window *)data;
        win->hide();
    }, this->window());
    close_button->labelsize(25);
    close_button->box(FL_ROUND_UP_BOX);
}

void ChessBoard::draw()
{
    // 调整边距以及网格大小
    int margin = w() / MARGIN_FACTOR; // 调节合适的边距
    int grid_size = (w() - 2 * margin) / (BOARD_SIZE - 1);

    // 绘制背景
    fl_push_clip(40, 40, w() - 80, h() - 80); // Limit redraw area to the board
    fl_color(fl_rgb_color(0xE0, 0xA9, 0x6A)); // Light brown
    fl_rectf(x(), y(), w(), h());
    fl_pop_clip();

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
            if ((i == 7 && j == 7) || (i == 3 && j == 3) || (i == 3 && j == 11) || (i == 11 && j == 3) || (i == 11 && j == 11))
            {
                fl_color(FL_BLACK);
                fl_pie(
                    margin + j * grid_size - 4,
                    margin + i * grid_size - 4,
                    8, 8, 0, 360);
            }
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

void showGameOverMessage(int winner) {
    int W = 800;
    int H = 600;
    Fl_Window *win = new Fl_Window(W, H, "Game Over");
    std::string message = (winner == BLACK) ? "Black wins!" : "White wins!";
    int margin = 50;
    Fl_Box *box = new Fl_Box(margin, margin, W - 2 * margin, H - 2 * margin - 50, message.c_str());
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(50);
    box->labeltype(FL_SHADOW_LABEL);

    Fl_Button *button = new Fl_Button(W / 2 - 50, H - 70, 100, 30, "OK");
    button->callback([](Fl_Widget *w, void *data) {
        Fl_Window *win = (Fl_Window *)data;
        win->hide();
    }, win);

    win->end();
    win->set_modal();
    win->show();
    Fl::run();
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
        } 
        else {
            std::cerr << "Failed to parse response JSON" << std::endl;
        }
    }
    return -1;
}

int sendAIRequest(const FBoard& gameBoard, int col, int row) {
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

    httplib::Client cli("localhost", 7070);
    auto res = cli.Post("/api/ai", jsonString, "application/json");
    if (!res || res->status != 200) {
        std::cerr << "Failed to send AI request: " << (res ? res->status : 0) << std::endl;
    } else {
        Json::Value responseJson;
        Json::Reader reader;

        if (reader.parse(res->body, responseJson)) {
            int move = responseJson["move"].asInt();
            return move;
        } 
        else {
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

        int lastPlayer = gameBoard.currentPlayer;
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        {
            if (gamecase == 1)
            {
                if (gameBoard.placeStone(row, col))
                {
                    redraw();
                    moves.push(std::make_pair(row, col));
                    int state = sendGameState(gameBoard, col, row);
                    if (state == -1) {
                        fl_message("Failed to send game state");
                        this->window()->hide();
                    }
                    else if (state != 200)
                    {
                        // Game over
                        this->window()->hide();
                        showGameOverMessage(lastPlayer);
                    }
                }
            }
            else if (gamecase == 2 && gameBoard.currentPlayer == user_color)
            {
                if (gameBoard.placeStone(row, col))
                {
                    redraw();
                    moves.push(std::make_pair(row, col));
                    int state = sendGameState(gameBoard, col, row);
                    if (state == -1) {
                        fl_message("Failed to send game state");
                        this->window()->hide();
                    }
                    else if (state != 200)
                    {
                        // Game over
                        this->window()->hide();
                        showGameOverMessage(lastPlayer);
                    }
                    
                }
            }
            else if (gamecase == 2 && gameBoard.currentPlayer != user_color){
                int move = sendAIRequest(gameBoard, moves.top().second, moves.top().first);
                int ai_col = move % BOARD_SIZE, ai_row = move / BOARD_SIZE;
                if (gameBoard.placeStone(ai_row, ai_col))
                {
                    redraw();
                    moves.push(std::make_pair(ai_row, ai_col));
                    int state = sendGameState(gameBoard, ai_col, ai_row);
                    if (state == -1) {
                    fl_message("Failed to send game state");
                        this->window()->hide();
                    }
                    else if (state != 200)
                    {
                        // Game over
                        this->window()->hide();
                        showGameOverMessage(lastPlayer);
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

