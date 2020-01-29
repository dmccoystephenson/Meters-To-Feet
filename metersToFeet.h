#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "Text.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

// text
Text prompt;
Text input;
Text output;

void init();

void loadMedia();

void cleanUp();

void drawRectangle(int x, int y, int w, int h, int r, int g, int b, int o);