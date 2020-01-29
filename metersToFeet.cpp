#include "metersToFeet.h"

using namespace std;

Text::Text() {
	xpos = 0;
	ypos = 0;
	width = 0;
	height = 0;
	texture = NULL;
	innerText = "";
}

Text::~Text() {
	free();
}

void Text::setPosition(int x, int y) {
	xpos = x;
	ypos = y;
}

void Text::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Text::loadText(std::string textToRender, SDL_Color textColor) {
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* temp_surface = TTF_RenderText_Solid(gFont, textToRender.c_str(), textColor);
	newTexture = SDL_CreateTextureFromSurface(gRenderer, temp_surface);
	width = temp_surface->w;
	height = temp_surface->h;
	SDL_FreeSurface(temp_surface);
	texture = newTexture;
}

void Text::render() {
	SDL_Rect renderQuad = {xpos, ypos, width, height};
	SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
}

int Text::getX() {
	return xpos;
}

int Text::getY() {
	return ypos;
}

int Text::getWidth() {
	return width;
}

int Text::getHeight() {
	return height;
}

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	gWindow = SDL_CreateWindow("Meters To Feet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Init();
}

void loadMedia() {
	// open the font
	gFont = TTF_OpenFont("lazy.ttf", 48);
}

void cleanUp() {
	prompt.free();
	input.free();
	output.free();
	
	TTF_CloseFont(gFont);
	gFont = NULL;
	
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	TTF_Quit();
	SDL_Quit();
}

void drawRectangle(int x, int y, int w, int h, int r, int g, int b, int o) {
	SDL_SetRenderDrawColor(gRenderer, r, g, b, o);
	SDL_Rect fillRect = {x, y, w, h};
	SDL_RenderFillRect(gRenderer, &fillRect);
}

int main(int argc, char* args[]) {
	init();
	loadMedia();
	
	SDL_Color color = {0, 0, 0, 0xFF};	
	prompt.loadText("Enter int in meters:", color);
	
	std::string inputText = "(input here)";
	input.loadText(inputText, color);
	
	std::string outputText = "";
	
	prompt.setPosition(SCREEN_WIDTH/2 - prompt.getWidth()/2, SCREEN_HEIGHT/8);
	input.setPosition(SCREEN_WIDTH/2 - input.getWidth()/2, SCREEN_HEIGHT/3);
	output.setPosition(SCREEN_WIDTH/2 - output.getWidth()/2, SCREEN_HEIGHT/2);
	
	// enable text input
	SDL_StartTextInput();
	
	SDL_Event e;
	bool running = true;
	while (running) {
		// rerender text flags
		bool renderText = false;
		
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			// special key input
			else if (e.type == SDL_KEYDOWN) {
				// handle backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
					// delete character
					inputText.pop_back();
					renderText = true;
				}
				// handle copy
				else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					SDL_SetClipboardText(inputText.c_str());
				}
				// handle paste
				else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
				// handle enter
				else if (e.key.keysym.sym == SDLK_RETURN) {
					if (inputText.size() > 0) {
						double feet = 0;
						int meters = atoi(inputText.c_str());
						feet = meters * 3.2808; // from https://www.metric-conversions.org/length/meters-to-feet.htm
						outputText = to_string(feet) + " ft";
					}
					else {
						outputText = "";
					}
					renderText = true;
				}
			}
			// special text input event
			else if (e.type == SDL_TEXTINPUT) {
				// not copy or pasting
				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
					// append character
					inputText += e.text.text;
					renderText = true;
				}
			}
		}
		
		// if rerender text needed
		if (renderText) {
			// if text is not empty
			if (inputText != "") {
				// render new text
				input.loadText(inputText, color);	
			}
			// text is empty
			else {
				// render space texture
				input.loadText(" ", color);
			}
			// if text is not empty
			if (outputText != "") {
				// render new text
				output.loadText(outputText, color);	
			}
			// text is empty
			else {
				// render space texture
				output.loadText(" ", color);
			}
			input.setPosition(SCREEN_WIDTH/2 - input.getWidth()/2, SCREEN_HEIGHT/3);
			output.setPosition(SCREEN_WIDTH/2 - output.getWidth()/2, SCREEN_HEIGHT/2);
		}
		
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		
		prompt.render();
		
//		drawRectangle(input.getX() - 25, input.getY() - 25, input.getWidth() + 50, input.getHeight() + 50, 25, 100, 200, 0);
		input.render();
		
		output.render();
		
		SDL_RenderPresent(gRenderer);
	}
	cleanUp();
}