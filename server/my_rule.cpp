#include "my_rule.h"
// 主要参考了修改前的裁判代码

// BBoard类的构造函数，从json对象初始化棋盘状态
BBoard::BBoard(const Json::Value& json) {
    // 初始化棋盘状态为一个二维向量，初始值为0
    this->boardState = vector<vector<int>>(BOARD_LEN, vector<int>(BOARD_LEN, 0));
    // 填充棋盘状态
    for (int i = 0; i < BOARD_LEN; i++) {
        for (int j = 0; j < BOARD_LEN; j++) {
            this->boardState[i][j] = json["board"][i][j].asInt();
        }
    }
    // 初始化当前玩家、行和列
    this->currentPlayer = json["currentPlayer"].asInt();
    this->nowRow = json["row"].asInt();
    this->nowCol = json["col"].asInt();
    // 确定上一个玩家
    this->lastPlayer = WHITE + BLACK - this->currentPlayer;
    string lastNotice = this->lastPlayer == BLACK ? "BLACK" : "WHITE";
    // 输出上一步的移动
    cout << "Place " <<  lastNotice << " Stone in" << endl;
    cout << "Row: " << this->nowRow + 1 << ' ';
    cout << "Col: " << this->nowCol + 1 << endl << endl;
}

// 检查游戏是否结束
int BBoard::isGameOver() {
    int we = this->win_end();
    if (we == notOver) {
        int flag = 1;
        // 检查是否还有空位
        for (int i = 0; i < BOARD_LEN; i++) {
            for (int j = 0; j < BOARD_LEN; j++) {
                if (this->boardState[i][j] == EMPTY) {
                    flag = 0;
                    break;
                }
            }
            if (flag == 0) break;
        }
        // 如果没有空位，则平局
        if (flag) {
            return flatFlag;
        }
        return notOver;
    }
    return we;
}

// 确定赢家或游戏是否仍在进行
int BBoard::win_end() {
    int last_player = this->lastPlayer;
    if (last_player == BLACK) {
        if (this->long_connect()) return WHITE;
        if (this->five_connect()) return BLACK;
        if (this->three_three()) return WHITE;
        if (this->four_four()) return WHITE;
    }
    else {
        if (this->five_connect()) return WHITE;
    }
    return notOver;
}

