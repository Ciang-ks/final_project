#include "play_with_human.h"
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
#include "jsoncpp/json.h"
using namespace std;

const int BOARD_LEN = 15;

class BBoard
{
public:
    const int EMPTY = 0;
    const int BLACK = 1;
    const int WHITE = 2;
    unsigned int numPieces = 0;
    bool isVictory = false;
    bool ruleViolation = false;
    int notOver = 200;
    int flatFlag = 100;
    vector<vector<int>> boardState;
    int nowRow = -1;
    int nowCol = -1;
    int currentPlayer;
    int lastPlayer;

    BBoard(FBoard *fboard);
    int getNumPieces();
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
}

BBoard::BBoard(FBoard *fboard) {
    this->boardState = fboard->boardState;
    this->currentPlayer = fboard->currentPlayer;
    this->nowRow = fboard->nowRow;
    this->nowCol = fboard->nowCol;
    this->lastPlayer = WHITE + BLACK - this->currentPlayer;
}

int BBoard::getNumPieces() {
    return this->numPieces;
}

int BBoard::isGameOver() {
    if (numPieces > 5)
    {
        int we = this->win_end();
        if (we == notOver) {
            if (find(this->boardState.begin(), this->boardState.end(), 0) == boardState.end()) {
                return flatFlag;
            }
            return notOver;
        }
        return we;
    }
    return notOver;
}

int BBoard::win_end() {
    int last_player = this->lastPlayer;
    if (last_player == BLACK) {
        if (long_connect()) return WHITE;
        if (five_connect()) return BLACK;
        if (three_three()) return WHITE;
        if (four_four()) return WHITE;
    }
    else {
        if (this->five_connect()) return WHITE;
    }
    return notOver;
}

