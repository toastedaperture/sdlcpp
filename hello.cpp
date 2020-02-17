#include <iostream>
#include <SDL2/SDL.h>
//#include "res_path.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Surface *bmp;
static SDL_Texture *tex;
std::string imagePath = "wave.bmp";
std::string imagePath2 = "wave2.bmp";

int currentPage;
int newTex;
std::string page[] = {
                      imagePath,
                      imagePath2};

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

//int renderTex(tex){
//    //First clear the renderer
//	SDL_RenderClear(ren);
//	//Draw the texture
//	SDL_RenderCopy(ren, tex, NULL, NULL);
//	//Update the screen
//	SDL_RenderPresent(ren);
//	//Take a quick break after all that hard work
//	SDL_Delay(1000);
//}

int initSDLWindow(){
	//First we need to start up SDL, and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	//Now create a window with title "Hello World" at 100, 100 on the screen with w:640 h:480 and show it
	win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	//Make sure creating our window went ok
	if (win == nullptr){
		logSDLError(std::cout, "CreateWindow");
		return 1;
	}
    return 1;
}




int main(int, char**){
    currentPage = 1;
    initSDLWindow();

	//Create a renderer that will draw to the window, -1 specifies that we want to load whichever
	//video driver supports the flags we're passing
	//Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
	//SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
	//synchronized with the monitor's refresh rate
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		logSDLError(std::cout, "CreateRenderer");
		SDL_Quit();
		return 1;
	}

	//SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
	//this lets us choose when to upload or remove textures from the GPU
	bmp = SDL_LoadBMP((page[currentPage]).c_str());
	if (bmp == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//To use a hardware accelerated texture for rendering we can create one from
	//the surface we loaded
	tex = SDL_CreateTextureFromSurface(ren, bmp);
	//We no longer need the surface
	SDL_FreeSurface(bmp);
	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	//First clear the renderer
	SDL_RenderClear(ren);
	//Draw the texture
	SDL_RenderCopy(ren, tex, NULL, NULL);
	//Update the screen
	SDL_RenderPresent(ren);
	//Take a quick break after all that hard work
	SDL_Delay(1000);

	
	//Our event structure
	SDL_Event e;
	//For tracking if we want to quit
	bool quit = false;
	while (!quit){
		//Read any events that occured, for now we'll just quit if any event occurs
		while (SDL_PollEvent(&e)){
			//If user closes the window
			if (e.type == SDL_QUIT){
				quit = true;
			}
			//If user presses any key
			if (e.type == SDL_KEYDOWN){
				quit = true;
			}
			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN){
				quit = true;
			}
		}
	}

	//Clean up our objects and quit
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

