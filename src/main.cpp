#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#pragma region Variables
const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
const char* IMAGE_PATH = "./Images/";
struct Vector2 {
	int x;
	int y;
};
//Snake
const int SnakePartSize = 48;
Vector2 snakeHeadPosition = { ScreenWidth - SnakePartSize,ScreenHeight - SnakePartSize };
Vector2 snakeBodyPosition = { snakeHeadPosition.x + SnakePartSize ,snakeHeadPosition.y};
Vector2 snakeTailPosition = { snakeBodyPosition.x + SnakePartSize ,snakeBodyPosition.y };

SDL_Rect snakeHeadRectangle = { snakeHeadPosition.x, snakeHeadPosition.y, SnakePartSize, SnakePartSize };
SDL_Rect snakeBodyRectangle = { snakeBodyPosition.x, snakeBodyPosition.y, SnakePartSize, SnakePartSize };
SDL_Rect snakeTailectangle = { snakeTailPosition.x, snakeTailPosition.y, SnakePartSize, SnakePartSize };

std::vector<SDL_Rect> snakeRects = { snakeHeadRectangle, snakeBodyRectangle, snakeTailectangle };
std::vector<Vector2> snakePositions = { snakeHeadPosition, snakeBodyPosition, snakeTailPosition };

//Fruit
const int FruitSize = 48;
Vector2 fruitPosition = { ScreenWidth - SnakePartSize, ScreenHeight - SnakePartSize };
SDL_Rect fruitRectangle{ fruitPosition.x, fruitPosition.y, FruitSize, FruitSize };

//Game
bool running = true;

enum Direction
{
	STOP,
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

struct Boundaries {
	int xStart = SnakePartSize;
	int yStart = SnakePartSize;
	//int xEnd = ScreenWidth - SnakePartSize;
	int xEnd = 2000;
	int yEnd = 1200;
	//int yEnd = ScreenHeight - SnakePartSize;
};

Direction direction;
Boundaries boundaries;
#pragma endregion


#pragma region Methods
void Render(SDL_Renderer* renderer, std::vector<SDL_Rect>& snakeRectangles, const SDL_Rect& fruitRect, SDL_Texture* snakeHeadTexture, SDL_Texture* snakeBodyTexture, SDL_Texture* snakeTailTexture, SDL_Texture* fruitTexture) {
	int head_rotation = 0;
	//Background
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderClear(renderer);

	//Fruit
	SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);


	//Snake
	//SDL_Rect head = snakeRectangles[0];
	//SDL_RenderCopy(renderer, snakeHeadTexture, NULL, &head);
	switch (direction) {
	case RIGHT:
		head_rotation = -90;
		break;
	case LEFT:
		head_rotation = 90;
		break;
	case UP:
		head_rotation = 180;
		break;
	case DOWN:
		head_rotation = 0;
		break;
	}
	SDL_Rect tailRect;
	if (!snakeRects.empty()) {
		tailRect = snakeRects.back();
	}
	SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &snakeRectangles[0], head_rotation, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, snakeBodyTexture, NULL, &snakeRects[1]);
	SDL_RenderCopy(renderer, snakeTailTexture, NULL, &tailRect);


	SDL_RenderPresent(renderer);
}

void HandleInputs(SDL_Event& event) {
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

void MoveSnake(std::vector<SDL_Rect>& snakeRects, std::vector<Vector2>& snakePositions) {
	switch (direction) {
	case RIGHT:
		if (snakeRects[0].x <= boundaries.xEnd) {
			snakePositions[0].x += SnakePartSize;
			snakeRects[0].x += SnakePartSize;
		}
		break;
	case LEFT:
		if (boundaries.xStart <= snakeRects[0].x) {
			snakePositions[0].x -= SnakePartSize;
			snakeRects[0].x -= SnakePartSize;
		}
		break;
	case UP:
		if (boundaries.yStart <= snakeRects[0].y) {
			snakePositions[0].y -= SnakePartSize;
			snakeRects[0].y -= SnakePartSize;
		}
		break;
	case DOWN:
		if (snakeRects[0].y <= boundaries.yEnd) {
			snakePositions[0].y += SnakePartSize;
			snakeRects[0].y += SnakePartSize;
		}
		break;
	default:
		break;
	}
}

int GetRandom(int min_value, int max_value, int step)
{
	int random_value = (rand() % ((++max_value - min_value) / step)) * step + min_value;
	return random_value;
}

void FruitUpdate(SDL_Rect& fruitRectangle, Vector2& fruitPosition) {
	int xPosition = GetRandom(0, ScreenWidth, SnakePartSize);
	int yPosition = GetRandom(0, ScreenHeight, SnakePartSize);
	fruitRectangle.x = xPosition;
	fruitRectangle.y = yPosition;
	fruitPosition.x = xPosition;
	fruitPosition.y = yPosition;
}

bool Vector2Equal(Vector2& vector1, Vector2 vector2, int step) {
	if ((vector1.x - step <= vector2.x && vector2.x <= vector1.x + step) && (vector1.y - step <= vector2.y && vector2.y <= vector1.y + step)) {
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
#pragma endregion



int main(int argc, char* args[]) {


	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* snakeHead = nullptr;
	SDL_Surface* snakeBody = nullptr;
	SDL_Surface* snakeTail = nullptr;
	SDL_Surface* apple = nullptr;
	SDL_Texture* snakeHeadTexture = nullptr;
	SDL_Texture* snakeBodyTexture = nullptr;
	SDL_Texture* snakeTailTexture = nullptr;
	SDL_Texture* appleTexture = nullptr;
	SDL_Event e;


	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_CreateWindowAndRenderer(ScreenWidth + 100, ScreenHeight + 100, 0, &window, &renderer);
	srand(time(0));
	snakeHead = LoadImage(snakeHead, "SnakeHead.png");
	snakeBody = LoadImage(snakeBody, "SnakeBody.png");
	snakeTail = LoadImage(snakeTail, "SnakeTail.png");
	apple = LoadImage(apple, "Apple.png");
	snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, snakeHead);
	snakeBodyTexture = SDL_CreateTextureFromSurface(renderer, snakeBody);
	snakeTailTexture = SDL_CreateTextureFromSurface(renderer, snakeTail);
	appleTexture = SDL_CreateTextureFromSurface(renderer, apple);


	while (running) {

		Render(renderer, snakeRects, fruitRectangle, snakeHeadTexture, snakeBodyTexture, snakeTailTexture, appleTexture);

		if (Vector2Equal(fruitPosition, snakePositions[0], 40)) {
			FruitUpdate(fruitRectangle, fruitPosition);
			//ADD SCORE
		}

		//Logic
		HandleInputs(e);
		MoveSnake(snakeRects, snakePositions);

		SDL_Delay(75);
	}
	SDL_FreeSurface(snakeHead);
	SDL_FreeSurface(apple);
	SDL_DestroyTexture(snakeHeadTexture);
	SDL_DestroyTexture(appleTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