// 检查是否有长连
bool BBoard::long_connect() {
    int h = this->nowRow;
    int w = this->nowCol;
    int last_player = this->lastPlayer;
    int ret;

    // 检查水平连线
    int bias = min(w, 5);
    for (int i = w - bias; i < w + 1; i++) {
        if (BOARD_LEN - 1 - i < 5) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k++) {
            if (this->boardState[h][i + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 检查垂直连线
    bias = min(h, 5);
    for (int i = h - bias; i < h + 1; i++) {
        if (BOARD_LEN - 1 - i < 5) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k++) {
            if (this->boardState[i + k][w] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 检查对角线（左上到右下）
    bias = min(min(h, 5), min(w, 5));
    for (int i = h - bias, j = w - bias; i < h + 1; i++, j++) {
        if ((BOARD_LEN - 1 - i < 5) || (BOARD_LEN - 1 - j < 5)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k ++) {
            if (this->boardState[i + k][j + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 检查对角线（左下到右上）
    bias = min(min(BOARD_LEN - 1 - h, 5), min(w, 5));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if ((BOARD_LEN - 1 - j < 5) || (i < 5)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 6; k++) {
            if (this->boardState[i - k][j + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }
    return false;
}

bool BBoard::tt_special_case(string& m_str, size_t pos, int t_case) {
    // 三三禁手特殊情况
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
    // 四四禁手特殊情况
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
            if (m_str[pos - 1] == '1') {
                if (pos + 6 < m_str.size()) {
                    if ((m_str[pos + 5] == 'o' && m_str[pos + 6] == '1') || (m_str[pos + 5] == '0'))
                        return true;
                }
                else if (pos + 5 == m_str.size() - 1) {
                    if (m_str[pos + 5] == '0')
                        return true;
                }
                else if (pos + 4 == m_str.size() - 1) {
                    return true;
                }
                else {
                    return false;
                }
            }
            return false;
        }
        else return false;
    }
    else {
        if (pos + 5 < m_str.size()) {
            if (m_str[pos + 5] == '1') {
                if (pos - 2 >= 0) {
                    if ((m_str[pos - 2] == '1' && m_str[pos - 1] == 'o') || (m_str[pos - 1] == '0'))
                        return true;
                }
                else if (pos - 1 == 0) {
                    if (m_str[pos - 1] == '0')
                    return true;
                }
                else {
                    return true;
                }
            }
            return false;
        }
        else return false;
    }

}

bool BBoard::thereCount(string& m_str) {
    // 活三计数
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
    else return false;
}

bool BBoard::three_three() {
    // 活三禁手
    int h = this->nowRow;
    int w = this->nowCol;
    string m_str;
    int bias = min(w, 4);
    int altbias;
    // 将周围落子处理为字符串
    // 水平
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

    // 竖直
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

    // 对角线1
    bias = min(min(h, 4), min(w, 4));
    altbias = min(min(BOARD_LEN - 1 - h, 4), min(BOARD_LEN - 1 - w, 4));
    for (int i = h - bias, j = w - bias; i < h + altbias + 1; i++, j++) {
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

    // 对角线2
    bias = min(min(BOARD_LEN - 1 - w, 4), min(h, 4));
    altbias = min(min(BOARD_LEN - 1 - h, 4), min(w, 4));
    for (int i = h - bias, j = w + bias; i < h + altbias + 1; i++, j--) {
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
    // 活四计数
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
    else return false;
}

bool BBoard::four_four() {
    // 活四禁手
    int h = this->nowRow;
    int w = this->nowCol;
    string m_str;
    int bias;
    int altbias;

    // 将周围落子处理为字符串
    // 水平
    bias = min(w, 5);
    for (int i = w - bias; i < w + min(BOARD_LEN - 1 - w, 5) + 1; i++) {
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

    // 竖直
    bias = min(h, 5);
    for (int i = h - bias; i < h + min(BOARD_LEN - 1 - h, 5) + 1; i++) {
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

    // 对角线1
    bias = min(min(h, 5), min(w, 5));
    altbias = min(min(BOARD_LEN - 1 - h, 5), min(BOARD_LEN - 1 - w, 5));
    for (int i = h - bias, j = w - bias;i < h + altbias + 1; i++, j++) {
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

    // 对角线2
    bias = min(min(BOARD_LEN - 1 - w, 5), min(h, 5));
    altbias = min(min(BOARD_LEN - 1 - h, 5), min(w, 5));
    for (int i = h - bias, j = w + bias; i < altbias + 1; i++, j--) {
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
    // 判断是否五连
    int h = this->nowRow;
    int w = this->nowCol;
    int last_player = this->lastPlayer;
    int ret;

    // 水平
    int bias = min(w, 4);
    for (int i = w - bias; i < w + 1; i++) {
        if (BOARD_LEN - 1 - i < 4) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[h][i + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 竖直
    bias = min(h, 4);
    for (int i = h - bias; i < h + 1; i++) {
        if (BOARD_LEN - 1 - i < 4) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[i + k][w] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 对角线1
    bias = min(min(h, 4), min(w, 4));
    for (int i = h - bias, j = w - bias; i < h + 1; i++, j++) {
        if ((BOARD_LEN - 1 - i < 4) || (BOARD_LEN - 1 - j < 4)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {
            if (this->boardState[i + k][j + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }

    // 对角线2
    bias = min(min(BOARD_LEN - 1 - h, 4), min(w, 4));
    for (int i = h + bias, j = w - bias; i > h - 1; i--, j++) {
        if ((BOARD_LEN - 1 - j < 4) || (i < 4)) {
            break;
        }
        ret = 0;
        for (int k = 0; k < 5; k++) {

            if (this->boardState[i - k][j + k] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) return true;
    }
    return false;
}
