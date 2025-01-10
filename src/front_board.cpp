#include "front_board.h"
using namespace std;
const int BOARD_SIZE = 15;
Mix_Chunk *placeSound = nullptr;

FBoard::FBoard(int size) : boardState(size, vector<int>(size, 0)), currentPlayer(1) {
    // 初始化 SDL 音频子系统
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
    }
    // 初始化 SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
    }

    // 加载音频文件
    placeSound = Mix_LoadWAV("assets/audio/place.mp3");
    if (!placeSound) {
        cerr << "Failed to load place.mp3: " << Mix_GetError() << endl;
    }

    // 设置音量
    Mix_VolumeChunk(placeSound, MIX_MAX_VOLUME);
}

FBoard::~FBoard() {
    // 清理 SDL2 和 SDL2_mixer 资源
    Mix_FreeChunk(placeSound);
    Mix_CloseAudio();
    SDL_Quit();
}

bool FBoard::placeStone(int row, int col) {
    // 检查位置是否合法且为空
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && boardState[row][col] == 0) {
        // 放置棋子
        boardState[row][col] = currentPlayer;
        // 切换玩家
        currentPlayer = 3 - currentPlayer;
        // 播放音效
        if (placeSound) {
            Mix_PlayChannel(-1, placeSound, 0);
        }

        return true;
    }
    return false;
}


