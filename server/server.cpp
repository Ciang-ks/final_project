#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "my_rule.h"

// 服务器端的主要功能是接收客户端的请求并判断请求的类型，然后调用相应的函数处理请求
// 服务器通过http协议与客户端通信，因此服务器需要解析http请求，传进的http是一个棋盘的状态，希望能用json格式传输
// 服务器需要判断这个棋盘的状态，返回的http包括以下几个部分：是否有人胜利。
// 返回值通过一个api包裹，api名称为/api/board，返回值为json格式
// 其中规则如rule.cpp所示，需要调用rule.cpp中的函数判断。

// 首先设立一个服务器类和api，用于处理http请求，发送的格式如draw_board.cpp所示


void handleBoardRequest(const httplib::Request& req, httplib::Response& res) {
    Json::Value jsonData;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
    std::istringstream s(req.body);
    if (!Json::parseFromStream(readerBuilder, s, &jsonData, &errs)) {
        res.status = 400;
        res.set_content("Invalid JSON: " + errs, "text/plain");
        return;
    }

    BBoard *b = new BBoard(jsonData);
    
    Json::Value responseJson;
    int status = b->isGameOver();

    res.set_content(std::to_string(status), "application/json");
}

int main() {
    httplib::Server svr;

    svr.Post("/api/board", handleBoardRequest);

    std::cout << "Server started at http://localhost:8080" << std::endl;
    svr.listen("localhost", 8080);

    return 0;
}