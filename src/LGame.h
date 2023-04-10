#ifndef LGAME_H
#define LGAME_H
#include "LPiece.h"
#include "LRandom.h"
#include "LScore.h"
#include "ALLConst.h"
#include "LButton.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>
#include <string>

struct Block {
    /*
        top left coordinate
    */
    int x, y;

    /*
        type of the block in the grid
    */
    BlockType type;
};

struct Candidate {
    /*
        top left coordinate
    */
    int x = 0, y = 0;

    /*
        the piece of candidate
    */
    LPiece piece;
};

class LGame {
    private:
        /*
            game window
        */ 
        SDL_Window* mWindow = nullptr;

        /*
            game renderer
        */
        SDL_Renderer* mRenderer = nullptr;

        /*
            play video renderer
        */
        SDL_Renderer* playVideoRenderer = nullptr;

        /*
            play video window
        */
        SDL_Window* playVideoWindow = nullptr;

        /*
            game font
        */
        TTF_Font* mFont;

        /*
            background:
        */
        LTexture backGround, startGameBackGround, popUpEndGameBackGround;

        LTexture rickRoll[190];

        /*
            button
        */
        LButton startGameButton, restartGameButton, exitGameButton, doubleScoreButton;

        /*
            LScore to save game process
        */
        LScore sc;
        LRandom rnd;

        /*
            info about current block
        */
        Block currentBlock[SIZE][SIZE];

        /*
            game event
        */
        SDL_Event mEvent;

        /*
            game pieces, we will take three of theese to make candidates
        */
        std::vector<LPiece> vectorPiece;

        /*
            current row that mouse focus on
        */
        int row = -1;
        
        /*
            current column that mouse focus on
        */
        int col = -1;
        
        /*
            current picked index of candidates
        */
        int IdxCandidatePiece = -1;

        /*
            candidates, wait list..
        */
        Candidate candidatePiece[NUM_CANDIDATE];

        /*
            music !!
        */
        Mix_Music* startGameMusic = nullptr;
        Mix_Music* playGameMusic = nullptr;
        Mix_Music* rickRollMusic = nullptr;

    public:
        /*
            * constructor
            * init position of the blocks in the grid, gameplay, position of candidates piece
            * loading and init game media
        */
        LGame(SDL_Renderer* gRenderer, SDL_Window* gWindow, TTF_Font* gFont);

        /*
            *destructor
        */
        ~LGame();

        /*
            load game media: button, background, vector pieces
        */
        bool loadGameMedia();     

        /*
            * call when you want to play game
            * it will open start page
        */
        void startPage();  

        /*
            initialize vectorPieces from a custom file
        */
        bool initPieceFromFile(std::string path);

        bool handleGameEvent();

        /*
            @return true if we can set piece in block (u, v)
        */
        bool checkCurrentPieceInside(int u, int v, LPiece piece);

        /*
            load preview of candidate pieces
        */
        void loadPreview();

        /*
            we will lose if there is no way to put candidates piece.
        */
        bool checkLosingGame();

        /*
            update score and remove filled row or column
        */
        void updateScoreAndGameState();

        /*
            reset game state
        */
        void resetGame();

        /*
            contains game content
            when you lose, it will open end pgae
        */
        void gameLoop();

        /*
            * open end page
            * its will show best score
        */
        void endPage(bool double_score);

        /*
            if you clicked on start game, its running into gameloop
        */
        int handleStartPageEvent();

        /*
            @return 1 if you clicked on restart, or 2 if exit, or 3 if double score
        */
        int handleEndPageEvent(bool double_score);

        void playRickRoll();
};

#endif