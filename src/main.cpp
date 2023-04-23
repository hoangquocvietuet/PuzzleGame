#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "LTexture.h"
#include "LPiece.h"
#include "LGame.h"

const int WINDOW_WIDTH = 540;
const int WINDOW_HEIGHT = 960;

/*
    window that displays
*/
SDL_Window *gWindow;

/*
    renderer of the window
*/
SDL_Renderer *gRenderer;

/*
    font of the window
*/
TTF_Font *gFont = nullptr;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("Puzzle Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI);
        if (gWindow == nullptr)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == nullptr)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                gFont = TTF_OpenFont("../assets/consola.ttf", 50);
                if (gFont == nullptr)
                {
                    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
                {
                    printf("failed to initialize music mixer! Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    TTF_CloseFont(gFont);
    gFont = nullptr;
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

int main(int argc, char *argv[])
{
    if (!init())
    {
        printf("failed to initialize SDL!!!!\nSDL Error: %s\n", SDL_GetError());
    }
    else
    {
        LGame a(gRenderer, gWindow, gFont);
        a.startPage();
    }
    close();
    return 0;
}