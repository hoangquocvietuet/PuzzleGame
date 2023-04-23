#include "LButton.h"

bool LButton::init(SDL_Renderer *gRenderer, std::string path, int _x, int _y)
{
    bool success = true;
    if (!buttonTexture.loadFromFile(gRenderer, path))
    {
        printf("cant load button texture here\n");
        success = false;
    }
    x = _x;
    y = _y;
    return success;
}

void LButton::setPosition(int _x, int _y)
{
    x = _x;
    y = _y;
}

void LButton::render(SDL_Renderer *gRenderer, int _x, int _y)
{
    if (mouseFocusOn(_x, _y))
        buttonTexture.setAlpha(191);
    buttonTexture.render(gRenderer, x, y);
    if (mouseFocusOn(_x, _y))
        buttonTexture.setAlpha(255);
}

bool LButton::mouseFocusOn(int _x, int _y)
{
    if (_x >= x && _y >= y && _x < x + buttonTexture.getWidth() && _y < y + buttonTexture.getHeight())
        return true;
    return false;
}