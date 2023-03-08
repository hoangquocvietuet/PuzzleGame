#include "LPiece.h"

LTexture loadedBlock[blockTotal];

bool loadBlockMedia(SDL_Renderer* gRenderer) {
    bool success = true;
    if (!loadedBlock[blockWood].loadFromFile(gRenderer, "assets/block-wood.png")) {
        success = false;
    }
    if (!loadedBlock[blockInvisible].loadFromFile(gRenderer, "assets/block-invisible.png")) {
        success = false;
    }
    if (!loadedBlock[blockBase].loadFromFile(gRenderer, "assets/block-base.png")) {
        success = false;
    }
    return success;
}

void LPiece::addToPattern(std::string str) {
    std::vector<BlockType> vectorBlockType;
    for (int i = 0; i < (int)str.size(); ++ i) {
        if (str[i] == '1') {
            vectorBlockType.push_back(blockWood);
        } else {
            vectorBlockType.push_back(blockInvisible);
        } 
    }
    pattern.push_back(vectorBlockType);
}

void LPiece::resetPattern() {
    pattern.clear();
}

void LPiece::draw(SDL_Renderer* gRenderer, bool preview, int _x, int _y) {
    if (_x == -1) {
        _x = x;
        _y = y;
    }
    int tmpy = _y;
    for (auto str : pattern) {
        int tmpx = _x;
        for (BlockType ch : str) {
            if (preview) loadedBlock[ch].setAlpha(alpha);
            loadedBlock[ch].render(gRenderer, tmpx, tmpy);
            if (preview) loadedBlock[ch].setAlpha(255);
            tmpx += SQUARE_DISTANCE;
        }
        tmpy += SQUARE_DISTANCE;
    }
}

void LPiece::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
}

void LPiece::setPattern(std::vector<std::vector<BlockType>> _pattern) {
    pattern = _pattern;
}


bool inside(int x, int y, int w, int h, int _x, int _y) {
    if (_x >= x && _y >= y && _x < x + w && _y < y + h) {
        return true;
    }
    return false;
}

bool LPiece::isClicked(int _x, int _y) {
    int tmpy = y;
    for (auto str : pattern) {
        int tmpx = x;
        for (auto ch : str) {
            if (ch == blockWood && inside(tmpx, tmpy, SQUARE_LENGTH, SQUARE_LENGTH, _x, _y)) {
                return true;
            }
            tmpx += SQUARE_DISTANCE;
        }
        tmpy += SQUARE_DISTANCE;
    }
    return false;
}

int LPiece::getX() {
    return x;
}

int LPiece::getY() {
    return y;
}

int LPiece::getNumRowPattern() {
    return pattern.size();
}

int LPiece::getLengthRowPattern(int idx) {
    assert(idx >= 0 && idx < (int)pattern.size());
    return pattern[idx].size();
}

BlockType LPiece::getCharacterPattern(int row, int col) {
    assert(row >= 0 && row < getNumRowPattern());
    assert(col >= 0 && col < getLengthRowPattern(row));
    return pattern[row][col];
}