#ifndef LBUTTON_H
#define LBUTTON_H
#include "LTexture.h"
#include "ALLConst.h"

#include "LButton.h"
#include <stdio.h>
#include <string>
class LButton {
	public:
        /*
            initialize: set Texture, coordinate
        */
        bool init(SDL_Renderer* gRenderer, std::string path, int _x, int _y);

        /*
            set coordinate of the button
        */
		void setPosition(int _x, int _y);

        /*
            @return 1 if mouse focus on the button, 0 otherwise
        */
        bool mouseFocusOn(int _x, int _y);

        /*
            render button texture
        */
		void render(SDL_Renderer* gRenderer, int _x, int _y);

	protected:
        /*
            coordinate of the button
        */
        int x, y;

        /*
            texture of the button
        */
        LTexture buttonTexture;
};

#endif