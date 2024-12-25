
from __future__ import print_function
import os
from flask import Flask, request, jsonify
from game import Board
from mcts import MCTSPlayer
from policy_value_net import PolicyValueNet

app = Flask(__name__)

current_dir = os.path.dirname(os.path.abspath(__file__))
model_file = os.path.join(current_dir, '../model/best_policy.model')
board_width = 15
board_height = 15
best_policy = PolicyValueNet(board_width, board_height, model_file=model_file)
mcts_player = MCTSPlayer(best_policy.policy_value_fn, c_puct=5, n_playout=1000)

@app.route('/api/ai', methods=['POST'])
def get_next_move():
    data = request.get_json()
    current_player = data['currentPlayer']
    col = data['col']
    row = data['row']
    board_state = data['board']

    # 创建 Board 对象并设置状态
    board = Board(width=board_width, height=board_height)
    board.init_board()
    board.current_player = current_player
    board.states = {i * board_width + j: board_state[i][j] for i in range(board_width) 
                    for j in range(board_height) if board_state[i][j] != 0}
    board.availables = [i * board_width + j for i in range(board_width) 
                        for j in range(board_height) if board_state[i][j] == 0]
    board.last_move = board.location_to_move([row, col])

    # 获取 AI 的下一步落子位置
    move = mcts_player.get_action(board)
    move = int(move)
    # 返回结果
    return jsonify({'move': move})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=7070)