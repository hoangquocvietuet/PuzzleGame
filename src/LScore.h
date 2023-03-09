#ifndef LSCORE_H
#define LSCORE_H
#include <SDL.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <fstream>
#include "ALLConst.h"
#include "LTexture.h"

class LScore {
    private:
        /*
            current soccer recorded 
        */
        long long totalScore = 0;

        /*
            text of score
        */
        LTexture scoreTexture;

    public:
        /*
            set current score = newScore
            @param newScore
        */
        void setScore(long long newScore);

        /*
            increase current score
            @param: increased amount
        */
        void addScore(int addedScore);

        /*
            increase current score
            @param cntRow: number of removed row
            @param cntCol: number removed column
        */
        void addScore(int cntRow, int cntCol);

        /*
            load score to texture then center and print it to [x, y, w, h] rectangle 
            @param x: top left x
            @param y: top left y
            @param w: width
            @param h: height
            @param score: if print another score
        */
        void printScore(SDL_Renderer* gRenderer, TTF_Font* gFont, int x, int y, int w, int h, long long score = -1);

        /*
            readScore from file
        */
        long long readScore(std::string path);

        /*
            save score to file, used to save best score
        */
        void saveScore(std::string path);
};
#endif
