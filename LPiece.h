#ifndef LPIECE_H
#define LPIECE_H
#include "LTexture.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <assert.h>

/*
    50% alpha
*/
const int alpha = 127; 
const int SQUARE_LENGTH = 44;
const int SQUARE_DISTANCE = 47;

enum BlockType {
    blockBase,
    blockWood,
    blockInvisible,
    blockTotal,
};

extern LTexture loadedBlock[blockTotal];
bool loadBlockMedia(SDL_Renderer* gRenderer);
bool inside(int x, int y, int w, int h, int _x, int _y);

class LPiece {
    private:
        std::vector<std::vector<BlockType>> pattern;
        int x = 0, y = 0;

    public:
        void draw(SDL_Renderer* gRenderer, bool preview, int _x = -1, int _y = -1);
        void setPosition(int _x, int _y);
        void setPattern(std::vector<std::vector<BlockType>> _pattern);
        void addToPattern(std::string str);
        void resetPattern();
        bool isClicked(int _x, int _y);
        int getX();
        int getY();
        int getNumRowPattern();
        int getLengthRowPattern(int idx);
        BlockType getCharacterPattern(int row, int col);
};

#endif