bool BBoard::long_connect() {
    int h = this->nowRow;
    int w = this->nowCol;
    int last_player = this->lastPlayer;
    int ret;

    int bias = min(w, 5);
    for (int i = w - bias; i < w + 1; i++) {
        if (BOARD_LEN - 1 - i < 5) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k++) {
            if (this->boardState[h][w + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }
    bias = min(h, 5);
    for (int i = h - bias; i < h + 1; i++) {
        if (BOARD_LEN - 1 - i < 5) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k++) {
            if (this->boardState[h + k][w] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    bias = min(min(h, 5), min(w, 5));
    for (int i = h - bias, j = w - bias; i < h + 1; i ++, j++) {
        if ((BOARD_LEN - 1 - i < 5) || (BOARD_LEN - 1 - j < 5)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k ++) {
            if (this->boardState[h + k][w + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    bias = min(min(BOARD_LEN - 1 - h, 5), min(w, 5));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if ((BOARD_LEN - 1 - i < 5) || (j < 5)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k ++) {
            if (this->boardState[h - k][w + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }
    return false;
}

bool BBoard::tt_special_case(string& m_str, size_t pos, int t_case) {
    if (t_case == 1) { //oo111o
        if (pos + 6 < m_str.size()) {
            if (m_str[pos + 6] == '1') return true;
        }
    }
    else { //o111oo
        if (pos > 0) {
            if (m_str[pos - 1] == '1') return true;
        }
    }
    return false;
}

bool BBoard::ff_special_case(std::string& m_str, size_t pos, int f_case) {
    if (f_case == 1) { //oo111o
        if (pos > 0) {
            if (m_str[pos - 1] == '1') return true;
        }
        if (pos + 5 < m_str.size()) {
            if (m_str[pos + 5] == '1') return true;
        }
        return false;
    }
    else if (f_case == 2) {
        if (pos > 0) {
            if (pos + 6 < m_str.size()) {
                if (m_str[pos - 1] == '1' && ((
                m_str[pos + 5] == 'o' && m_str[pos + 6] == '1') || (
                m_str[pos + 5] == '0'))) return true;
                return false;
            }
            if (pos + 5 < m_str.size()) {
                if (m_str[pos - 1] == '1' && m_str[pos + 5] == '0') return true;
                return false;
            }
            if (m_str[pos - 1] == '1') return true;
            return false;
        }
        else return false;
    }
    else {
        if (pos + 5 < m_str.size()) {
            if (pos - 2 >= 0) {
                if ((m_str[pos - 2] == '1' && m_str[pos - 1] == 'o') || (m_str[pos - 1] == '0') && m_str[pos + 5] == '1')
                    return true;
                return false;
            }
            else if (pos - 1 >= 0) {
                if (m_str[pos + 5] == '1' && m_str[pos - 1] == '0') return true;
                return false;
            }
            if (m_str[pos + 5] == '1') return true;
            return false;
        }
        else return false;
    }
}

bool BBoard::thereCount(string& m_str) {
    string jt1 = "o1o11o";
    string jt2 = "o11o1o";
    string ct1 = "oo111o";
    string ct2 = "o111oo";
    int three = 0;
    size_t pos;
    pos = m_str.find(ct1);
    if (pos != m_str.npos) {
        if (this->tt_special_case(m_str, pos, 1) == false) three++;
    }
    else {
        pos = m_str.find(ct2);
        if (pos != m_str.npos) {
            if (this->tt_special_case(m_str, pos, 2) == false) three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos) three++;
    if (m_str.find(jt2) != m_str.npos) three++;
    if (three > 1) return true;
}

bool BBoard::three_three() {
    int h = this->nowRow;
    int w = this->nowCol;
    string m_str;
    int bias = min(w, 4);
    for (int i = w - bias; i < w + min(BOARD_LEN - 1 - w, 4) + 1; i++) {
        if (this->boardState[h][i] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[h][i] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->thereCount(m_str)) return true;
    
    m_str.clear();

    bias = min(h, 4);
    for (int i = h - bias; i < h + min(BOARD_LEN - 1 - h, 4) + 1; i ++) {
        if (this->boardState[i][w] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][w] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->thereCount(m_str)) return true;

    m_str.clear();
    bias = min(min(h, 4), min(w, 4));
    for (int i = h - bias, j = w - bias; i < h + min(BOARD_LEN - 1 - h, 4) + 1; i++, j++) {
        if (this->boardState[i][j] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][j] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->thereCount(m_str)) return true;

    m_str.clear();

    bias = min(min(BOARD_LEN - 1 - w, 4), min(h, 4));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if (this->boardState[i][j] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][j] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->thereCount(m_str)) return true;

    return false;
}

bool BBoard::fourCount(string& m_str) {
    string jf1 = "111o1";
    string jf2 = "1o111";
    string jf3 = "11o11";
    string cf1 = "o1111";
    string cf2 = "1111o";
    int four = 0;
    size_t pos;
    
    pos = m_str.find(jf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false) four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false) four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false) four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 2) == false) four++;
    }
    else {
        pos = m_str.find(cf2);
        if (pos != m_str.npos) {
            if (this->ff_special_case(m_str, pos, 3) == false) four++;
        }
    }
    if (four > 1) return true;
}

bool BBoard::four_four() {
    int h = this->nowRow;
    int w = this->nowCol;
    string m_str;
    int bias;

    bias = min(w, 5);
    for (int i = w - bias; i < w + min(width - 1 - w, 5) + 1; i++) {
        if (this->boardState[h][i] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[h][i] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->fourCount(m_str)) return true;

    m_str.clear();
    bias = min(h, 5);
    for (int i = h - bias; i < h + min(width - 1 - h, 5) + 1; i++) {
        if (this->boardState[i][w] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][w] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->fourCount(m_str)) return true;

    m_str.clear();
    bias = min(min(h, 5), min(w, 5));
    for (int i = h - bias, j = w - bias; i < h + min(width - 1 - h, 5) + 1; i++, j++) {
        if (this->boardState[i][j] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][j] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->fourCount(m_str)) return true;

    m_str.clear();
    bias = min(min(width - 1 - w, 5), min(h, 5));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if (this->boardState[i][j] == 0) {
            m_str.append(1, 'o');
        }
        else if (this->boardState[i][j] == 1) {
            m_str.append(1, '0');
        }
        else {
            m_str.append(1, '1');
        }
    }
    if (this->fourCount(m_str)) return true;
    return false;
}

bool BBoard::five_connect() {
    int h = this->nowRow;
    int w = this->nowCol;
    int last_player = this->lastPlayer;
    int ret;
    if (h == -1) return false;
    int bias = min(w, 4);
    for (int i = w - bias; i < w + 1; i++) {
        if (BOARD_LEN - 1 - w < 4) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[w + k][h] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    bias = min(h, 4);
    for (int i = h - bias; i < h + 1; i++) {
        if (BOARD_LEN - 1 - h < 4) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[w][h + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }

    bias = min(min(h, 4), min(w, 4));
    for (int i = h - bias, j = w - bias; i < h + 1; i++, j++) {
        if ((BOARD_LEN - 1 - i < 4) || (BOARD_LEN - 1 - j < 4)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[w + k][h + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    bias = min(min(BOARD_LEN - 1 - h, 4), min(w, 4));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if ((BOARD_LEN - 1 - i < 4) || (j < 4)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[w + k][h - k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    return false;
}
