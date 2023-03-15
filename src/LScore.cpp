#include "LScore.h"

void LScore::addScore(int addedScore) {
    totalScore += addedScore;
}

/*
    the middle x of rectangle is x + w / 2. We need to subtract width of scoreTexture to make it center
    the same think to y
*/
void LScore::printScore(SDL_Renderer* gRenderer, TTF_Font* gFont, int x, int y, int w, int h, long long score) {
    if (score == -1) score = totalScore;
    std::stringstream ss; ss << score;
    std::string scoreString; ss >> scoreString;
    scoreTexture.loadFromRenderedText(gFont, gRenderer, scoreString, {255, 255, 255});
    scoreTexture.render(gRenderer, x + (w - scoreTexture.getWidth()) / 2, y + (h - scoreTexture.getHeight()) / 2);
}

void LScore::setScore(long long newScore) {
    totalScore = newScore;
}

void LScore::addScore(int cntRow, int cntCol) {
    int scoreRow = 50 + 10 * cntRow * cntRow;
    int scoreCol = 50 + 10 * cntCol * cntCol;
    if (cntRow) addScore(scoreRow);
    if (cntCol) addScore(scoreCol);
    if (cntRow && cntCol) {
        addScore(scoreRow + scoreCol);
    } 
}

long long LScore::readScore(std::string path) {
    std::ifstream input(path.c_str());
    long long current;
    input >> current;
    return current;
}

void LScore::saveScore(std::string path) {
    long long current = readScore(path);
    if (current < totalScore) current = totalScore;
    std::ofstream output(path.c_str());
    output << current;
}

void LScore::doubleScore() {
    totalScore *= 2;
}