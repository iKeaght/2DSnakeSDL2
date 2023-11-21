#include <SDL.h>
#include <iostream>
const int SnakePartSize = 50;
const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
struct Boundaries {
	int xStart = SnakePartSize;
	int yStart = SnakePartSize;
	int xEnd = ScreenWidth - SnakePartSize;
	int yEnd = ScreenHeight - SnakePartSize;
};
const int xBoundaries = ScreenWidth - SnakePartSize;
const int yBoundaries = ScreenHeight - SnakePartSize;


void HandleInputs(bool& running, Boundaries& bound, SDL_Event& event, SDL_Rect& rect) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				if (rect.x <= bound.xEnd) {
					rect.x += 50;
				}
				break;
			case SDLK_LEFT:
				if (bound.xStart <= rect.x) {
					rect.x -= 50;
				}
				break;

			case SDLK_UP:
				if (bound.yStart <= rect.y) {
					rect.y -= 50;
				}
				break;

			case SDLK_DOWN:
				if (rect.y <= bound.yEnd) {
					rect.y += 50;
				}
				break;
			}

		}
	}
}

int main(int argc, char* args[]) {
	Boundaries boundaries;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;

	SDL_Init(SDL_INIT_EVERYTHING);
	bool running = true;
	SDL_Rect r{ 1920 - 50,1080 - 50,SnakePartSize,SnakePartSize };

	SDL_CreateWindowAndRenderer(ScreenWidth + 100, ScreenHeight + 100, 0, &window, &renderer);

	while (running) {
		HandleInputs(running, boundaries, e, r);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &r);

		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
	return 0;
}

void Input(bool& running, Boundaries& bound, SDL_Event& event, SDL_Rect& rect) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				if (rect.x <= bound.xEnd) {
					rect.x += 50;
				}
				break;
			case SDLK_LEFT:
				if (bound.xStart <= rect.x) {
					rect.x -= 50;
				}
				break;

			case SDLK_UP:
				if (bound.yStart <= rect.y) {
					rect.y -= 50;
				}
				break;

			case SDLK_DOWN:
				if (rect.y <= bound.yEnd) {
					rect.y += 50;
				}
				break;
			}

		}
	}
}