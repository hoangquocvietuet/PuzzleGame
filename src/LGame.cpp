#include "LGame.h"

LGame::LGame(SDL_Renderer *gRenderer, SDL_Window *gWindow, TTF_Font *gFont)
{
    mRenderer = gRenderer;
    mWindow = gWindow;
    mFont = gFont;
    playVideoWindow = SDL_CreateWindow("RickRoll", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_BORDERLESS);
    playVideoRenderer = SDL_CreateRenderer(playVideoWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_HideWindow(playVideoWindow);
    if (!loadGameMedia())
    {
        printf("fail to load game media\n");
    }
    for (int i = 0, ss = 250; i < 10; ++i, ss += SQUARE_DISTANCE)
    {
        for (int j = 0, st = 35; j < 10; ++j, st += SQUARE_DISTANCE)
        {
            currentBlock[i][j].x = st;
            currentBlock[i][j].y = ss;
            currentBlock[i][j].type = blockBase;
        }
    }
    for (int i = 0, ss = 20; i < NUM_CANDIDATE; ++i, ss += 180)
    {
        candidatePiece[i].x = ss;
        candidatePiece[i].y = 760;
    }
}

LGame::~LGame()
{
    mRenderer = nullptr;
    mFont = nullptr;
    mWindow = nullptr;
    SDL_DestroyWindow(playVideoWindow);
    SDL_DestroyRenderer(playVideoRenderer);
    playVideoWindow = nullptr;
    playVideoRenderer = nullptr;
}

bool LGame::loadGameMedia()
{
    bool success = true;
    if (!backGround.loadFromFile(mRenderer, "../assets/background.png"))
    {
        success = false;
    }
    if (!startGameBackGround.loadFromFile(mRenderer, "../assets/startgame-background.png"))
    {
        success = false;
    }
    if (!popUpEndGameBackGround.loadFromFile(mRenderer, "../assets/endgame-popup.png"))
    {
        success = false;
    }
    if (!loadBlockMedia(mRenderer))
    {
        success = false;
    }
    if (!startGameButton.init(mRenderer, "../assets/startgame-button.png", 130, 600))
    {
        success = false;
    }
    if (!restartGameButton.init(mRenderer, "../assets/restartgame-button.png", 130, 600))
    {
        success = false;
    }
    if (!exitGameButton.init(mRenderer, "../assets/exit.png", 470, 0))
    {
        success = false;
    }
    if (!doubleScoreButton.init(mRenderer, "../assets/doubleScore-button.png", 130, 500))
    {
        success = false;
    }
    if (!initPieceFromFile("../assets/pieces.txt"))
    {
        success = false;
    }
    for (int i = 0; i < 190; ++i)
    {
        std::stringstream ss;
        ss << (i + 1);
        std::string path;
        ss >> path;
        path = "../assets/rickroll/out" + path + ".png";
        if (!rickRoll[i].loadFromFile(playVideoRenderer, path))
        {
            success = false;
        }
    }
    rickRollMusic = Mix_LoadMUS("../assets/rickroll/audio.mp3");
    if (rickRollMusic == nullptr)
    {
        success = false;
    }
    startGameMusic = Mix_LoadMUS("../assets/startgame-music.mp3");
    if (startGameMusic == nullptr)
    {
        success = false;
    }
    playGameMusic = Mix_LoadMUS("../assets/playgame-music.mp3");
    if (playGameMusic == nullptr)
    {
        success = false;
    }
    return success;
}

bool LGame::initPieceFromFile(std::string path)
{
    vectorPiece.clear();
    std::ifstream input(path.c_str());
    if (!input)
    {
        printf("can not not open file contains pieces");
        return false;
    }
    std::string str = "";
    LPiece tmpPiece;
    while (input >> str)
    {
        if (str == "23052004")
        {
            vectorPiece.push_back(tmpPiece);
            tmpPiece.resetPattern();
            continue;
        }
        tmpPiece.addToPattern(str);
    }
    return true;
}

bool LGame::checkCurrentPieceInside(int u, int v, LPiece piece)
{
    if (u + piece.getNumRowPattern() - 1 >= SIZE)
        return false;
    for (int i = 0; i < piece.getNumRowPattern(); ++i)
    {
        if (v + piece.getLengthRowPattern(i) - 1 >= SIZE)
            return false;
        for (int j = 0; j < piece.getLengthRowPattern(i); ++j)
        {
            if (piece.getCharacterPattern(i, j) == blockWood && currentBlock[i + u][j + v].type == blockWood)
            {
                return false;
            }
        }
    }
    return true;
}

/*
 * step 1: click on 1 of 3 candidates
 * step 2: move it to where you want to put
 * step 3: enjoy the game!
 */
bool LGame::handleGameEvent()
{
    while (SDL_PollEvent(&mEvent))
    {
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        if (IdxCandidatePiece == -1 && mEvent.type == SDL_MOUSEBUTTONDOWN)
        {
            for (int i = 0; i < NUM_CANDIDATE; ++i)
            {
                if (candidatePiece[i].piece.mouseFocusOn(tmpx, tmpy) && candidatePiece[i].used == 0)
                {
                    IdxCandidatePiece = i;
                    SDL_WarpMouseInWindow(mWindow, candidatePiece[IdxCandidatePiece].piece.getX(), candidatePiece[IdxCandidatePiece].piece.getY());
                    break;
                }
            }
        }
        if (IdxCandidatePiece != -1 && mEvent.type == SDL_MOUSEBUTTONUP)
        {
            if (row != -1)
            {
                for (int i = 0; i < candidatePiece[IdxCandidatePiece].piece.getNumRowPattern(); ++i)
                {
                    for (int j = 0; j < candidatePiece[IdxCandidatePiece].piece.getLengthRowPattern(i); ++j)
                    {
                        if (candidatePiece[IdxCandidatePiece].piece.getCharacterPattern(i, j) == blockWood)
                        {
                            currentBlock[i + row][j + col].type = blockWood;
                        }
                    }
                }
                candidatePiece[IdxCandidatePiece].used = 1;
            }
            int cnt_notused = 0;
            for (int i = 0; i < NUM_CANDIDATE; ++ i) {
                if (candidatePiece[i].used == 0) ++ cnt_notused;
            }
            if (cnt_notused == 0) {
                for (int i = 0; i < NUM_CANDIDATE; ++ i) {
                    candidatePiece[i].piece = rnd.getOneRandom(vectorPiece);
                    candidatePiece[i].piece.setPosition(candidatePiece[i].x, candidatePiece[i].y);
                    candidatePiece[i].used = 0;
                }
            } else {
                candidatePiece[IdxCandidatePiece].piece.setPosition(candidatePiece[IdxCandidatePiece].x, candidatePiece[IdxCandidatePiece].y);
            }
            row = col = -1;
            IdxCandidatePiece = -1;
        }
        if (IdxCandidatePiece != -1 && mEvent.type == SDL_MOUSEMOTION)
        {
            candidatePiece[IdxCandidatePiece].piece.setPosition(tmpx, tmpy);

            row = col = -1;
            for (int i = 0; i < SIZE; ++i)
            {
                for (int j = 0; j < SIZE; ++j)
                {
                    if (inside(currentBlock[i][j].x, currentBlock[i][j].y,
                               SQUARE_LENGTH, SQUARE_DISTANCE, tmpx, tmpy))
                    {
                        if (checkCurrentPieceInside(i, j, candidatePiece[IdxCandidatePiece].piece))
                        {
                            row = i;
                            col = j;
                        }
                    }
                }
            }
        }
        if (mEvent.type == SDL_MOUSEBUTTONDOWN && exitGameButton.mouseFocusOn(tmpx, tmpy))
        {
            return true;
        }
    }
    return false;
}

void LGame::loadPreview()
{
    if (IdxCandidatePiece == -1 || row == -1)
        return;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (i == row && j == col)
            {
                candidatePiece[IdxCandidatePiece].piece.draw(mRenderer, 1, 0, currentBlock[i][j].x, currentBlock[i][j].y);
            }
        }
    }
}

