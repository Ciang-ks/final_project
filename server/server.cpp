#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "my_rule.h"

// 处理棋盘请求的函数
void handleBoardRequest(const httplib::Request& req, httplib::Response& res) {
    Json::Value jsonData;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
    std::istringstream s(req.body);
    // 解析请求中的json数据
    if (!Json::parseFromStream(readerBuilder, s, &jsonData, &errs)) {
        res.status = 400;
        res.set_content("Invalid JSON: " + errs, "text/plain");
        return;
    }

    // 创建BBoard（backboard）对象
    BBoard *b = new BBoard(jsonData);
    
    Json::Value responseJson;
    int status = b->isGameOver();

    // 设置响应内容
    res.set_content(std::to_string(status), "application/json");
}

int main() {
    httplib::Server svr;

    // 设置处理POST请求的路由
    svr.Post("/api/board", handleBoardRequest);

    std::cout << "Server started at http://localhost:8080" << std::endl;
    // 启动服务器，监听localhost:8080
    svr.listen("localhost", 8080);

    return 0;
}