#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#pragma region Variables
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
const char* IMAGE_PATH = "./Images/";
const int SNAKEPARTSIZE = 48;
struct Transform {
	int x;
	int y;
	int rotation;
};
//Snake
Transform snakeHeadTransform = { SCREENWIDTH - SNAKEPARTSIZE,SCREENHEIGHT - SNAKEPARTSIZE, 0 };
Transform snakeBodyTransform = { snakeHeadTransform.x + SNAKEPARTSIZE , snakeHeadTransform.y, 90 };
Transform snakeTailTransform = { snakeBodyTransform.x + SNAKEPARTSIZE , snakeBodyTransform.y, 90 };

SDL_Rect snakeHeadRectangle = { snakeHeadTransform.x, snakeHeadTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
SDL_Rect snakeBodyRectangle = { snakeBodyTransform.x, snakeBodyTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
SDL_Rect snakeTailectangle = { snakeTailTransform.x, snakeTailTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };

std::vector<SDL_Rect> snakeRects = { snakeHeadRectangle, snakeBodyRectangle, snakeTailectangle };
std::vector<Transform> snakeTransforms = { snakeHeadTransform, snakeBodyTransform, snakeTailTransform };

//Fruit
const int FruitSize = 48;
Transform fruitPosition = { SCREENWIDTH - SNAKEPARTSIZE, SCREENHEIGHT - SNAKEPARTSIZE };
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
	int xStart = SNAKEPARTSIZE;
	int yStart = SNAKEPARTSIZE;
	int xEnd = SCREENWIDTH - SNAKEPARTSIZE;
	int yEnd = SCREENHEIGHT - SNAKEPARTSIZE;
};

Direction direction;
Boundaries boundaries;
#pragma endregion


#pragma region Methods
void Render(SDL_Renderer* renderer, std::vector<SDL_Rect>& snakeRectangles, const SDL_Rect& fruitRect, SDL_Texture* snakeHeadTexture, SDL_Texture* snakeBodyTexture, SDL_Texture* snakeTailTexture, SDL_Texture* fruitTexture) {
	int indexTail = snakeTransforms.size() - 1;
	//Background
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderClear(renderer);

	//Fruit
	SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);

	//Snake
	SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &snakeRectangles[0], snakeTransforms[0].rotation, NULL, SDL_FLIP_NONE);
	for (int i = 1; i < snakeTransforms.size() - 1; i++) {
		SDL_RenderCopyEx(renderer, snakeBodyTexture, NULL, &snakeRectangles[i], snakeTransforms[i].rotation, NULL, SDL_FLIP_NONE);
	}
	SDL_RenderCopyEx(renderer, snakeTailTexture, NULL, &snakeRectangles[indexTail], snakeTransforms[indexTail].rotation, NULL, SDL_FLIP_NONE);


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

void MoveSnake(std::vector<SDL_Rect>& snakeRects, std::vector<Transform>& snakeTranforms) {

	Transform currentTransform{ snakeTranforms[0].x, snakeTranforms[0].y, snakeTranforms[0].rotation - 90 };
	Transform nextTransform{ snakeTranforms[0].x, snakeTranforms[0].y, snakeTranforms[0].rotation - 90 };

	switch (direction) {
	case RIGHT:
		if (snakeRects[0].x <= boundaries.xEnd) {
			snakeTranforms[0].x += SNAKEPARTSIZE;
			snakeTranforms[0].rotation = -90;
			snakeRects[0].x += SNAKEPARTSIZE;
		}
		break;
	case LEFT:
		if (boundaries.xStart <= snakeRects[0].x) {
			snakeTranforms[0].x -= SNAKEPARTSIZE;
			snakeTranforms[0].rotation = 90;

			snakeRects[0].x -= SNAKEPARTSIZE;
		}
		break;
	case UP:
		if (boundaries.yStart <= snakeRects[0].y) {
			snakeTranforms[0].y -= SNAKEPARTSIZE;
			snakeTranforms[0].rotation = 180;
			snakeRects[0].y -= SNAKEPARTSIZE;
		}
		break;
	case DOWN:
		if (snakeRects[0].y <= boundaries.yEnd) {
			snakeTranforms[0].y += SNAKEPARTSIZE;
			snakeTranforms[0].rotation = 0;
			snakeRects[0].y += SNAKEPARTSIZE;
		}
		break;
	default:
		break;
	}
	if (snakeRects.size() == snakeTranforms.size()) {
		for (int i = 1; i < snakeTranforms.size(); i++) {
			//if ((snakeRects[0].x <= boundaries.xEnd + (SNAKEPARTSIZE * i)) && (boundaries.xStart <= snakeRects[0].x + (SNAKEPARTSIZE * i)) && ( boundaries.yStart <= snakeRects[0].y + (SNAKEPARTSIZE * i)) && (snakeRects[0].y <= boundaries.yEnd + (SNAKEPARTSIZE * i))) {
			currentTransform = { snakeTranforms[i].x, snakeTranforms[i].y, snakeTranforms[i].rotation };
			snakeTranforms[i] = { nextTransform.x, nextTransform.y, nextTransform.rotation };
			snakeRects[i] = { nextTransform.x, nextTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
			nextTransform = { currentTransform.x, currentTransform.y, currentTransform.rotation };
			//}
		}
	}

}

int GetRandom(int min_value, int max_value, int step)
{
	int random_value = (rand() % ((++max_value - min_value) / step)) * step + min_value;
	return random_value;
}

void FruitUpdate(SDL_Rect& fruitRectangle, Transform& fruitPosition) {
	int xPosition = GetRandom(0, SCREENWIDTH, SNAKEPARTSIZE);
	int yPosition = GetRandom(0, SCREENHEIGHT, SNAKEPARTSIZE);
	fruitRectangle.x = xPosition;
	fruitRectangle.y = yPosition;
	fruitPosition.x = xPosition;
	fruitPosition.y = yPosition;
}
void AddSnakeBody(std::vector<SDL_Rect>& snakeRectangles, std::vector<Transform>& snakeTransforms) {
	//int indexTail = snakeTransforms.size() - 1;
	Transform newTransform{  snakeHeadTransform.x + SNAKEPARTSIZE , snakeHeadTransform.y, 90 }; 
	snakeTransforms.push_back(newTransform);
	SDL_Rect newRectangle = { newTransform.x, newTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
	snakeRectangles.push_back(newRectangle);

}

bool TransformEqual(Transform& vector1, Transform Transform, int step) {
	if ((vector1.x - step <= Transform.x && Transform.x <= vector1.x + step) && (vector1.y - step <= Transform.y && Transform.y <= vector1.y + step)) {
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

	SDL_CreateWindowAndRenderer(SCREENWIDTH + 100, SCREENHEIGHT + 100, 0, &window, &renderer);
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

		if (TransformEqual(fruitPosition, snakeTransforms[0], 40)) {
			FruitUpdate(fruitRectangle, fruitPosition);
			AddSnakeBody(snakeRects, snakeTransforms);
			//ADD SCORE
		}

		//Logic
		HandleInputs(e);
		MoveSnake(snakeRects, snakeTransforms);

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
