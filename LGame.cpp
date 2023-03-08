#include "LGame.h"

LGame::LGame(SDL_Renderer* gRenderer, SDL_Window* gWindow, TTF_Font* gFont) {
    mRenderer = gRenderer;
    mWindow = gWindow;
    mFont = gFont;
    if (!loadGameMedia()) {
        printf("fail to load game media\n");
    }
    initPieceFromFile("assets/pieces.txt");

    /*
        to make beutiful
    */
    for (int i = 0, ss = 250; i < 10; ++ i, ss += SQUARE_DISTANCE) {
        for (int j = 0, st = 35; j < 10; ++ j, st += SQUARE_DISTANCE) {
            currentBlock[i][j].topLeftPosition = {st, ss};
            currentBlock[i][j].type = blockBase;
        }
    }
    for (int i = 0, ss = 20; i < NUM_CANDIDATE; ++ i, ss += 180) {
        candidatePiece[i].x = ss;
        candidatePiece[i].y = 760;
    }
}

LGame::~LGame() {
    mRenderer = NULL;
    mFont = NULL;
}
 
bool LGame::loadGameMedia() {
    bool success = true;
    if (!backGround.loadFromFile(mRenderer, "assets/background.png")) {
        success = false;
    }
    if (!scoreBorder.loadFromFile(mRenderer, "assets/border-score.png")) {
        success = false;
    }
    if (!loadBlockMedia(mRenderer)) {
        success = false;
    }
    return success;
}

void LGame::initPieceFromFile(std::string path) {
    vectorPiece.clear();
    std::ifstream input(path.c_str());
    if (!input) {
        printf("can not not open file contains pieces");
        return;
    }
    std::string str = "";
    LPiece tmpPiece;
    while (input >> str) {
        if (str == "23052004") {
            vectorPiece.push_back(tmpPiece);
            tmpPiece.resetPattern();
            continue;
        }
        tmpPiece.addToPattern(str);
    }
}

bool LGame::checkCurrentPieceInside(int u, int v) {
    assert(IdxCandidatePiece != -1);
    if (u + candidatePiece[IdxCandidatePiece].piece.getNumRowPattern() - 1 >= SIZE) return false;
    for (int i = 0; i < candidatePiece[IdxCandidatePiece].piece.getNumRowPattern(); ++ i) {
        if (v + candidatePiece[IdxCandidatePiece].piece.getLengthRowPattern(i) - 1 >= SIZE) return false;
        for (int j = 0; j < candidatePiece[IdxCandidatePiece].piece.getLengthRowPattern(i); ++ j) {
            if (candidatePiece[IdxCandidatePiece].piece.getCharacterPattern(i, j) == blockWood && currentBlock[i + u][j + v].type == blockWood) {
                return false;
            }
        }
    }
    return true;
}

