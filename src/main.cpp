#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SnakePartSize = 48;
const int FruitSize = 48;
const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
const char* IMAGE_PATH = "./Images/";
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

struct Vector2 {
	int x;
	int y;
};
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

void MoveSnake(SDL_Rect& rect, Vector2& snakePosition) {
	switch (direction) {
	case RIGHT:
		if (rect.x <= boundaries.xEnd) {
			snakePosition.x += SnakePartSize;
			rect.x += SnakePartSize;
		}
		break;
	case LEFT:
		if (boundaries.xStart <= rect.x) {
			snakePosition.x -= SnakePartSize;
			rect.x -= SnakePartSize;
		}
		break;
	case UP:
		if (boundaries.yStart <= rect.y) {
			snakePosition.y -= SnakePartSize;
			rect.y -= SnakePartSize;
		}
		break;
	case DOWN:
		if (rect.y <= boundaries.yEnd) {
			snakePosition.y += SnakePartSize;
			rect.y += SnakePartSize;
		}
		break;
	default:
		break;
	}

}

bool Vector2Equal(Vector2& vector1, Vector2 vector2, int step) {
	if ((vector1.x - step <= vector2.x && vector2.x <= vector1.x + step )&& (vector1.y - step <= vector2.y && vector2.y <= vector1.y + step)) {
		return true;
	}
	return false;
}

SDL_Surface* LoadImage(SDL_Surface* image, const char* imageName) {
	char fullPath[30];
	strcpy_s(fullPath, IMAGE_PATH);
	strcat_s(fullPath, imageName);
	image = IMG_Load(fullPath);
	if (!image) {
		std::cout << "Image : " << imageName << "not loaded" << std::endl;
	}
	return image;

}


int GetRandom(int min_value, int max_value, int step)
{
	int random_value = (rand() % ((++max_value - min_value) / step)) * step + min_value;
	return random_value;
}


int main(int argc, char* args[]) {

	//Init
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* snakeHead = nullptr;
	SDL_Texture* snakeHeadTexture = nullptr;
	SDL_Event e;
	srand(time(0));
	//Snake
	Vector2 snakePosition = { ScreenWidth - SnakePartSize,ScreenHeight - SnakePartSize };
	SDL_Rect snakeRectangle = { snakePosition.x, snakePosition.y, SnakePartSize, SnakePartSize };
	SDL_Rect startPositionRect{ snakePosition.x, snakePosition.y, SnakePartSize, SnakePartSize };

	//Fruit
	Vector2 fruitPosition = { ScreenWidth - SnakePartSize, ScreenHeight - SnakePartSize };
	SDL_Rect fruitRectangle{ fruitPosition.x, fruitPosition.y, FruitSize, FruitSize };
	SDL_Rect fruitRectangleStart{ fruitPosition.x, fruitPosition.y, FruitSize, FruitSize };


	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_CreateWindowAndRenderer(ScreenWidth + 100, ScreenHeight + 100, 0, &window, &renderer);

	IMG_Init(IMG_INIT_PNG);
	snakeHead = LoadImage(snakeHead, "SnakeHead.png");
	snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, snakeHead);

	//Positionning

	//fruits
	SDL_SetRenderDrawColor(renderer, 204, 44, 36, 255);
	SDL_RenderFillRect(renderer, &fruitRectangleStart);

	//snake
	SDL_RenderCopy(renderer, snakeHeadTexture, NULL, &startPositionRect);


	SDL_RenderPresent(renderer);

	while (running) {

		//background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//fruits
		std::cout << "Position : " << "fruitX: " << fruitPosition.x << "&&Snake X : " << snakePosition.x << "--" << "fruity: " << fruitPosition.y << "&&Snake y : " << snakePosition.y << std::endl;
		if (Vector2Equal(fruitPosition, snakePosition, 40)) {
			
			int xPosition = GetRandom(0, ScreenWidth, SnakePartSize);
			int yPosition = GetRandom(0, ScreenHeight, SnakePartSize);
			fruitRectangle = {xPosition, yPosition, FruitSize, FruitSize };
		
			fruitPosition.x = xPosition;
			fruitPosition.y = yPosition;

		}

		//fruits
		SDL_SetRenderDrawColor(renderer, 204, 44, 36, 255);
		SDL_RenderFillRect(renderer, &fruitRectangle);
		

		//snake
		SDL_RenderCopy(renderer, snakeHeadTexture, NULL, &snakeRectangle);

		//Logic
		HandleInputs(e, snakeRectangle);
		MoveSnake(snakeRectangle, snakePosition);

		
		SDL_RenderPresent(renderer);
		SDL_Delay(100);
	}
	SDL_FreeSurface(snakeHead);
	SDL_DestroyTexture(snakeHeadTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
