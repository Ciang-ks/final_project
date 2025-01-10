from __future__ import print_function
import numpy as np


class Board(object):

    def __init__(self, **kwargs):
        self.width = int(kwargs.get('width', 15))
        self.height = int(kwargs.get('height', 15))
        # 用字典存储棋盘状态
        # 键代表位置，值代表棋子类型
        self.states = {}
        # 胜利所需棋子数
        self.n_in_row = int(kwargs.get('n_in_row', 5))
        self.players = [1, 2]

    def init_board(self, start_player=0):
        if self.width < self.n_in_row or self.height < self.n_in_row:
            raise Exception('board width and height can not be '
                            'less than {}'.format(self.n_in_row))
        self.current_player = self.players[start_player]
        # 先手执黑
        self.first_player = self.current_player
        # 维护禁止落子的位置
        self.availables = list(range(self.width * self.height))
        self.states = {}
        self.last_move = -1

    def move_to_location(self, move):
        # 转换move为坐标
        h = move // self.width
        w = move % self.width
        return [h, w]

    def location_to_move(self, location):
        # 转换坐标为move
        if len(location) != 2:
            return -1
        h = location[0]
        w = location[1]
        move = h * self.width + w
        if move not in range(self.width * self.height):
            return -1
        return move

    def current_state(self):
        # 初始化一个4通道的棋盘状态矩阵
        square_state = np.zeros((4, self.width, self.height))
        if self.states:
            # 获取所有落子的位置和对应的玩家
            moves, players = np.array(list(zip(*self.states.items())))
            # 当前玩家的落子位置
            move_curr = moves[players == self.current_player]
            # 对手的落子位置
            move_oppo = moves[players != self.current_player]
            # 标记当前玩家的落子位置
            square_state[0][move_curr // self.width,
                            move_curr % self.height] = 1.0
            # 标记对手的落子位置
            square_state[1][move_oppo // self.width,
                            move_oppo % self.height] = 1.0
            # 标记最后一个落子的位置
            square_state[2][self.last_move // self.width,
                            self.last_move % self.height] = 1.0
        # 如果当前是先手玩家，则标记第四个通道
        if len(self.states) % 2 == 0:
            square_state[3][:, :] = 1.0
        # 返回棋盘状态矩阵，翻转第二个维度
        return square_state[:, ::-1, :]

    def do_move(self, move):
        # 落子
        self.states[move] = self.current_player
        self.availables.remove(move)
        self.current_player = (
            self.players[0] if self.current_player == self.players[1]
            else self.players[1]
        )
        self.last_move = move

    def check(self, player, loc, delta, n):
            # 获取某一个方向上的连续棋子数
            nline = 1
            # 正向搜索
            for i in range(1, n + 1):
                if loc + delta * i in self.states and self.states[loc + delta * i] == player:
                    nline += 1
                else:
                    break

            # 反向搜索   
            for i in range(1, n + 1):
                if loc - delta * i in self.states and self.states[loc - delta * i] == player:
                    nline += 1
                else:
                    break
            
            return nline
    
    def get_in_row(self, move, player, n):
        # 获取当前棋子所能连成的最大棋子数
        directions = [(0, 1), (1, 0), (1, 1), (1, -1)]
        max_nline = 0
        for d in directions:
            delta = d[0] * self.width + d[1]
            nline = self.check(player, move, delta, n)
            if nline > max_nline:
                max_nline = nline
        return max_nline
    
    def is_four(self, move, player, delta):
        # 判断是否形成四子
        loc = move + delta
        while loc in self.states and self.states[loc] == player:
            loc += delta
        if loc not in self.states and loc in range(self.width * self.height):
            if self.get_in_row(loc, player, 5) == 5:
                return True

             
        loc = move - delta
        while loc in self.states and self.states[loc] == player:
            loc -= delta
        if loc not in self.states and loc in range(self.width * self.height):
            if self.get_in_row(loc, player, 5) == 5:
                return True
        return False
    
    def is_open_four(self, move, player, delta):
        # 判断是否形成活四
        nline = 1
        loc = move + delta
        while loc in self.states and self.states[loc] == player:
            loc += delta
            nline += 1

        if loc not in self.states and loc in range(self.width * self.height):
            if self.get_in_row(loc, player, 5) != 5:
                return 0
        else: 
            return 0

        loc = move - delta
        while loc in self.states and self.states[loc] == player:
            loc -= delta
            nline += 1
        if loc not in self.states and loc in range(self.width * self.height):
            if self.get_in_row(loc, player, 5) == 5:
                if nline == 4:
                    return 1
                elif nline < 4:
                    return 2
        return 0

    def double_four(self, move, player):
        # 判断是否形成两个以上活四
        directions = [(0, 1), (1, 0), (1, 1), (1, -1)]
        nfour = 0
        for d in directions:
            delta = d[0] * self.width + d[1]
            if self.is_open_four(move, player, delta) == 2:
                nfour += 2
            elif self.is_four(move, player, delta):
                nfour += 1
        if nfour >= 2:
            return True
        return False

    def is_open_three(self, move, player, delta):
        # 判断活三
        loc = move
        while loc in self.states and self.states[loc] == player:
            loc += delta
        if loc not in self.states and loc in range(self.width * self.height):
            if self.is_open_four(loc, player, delta) == 1:
                return True

        loc = move
        while loc in self.states and self.states[loc] == player:
            loc -= delta
        if loc not in self.states and loc in range(self.width * self.height):
            if self.is_open_four(loc, player, delta) == 1:
                return True
        return False
    
    def double_three(self, move, player):
        # 是否形成两个以上活三
        directions = [(0, 1), (1, 0), (1, 1), (1, -1)]
        nthree = 0
        for d in directions:
            delta = d[0] * self.width + d[1]
            if self.is_open_three(move, player, delta):
                nthree += 1
        if nthree >= 2:
            return True
        return False

    
    def has_a_winner(self):
        # 判断胜负
        width = self.width
        height = self.height
        states = self.states
        n = self.n_in_row
        last_move = self.last_move
        # 落子过少，必没结束
        movedpace = width * height - len(self.availables)
        if movedpace < self.n_in_row *2-1:
            return False, -1
        # 判断白是否赢
        if states[last_move] != self.first_player:
            if self.get_in_row(last_move, states[last_move], n) >= n:
                return True, states[last_move]
        # 判断黑是否赢
        else:
            if self.get_in_row(last_move, states[last_move], n) > n:
                return True, 3 - states[last_move]
            if self.get_in_row(last_move, states[last_move], n) == n:
                return True, states[last_move]
            if self.double_four(last_move, states[last_move]) or self.double_three(last_move, states[last_move]):
                return True, 3 - states[last_move]
        return False, -1

    def game_end(self):
        # 如果产生赢家，则游戏结束，返回胜者
        win, winner = self.has_a_winner()
        if win:
            return True, winner
        # 若无剩余可落子位置，返回平局
        elif not len(self.availables):
            return True, -1
        return False, -1

    def get_current_player(self):
        return self.current_player