void LGame::handleEvent() {
    if (IdxCandidatePiece == -1 && mEvent.type == SDL_MOUSEBUTTONDOWN) {
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        for (int i = 0; i < NUM_CANDIDATE; ++ i) {
            if (candidatePiece[i].piece.isClicked(tmpx, tmpy)) {
                IdxCandidatePiece = i;
                SDL_WarpMouseInWindow(mWindow, candidatePiece[IdxCandidatePiece].piece.getX(), candidatePiece[IdxCandidatePiece].piece.getY());
                break;
            }
        }
    }
    if (IdxCandidatePiece != -1 && mEvent.type == SDL_MOUSEBUTTONUP) {
        if (row != -1) {
            for (int i = 0; i < candidatePiece[IdxCandidatePiece].piece.getNumRowPattern(); ++ i) {
                for (int j = 0; j < candidatePiece[IdxCandidatePiece].piece.getLengthRowPattern(i); ++ j) {
                    if (candidatePiece[IdxCandidatePiece].piece.getCharacterPattern(i, j) == blockWood) {
                        currentBlock[i + row][j + col].type = blockWood;
                    }
                }
            }
            candidatePiece[IdxCandidatePiece].piece = rnd.getOneRandom(vectorPiece);
        }
        candidatePiece[IdxCandidatePiece].piece.setPosition(candidatePiece[IdxCandidatePiece].x, candidatePiece[IdxCandidatePiece].y);
        row = col = -1;
        IdxCandidatePiece = -1;
    }
    if (IdxCandidatePiece != -1 && mEvent.type == SDL_MOUSEMOTION) {
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        candidatePiece[IdxCandidatePiece].piece.setPosition(tmpx, tmpy);

        row = col = -1;
        for (int i = 0; i < SIZE; ++ i) {
            for (int j = 0; j < SIZE; ++ j) {
                if (inside(currentBlock[i][j].topLeftPosition.x, currentBlock[i][j].topLeftPosition.y,
                SQUARE_LENGTH, SQUARE_DISTANCE, tmpx, tmpy)) {
                    if (checkCurrentPieceInside(i, j)) {
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }
}

void LGame::loadPreview() {
    if (IdxCandidatePiece == -1 || row == -1) return;
    for (int i = 0; i < 10; ++ i) {
        for (int j = 0; j < 10; ++ j) {
            if (i == row && j == col) {
                candidatePiece[IdxCandidatePiece].piece.draw(mRenderer, 1, currentBlock[i][j].topLeftPosition.x, currentBlock[i][j].topLeftPosition.y);
            }
        }
    }
}

bool LGame::checkLosingGame() {

}

bool LGame::continueGame() {
    return true;
}

void LGame::updateScoreAndGameState() {
    std::vector<int> deleteRow, deleteCol;
    for (int i = 0; i < SIZE; ++ i) {
        bool okRow = true, okCol = true;
        for (int j = 0; j < SIZE; ++ j) {
            if (currentBlock[i][j].type != blockWood) {
                okRow = false;
            }    
            if (currentBlock[j][i].type != blockWood) {
                okCol = false;
            }
        }
        if (okRow) {
            deleteRow.push_back(i);
        }
        if (okCol) {
            deleteCol.push_back(i);
        }
    }
    for (auto tmp : deleteRow) {
        for (int i = 0; i < SIZE; ++ i) {
            currentBlock[tmp][i].type = blockBase;
        }
    }
    for (auto tmp : deleteCol) {
        for (int i = 0; i < SIZE; ++ i) {
            currentBlock[i][tmp].type = blockBase;
        }
    }
    int scoreRow = 10 + 10 * (int)deleteRow.size() * (int)deleteRow.size();
    int scoreCol = 10 + 10 * (int)deleteCol.size() * (int)deleteCol.size();
    if ((int)deleteRow.size()) {
        totalScore += scoreRow;
    } 
    if ((int)deleteCol.size()) {
        totalScore += scoreCol;
    }
    if ((int)deleteRow.size() && (int)deleteCol.size()) {
        totalScore += scoreRow + scoreCol;
    }
    std::stringstream ss; ss << totalScore;
    std::string scoreString; ss >> scoreString;
    printScore.loadFromRenderedText(mFont, mRenderer, scoreString, {255, 255, 255});
    scoreBorder.render(mRenderer, 180, 150);
    printScore.render(mRenderer, 190, 160);
}

void LGame::resetGame() {
    for (int i = 0; i < NUM_CANDIDATE; ++ i) {
        candidatePiece[i].piece = rnd.getOneRandom(vectorPiece);
        candidatePiece[i].piece.setPosition(candidatePiece[i].x, candidatePiece[i].y);
    }
    for (int i = 0; i < SIZE; ++ i) {
        for (int j = 0; j < SIZE; ++ j) {
            currentBlock[i][j].type = blockBase;
        }
    }
    quit = false;
    totalScore = 0;
    row = col = -1;
    IdxCandidatePiece = -1;
}

void LGame::playGame() {
    resetGame();
    while (!quit) {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);
        backGround.render(mRenderer, 0, 0);
        for (int i = 0; i < SIZE; ++ i) {
            for (int j = 0; j < SIZE; ++ j) {
                loadedBlock[currentBlock[i][j].type].render(mRenderer, currentBlock[i][j].topLeftPosition.x, currentBlock[i][j].topLeftPosition.y);
            }
        }
        for (int i = 0; i < NUM_CANDIDATE; ++ i) {
            if (i != IdxCandidatePiece) {
                candidatePiece[i].piece.draw(mRenderer, 0, candidatePiece[i].x, candidatePiece[i].y);
            }
        }
        loadPreview();
        if (IdxCandidatePiece != -1) {
            candidatePiece[IdxCandidatePiece].piece.draw(mRenderer, 0);
        }
        updateScoreAndGameState();
        SDL_RenderPresent(mRenderer);


        while (SDL_PollEvent(&mEvent)) {
            if (mEvent.type == SDL_QUIT) {
                quit = true;
            }
            handleEvent();
        }
    }   
}