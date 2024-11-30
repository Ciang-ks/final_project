// 服务器端的主要功能是接收客户端的请求并判断请求的类型，然后调用相应的函数处理请求
// 服务器通过http协议与客户端通信，因此服务器需要解析http请求，传进的http是一个棋盘的状态，希望能用json格式传输
// 服务器需要判断这个棋盘的状态，返回的http包括以下几个部分：是否有人胜利，是否有人违反规则，是否有人请求悔棋，是否有人请求求和，是否有人请求认输
// 返回值通过一个api包裹，api名称为/api/board，包含以下几个字段：victory，ruleViolation，undoRequest，drawRequest，surrenderRequest
// 其中规则如rule.cpp所示，需要调用rule.cpp中的函数判断。
// 五子棋的胜利规则是：横向、纵向、左斜、右斜四个方向有五个连续的棋子即为胜利。
/* #include <iostream>
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include "httplib.h"

void handleRequest(const std::string& request) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(request, root)) {
        std::cerr << "Failed to parse request" << std::endl;
        return;
    }

    // Extract the board state from the request
    std::vector<std::vector<int>> board;
    for (const auto& row : root["board"]) {
        std::vector<int> boardRow;
        for (const auto& cell : row) {
            boardRow.push_back(cell.asInt());
        }
        board.push_back(boardRow);
    }

    Json::Value response;
    response["victory"] = checkVictory(board);
    response["ruleViolation"] = checkRuleViolation(board);
    response["undoRequest"] = root["undoRequest"].asBool();
    response["drawRequest"] = root["drawRequest"].asBool();
    response["surrenderRequest"] = root["surrenderRequest"].asBool();

    Json::StreamWriterBuilder writer;
    std::string output = Json::writeString(writer, response);
    std::cout << output << std::endl;
}

int main() {
    std::string request;
    while (std::getline(std::cin, request)) {
        handleRequest(request);
    }
    return 0;
}
*/