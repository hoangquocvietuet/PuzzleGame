#ifndef LGAME_H
#define LGAME_H
#include "LPiece.h"
#include "LRandom.h"
#include <SDL_ttf.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

const int NUM_CANDIDATE = 3;
const int SIZE = 10;

struct Block {
    SDL_Point topLeftPosition;
    BlockType type;
};

struct Candidate {
    /*
        default position
    */
    int x = 0, y = 0;
    LPiece piece;
};

class LGame {
    private:
        /*
            game window
        */ 
        SDL_Window* mWindow = NULL;

        /*
            game renderer
        */
        SDL_Renderer* mRenderer = NULL;

        /*
            game font
        */
        TTF_Font* mFont;

        /*
            background
        */
        LTexture backGround;

        /*
            score border and score texture
        */
        LTexture scoreBorder, printScore;


        LRandom rnd;

        /*
            info about current block
        */
        Block currentBlock[SIZE][SIZE];

        SDL_Event mEvent;
        std::vector<LPiece> vectorPiece;
        bool quit = false;
        int row = -1, col = -1, IdxCandidatePiece = -1;
        Candidate candidatePiece[NUM_CANDIDATE];
        long long totalScore = 0;

    public:
        LGame(SDL_Renderer* gRenderer, SDL_Window* gWindow, TTF_Font* gFont);
        ~LGame();
        bool loadGameMedia();     
        void playGame();  
        void initPieceFromFile(std::string path);
        void handleEvent();
        bool checkCurrentPieceInside(int u, int v);
        void loadPreview();
        bool checkLosingGame();
        bool continueGame();
        void updateScoreAndGameState();
        void resetGame();
};

#endif