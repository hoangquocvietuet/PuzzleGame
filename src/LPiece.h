#ifndef LPIECE_H
#define LPIECE_H
#include "LTexture.h"
#include "ALLConst.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <assert.h>

/*
 * blockBase: back ground block when play game
 * blockWood: wood block
 * blockInvisible: for convenient
 * smallBlockWood: wood block but smaller
 * blockTotal: number of block types
 */
enum BlockType
{
    blockBase,
    blockWood,
    blockInvisible,
    smallBlockWood,
    blockTotal,
};

/*
    LTexture contains image of blocks
*/
extern LTexture loadedBlock[blockTotal];

/*
    load block image
*/
bool loadBlockMedia(SDL_Renderer *gRenderer);

/*
    check ìf rentangle [x, y, w, h] contains point (_x, _y)
*/
bool inside(int x, int y, int w, int h, int _x, int _y);

class LPiece
{
private:
    /*
        a pattern contains many vector of blocktype -> each shows a puzzle piece
    */
    std::vector<std::vector<BlockType>> pattern;

    /*
        coordinate of the piece
    */
    int x = 0, y = 0;

public:
    /*
        draw the piece
        @param preview: transparent the piece to make preview effect
        @param small: load small block
        @param _x: if _x = -1, then x = coordinate of piece
        @param _y
    */
    void draw(SDL_Renderer *gRenderer, bool preview, bool small = false, int _x = -1, int _y = -1);

    /*
        set the coordinate of the piece
    */
    void setPosition(int _x, int _y);

    /*
        set the pattern of the piece
    */
    void setPattern(std::vector<std::vector<BlockType>> _pattern);

    /*
     *add new row to pattern
     *assume that str contains `0` and `1`
     */
    void addToPattern(std::string str);

    /*
        clear pattern
    */
    void resetPattern();

    /*
     * if mouse focus on piece
     * assume that it was small piece (candidate piece)
     */
    bool mouseFocusOn(int _x, int _y);

    /*
        get top left x
    */
    int getX();

    /*
        get top left y
    */
    int getY();

    /*
        get number of row of pattern
    */
    int getNumRowPattern();

    /*
        @param idx: get number of column of pattern idx
    */
    int getLengthRowPattern(int idx);

    /*
        get exactly block type of pattern in position (row, col)
    */
    BlockType getCharacterPattern(int row, int col);
};

#endif