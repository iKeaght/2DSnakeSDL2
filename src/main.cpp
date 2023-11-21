#include <SDL.h>
#include <iostream>
const int snakePartSize = 50;
const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
int main(int argc, char* args[]) {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;

	SDL_Init(SDL_INIT_EVERYTHING);
	bool running = true;
	SDL_Rect r{ 10,10,snakePartSize,snakePartSize };
	
	SDL_CreateWindowAndRenderer(ScreenWidth, ScreenHeight, 0, &window, &renderer);

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_RIGHT:
					r.x += 50;
					break;
				case SDLK_LEFT:
					r.x -= 50;
					break;

				case SDLK_UP:
					r.y -= 50;
					break;

				case SDLK_DOWN:
					r.y += 50;
					break;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_RIGHT:
					std::cout << "Right key is released" << std::endl;
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &r);

		SDL_RenderPresent(renderer);
		//SDL_Delay(10);
	}
	return 0;
}