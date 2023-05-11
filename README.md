# PuzzleGame
my Advanced Programming project - a Puzzle Game built with SDL and C++.

## Window building

- First, install [`Mingw-w64`](https://sourceforge.net/projects/mingw-w64/files/), my version is `GCC-7.3.0 i686-posix-sjlj`.
- Second, download [`SDL2`](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/mingw/index.php), [`SDL2_mixer`](https://github.com/libsdl-org/SDL_mixer/releases), [`SDL2_image`](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.3), [`SDL2_ttf`](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2). Put all this stuff to a folder named `SDL2-dev`. Or else, just download [my installed library](https://drive.google.com/file/d/1nbja5W2XzALFDwNW9Le9KWG8nCsnyxcN/view?usp=sharing). Set it to `C:\SDL2-dev`.
- Third, install [`GNU MAKE`](https://stackoverflow.com/a/57042516/21271990).
- Fourth, clone this repo. Change directory to `src` folder. Open `cmd` and type `make -f Makefile_window`. It will compile into a program named `PuzzleGame.exe`.

## Linux building

- First install required packages as dependencies:
    - For Ubuntu, install `build-essential` via `sudo apt update && sudo apt install build-essential`.
    - For Arch, use command `sudo pacman -Sy base-devel`.
- Second, install `SDL2`, `SDL2_image`, `SDL2_ttf`, `SDL2_mixer`:
    - For Ubuntu: `sudo apt-get install libsdl2-dev`.
    - For Arch: `sudo pacmac -S sdl2 sdl2_ttf sdl2_mixer sdl2_image`.
- CD to `src`, run `make -f Makefile_linux && ./PuzzleGame`

--------------------------------------------

- Now enjoy the game..!😍😎😎

## Project Structure

- `assets/`: Game assets, contains images, text files, music files.
- `src/`: Source code of the project implementation.

## Game overview

![]

