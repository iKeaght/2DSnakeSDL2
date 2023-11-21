#include <SDL.h>
#include <iostream>
const int SnakePartSize = 50;
const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
enum Direction
{
	STOP,
	UP,
	RIGHT,
	DOWN,
	LEFT,
};
Direction direction;
struct Boundaries {
	int xStart = SnakePartSize;
	int yStart = SnakePartSize;
	int xEnd = ScreenWidth - SnakePartSize;
	int yEnd = ScreenHeight - SnakePartSize;
};
Boundaries boundaries;
bool running = true;
const int xBoundaries = ScreenWidth - SnakePartSize;
const int yBoundaries = ScreenHeight - SnakePartSize;


void HandleInputs(SDL_Event& event, SDL_Rect& rect) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {

			running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				direction = RIGHT;
				break;
			case SDLK_LEFT:
				direction = LEFT;
				break;
			case SDLK_UP:
				direction = UP;
				break;
			case SDLK_DOWN:
				direction = DOWN;
				break;
			}

		}
	}
}

void MoveSnake(SDL_Rect& rect) {
	switch (direction) {
	case RIGHT:
		std::cout << "Right" << std::endl;
		if (rect.x <= boundaries.xEnd) {
			rect.x += 50;
		}
		break;
	case LEFT:
		std::cout << "Left" << std::endl;
		if (boundaries.xStart <= rect.x) {
			rect.x -= 50;
		}
		break;
	case UP:
		std::cout << "Up" << std::endl;
		if (boundaries.yStart <= rect.y) {
			rect.y -= 50;
		}
		break;
	case DOWN:
		std::cout << "Down" << std::endl;
		if (rect.y <= boundaries.yEnd) {
			rect.y += 50;
		}
		break;
	default:
		break;
	}

}
int main(int argc, char* args[]) {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Rect r{ 1920 - 50,1080 - 50,SnakePartSize,SnakePartSize };

	SDL_CreateWindowAndRenderer(ScreenWidth + 100, ScreenHeight + 100, 0, &window, &renderer);

	while (running) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &r);

		SDL_RenderPresent(renderer);
		HandleInputs(e, r);
		MoveSnake(r);
		SDL_Delay(100);
	}
	return 0;
}