bool LGame::checkLosingGame()
{
    for (int i = 0; i < NUM_CANDIDATE; ++i)
    {
        if (candidatePiece[i].used == 1) continue;
        for (int j = 0; j < SIZE; ++j)
        {
            for (int k = 0; k < SIZE; ++k)
            {
                if (checkCurrentPieceInside(j, k, candidatePiece[i].piece))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void LGame::updateScoreAndGameState()
{
    std::vector<int> deleteRow, deleteCol;
    for (int i = 0; i < SIZE; ++i)
    {
        bool okRow = true, okCol = true;
        for (int j = 0; j < SIZE; ++j)
        {
            if (currentBlock[i][j].type != blockWood)
            {
                okRow = false;
            }
            if (currentBlock[j][i].type != blockWood)
            {
                okCol = false;
            }
        }
        if (okRow)
        {
            deleteRow.push_back(i);
        }
        if (okCol)
        {
            deleteCol.push_back(i);
        }
    }
    for (auto tmp : deleteRow)
    {
        for (int i = 0; i < SIZE; ++i)
        {
            currentBlock[tmp][i].type = blockBase;
        }
    }
    for (auto tmp : deleteCol)
    {
        for (int i = 0; i < SIZE; ++i)
        {
            currentBlock[i][tmp].type = blockBase;
        }
    }
    sc.addScore((int)deleteRow.size(), (int)deleteCol.size());
}

void LGame::resetGame()
{
    for (int i = 0; i < NUM_CANDIDATE; ++i)
    {
        candidatePiece[i].piece = rnd.getOneRandom(vectorPiece);
        candidatePiece[i].piece.setPosition(candidatePiece[i].x, candidatePiece[i].y);
    }
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            currentBlock[i][j].type = blockBase;
        }
    }
    sc.setScore(0);
    row = col = -1;
    IdxCandidatePiece = -1;
}

void LGame::gameLoop()
{
    Mix_PlayMusic(playGameMusic, -1);
    resetGame();
    while (true)
    {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);
        {
            backGround.render(mRenderer, 0, 0);
            for (int i = 0; i < SIZE; ++i)
            {
                for (int j = 0; j < SIZE; ++j)
                {
                    loadedBlock[currentBlock[i][j].type].render(mRenderer, currentBlock[i][j].x, currentBlock[i][j].y);
                }
            }
            for (int i = 0; i < NUM_CANDIDATE; ++ i)
            {
                if (candidatePiece[i].used == 0 && i != IdxCandidatePiece)
                {
                    bool preview = true;
                    for (int j = 0; j < SIZE; ++j)
                    {
                        for (int k = 0; k < SIZE; ++k)
                        {
                            if (checkCurrentPieceInside(j, k, candidatePiece[i].piece))
                            {
                                preview = false;
                            }
                        }
                    }   
                    candidatePiece[i].piece.draw(mRenderer, preview, 1, candidatePiece[i].x, candidatePiece[i].y);
                }
            }
            loadPreview();
            if (IdxCandidatePiece != -1)
            {
                candidatePiece[IdxCandidatePiece].piece.draw(mRenderer, 0);
            }
            updateScoreAndGameState();
            sc.printScore(mRenderer, mFont, 178, 143, 162, 73);

            int tmpx, tmpy;
            SDL_GetMouseState(&tmpx, &tmpy);
            exitGameButton.render(mRenderer, tmpx, tmpy);
        }
        SDL_RenderPresent(mRenderer);
        if (checkLosingGame())
        {
            sc.saveScore("../assets/bestScore.txt");
            break;
        }
        // exit game
        if (handleGameEvent())
        {
            return;
        }
    }
    SDL_Delay(600);
    Mix_HaltMusic();
    endPage(0);
}

int LGame::handleStartPageEvent()
{
    while (SDL_PollEvent(&mEvent))
    {
        if (mEvent.type != SDL_MOUSEBUTTONDOWN)
            continue;
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        if (startGameButton.mouseFocusOn(tmpx, tmpy))
        {
            return 1;
        }
        if (exitGameButton.mouseFocusOn(tmpx, tmpy))
        {
            return 2;
        }
    }
    return 0;
}

void LGame::startPage()
{
    int stop = 0;
    Mix_PlayMusic(startGameMusic, -1);
    Mix_VolumeMusic(30);
    while (!stop)
    {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        startGameBackGround.render(mRenderer, 0, 0);
        startGameButton.render(mRenderer, tmpx, tmpy);
        exitGameButton.render(mRenderer, tmpx, tmpy);
        SDL_RenderPresent(mRenderer);
        stop = handleStartPageEvent();
    }
    Mix_HaltMusic();
    SDL_Delay(400);
    if (stop == 1)
        gameLoop();
}

int LGame::handleEndPageEvent(bool double_score)
{
    while (SDL_PollEvent(&mEvent))
    {
        if (mEvent.type != SDL_MOUSEBUTTONDOWN)
            continue;
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        if (restartGameButton.mouseFocusOn(tmpx, tmpy))
        {
            return 1;
        }
        if (exitGameButton.mouseFocusOn(tmpx, tmpy))
        {
            return 2;
        }
        if (!double_score && doubleScoreButton.mouseFocusOn(tmpx, tmpy))
        {
            return 3;
        }
    }
    return 0;
}

void LGame::playRickRoll()
{
    SDL_HideWindow(mWindow);
    SDL_ShowWindow(playVideoWindow);
    Mix_PlayMusic(rickRollMusic, -1);
    for (int i = 0; i < 190; ++i)
    {
        SDL_SetRenderDrawColor(playVideoRenderer, 255, 255, 255, 255);
        SDL_RenderClear(playVideoRenderer);
        rickRoll[i].render(playVideoRenderer, 0, 0);
        SDL_RenderPresent(playVideoRenderer);
        SDL_Delay(36);
    }
    Mix_HaltMusic();
    SDL_HideWindow(playVideoWindow);
    SDL_ShowWindow(mWindow);
}

void LGame::endPage(bool double_score)
{
    int stop = 0;
    while (!stop)
    {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);
        backGround.render(mRenderer, 0, 0);
        popUpEndGameBackGround.render(mRenderer, 35, 250);
        int tmpx, tmpy;
        SDL_GetMouseState(&tmpx, &tmpy);
        restartGameButton.render(mRenderer, tmpx, tmpy);
        exitGameButton.render(mRenderer, tmpx, tmpy);
        if (!double_score)
            doubleScoreButton.render(mRenderer, tmpx, tmpy);
        sc.printScore(mRenderer, mFont, 178, 143, 162, 73);
        sc.printScore(mRenderer, mFont, 280, 375, 100, 60, sc.readScore("../assets/bestScore.txt"));
        SDL_RenderPresent(mRenderer);
        stop = handleEndPageEvent(double_score);
    }
    if (stop == 1)
        gameLoop();
    if (stop == 3)
    {
        playRickRoll();
        sc.doubleScore();
        sc.saveScore("../assets/bestScore.txt");
        endPage(1);
    }
}