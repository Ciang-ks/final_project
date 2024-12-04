#ifndef MY_RULE_H
#define MY_RULE_H

#include "front_board.h"
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
#include <jsoncpp/json/json.h>
using namespace std;

const int BOARD_LEN = 15;

class BBoard
{
public:
    const int EMPTY = 0;
    const int BLACK = 1;
    const int WHITE = 2;
    bool isVictory = false;
    bool ruleViolation = false;
    int notOver = 200;
    int flatFlag = 100;
    vector<vector<int>> boardState;
    int nowRow = -1;
    int nowCol = -1;
    int currentPlayer;
    int lastPlayer;

    BBoard(const Json::Value& json);
    int isGameOver();
    int win_end();
    bool long_connect();
    bool tt_special_case(string& m_str, size_t pos, int t_case);
    bool ff_special_case(string& m_str, size_t pos, int f_case);
    bool thereCount(string& m_str);
    bool fourCount(string& m_str);
    bool three_three();
    bool four_four();
    bool five_connect();
};

#endif // MY_RULE_H