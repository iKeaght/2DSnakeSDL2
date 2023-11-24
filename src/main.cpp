#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#pragma region Variables
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
const char* IMAGE_PATH = "./images/";
const int SNAKEPARTSIZE = 48;
struct Transform {
	int x;
	int y;
	int rotation;
};
struct Boundaries {
	int xStart = 48;
	int yStart = 48;
	int xEnd = 1440;
	int yEnd = 960;
};
Boundaries boundaries;

//Snake
Transform snakeHeadTransform = { boundaries.xEnd - 300,boundaries.yEnd - 300, 0 };
Transform snakeBodyTransform = { snakeHeadTransform.x + SNAKEPARTSIZE , snakeHeadTransform.y, 90 };
Transform snakeTailTransform = { snakeBodyTransform.x + SNAKEPARTSIZE , snakeBodyTransform.y, 90 };

SDL_Rect snakeHeadRectangle = { snakeHeadTransform.x, snakeHeadTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
SDL_Rect snakeBodyRectangle = { snakeBodyTransform.x, snakeBodyTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
SDL_Rect snakeTailectangle = { snakeTailTransform.x, snakeTailTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };

std::vector<SDL_Rect> snakeRects = { snakeHeadRectangle, snakeBodyRectangle, snakeTailectangle };
std::vector<Transform> snakeTransforms = { snakeHeadTransform, snakeBodyTransform, snakeTailTransform };
//When collisions, remove element, to optimisation + return when found


SDL_Surface* snakeHead = nullptr;
SDL_Surface* snakeBody = nullptr;
SDL_Surface* snakeTail = nullptr;
SDL_Texture* snakeHeadTexture = nullptr;
SDL_Texture* snakeBodyTexture = nullptr;
SDL_Texture* snakeTailTexture = nullptr;

//Fruit
const int FruitSize = 48;
Transform fruitPosition = { boundaries.xEnd - 300, boundaries.yEnd - 222 };
SDL_Rect fruitRects{ fruitPosition.x, fruitPosition.y, FruitSize, FruitSize };
SDL_Surface* apple = nullptr;
SDL_Texture* fruitTexture = nullptr;

//Game
bool running = true;
int score = 0;
TTF_Font* roboto;
enum Direction
{
	STOP,
	UP,
	RIGHT,
	DOWN,
	LEFT,
};



Direction direction;
#pragma endregion


#pragma region Methods
void DrawBorder(SDL_Renderer* renderer) {
	SDL_Rect rectangle;
	SDL_Rect rectangle2;

	for (int x = boundaries.xStart; x <= boundaries.xEnd; x += SNAKEPARTSIZE) {
		rectangle = { x, boundaries.yStart, SNAKEPARTSIZE, SNAKEPARTSIZE };
		rectangle2 = { x, boundaries.yEnd, SNAKEPARTSIZE, SNAKEPARTSIZE };
		SDL_SetRenderDrawColor(renderer, 89, 120, 44, 255);
		SDL_RenderFillRect(renderer, &rectangle);
		SDL_RenderFillRect(renderer, &rectangle2);
	}

	for (int y = boundaries.yStart; y <= boundaries.yEnd; y += SNAKEPARTSIZE) {
		rectangle = { boundaries.xStart, y, SNAKEPARTSIZE, SNAKEPARTSIZE };
		rectangle2 = { boundaries.xEnd, y, SNAKEPARTSIZE, SNAKEPARTSIZE };
		SDL_SetRenderDrawColor(renderer, 89, 120, 44, 255);
		SDL_RenderFillRect(renderer, &rectangle);
		SDL_RenderFillRect(renderer, &rectangle2);
	}
}
void DrawText(SDL_Renderer* renderer, TTF_Font* font) {

	if (!font) {
		std::cout << "Font : " << font << "not loaded" << std::endl;
	}
	SDL_Color white = { 255,255,255 };
	std::string str = "Score : " + std::to_string(score);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, str.c_str(), white);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect messageRect{ 1520,30,375,125 };
	SDL_RenderCopy(renderer, message, NULL, &messageRect);
}
void Render(SDL_Renderer* renderer, std::vector<SDL_Rect>& snakeRectangles, SDL_Rect& fruitRect) {
	int indexTail = snakeTransforms.size() - 1;

	//Background
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderClear(renderer);
	DrawBorder(renderer);
	DrawText(renderer, roboto);


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

bool CollisionsChecks(std::vector<Transform> snakeTransforms) {
	for (int i = 0; i < snakeTransforms.size(); i++) {
		for (int j = i + 1; j < snakeTransforms.size() - i; j++) {
			if (snakeTransforms[i].x == snakeTransforms[j].x && snakeTransforms[i].y == snakeTransforms[j].y) {
				//hit
				std::cout << "hit" << std::endl;
				snakeTransforms.erase(snakeTransforms.begin());
				return true;
			}
		}
	}
	//not hit
	std::cout << "not hited" << std::endl;
	return false;
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

	int offsetEnd = boundaries.xStart + SNAKEPARTSIZE;
	int offsetStart = boundaries.xStart + 35;
	switch (direction) {
	case RIGHT:
		if (snakeRects[0].x <= boundaries.xEnd - offsetEnd) {
			snakeTranforms[0].x += SNAKEPARTSIZE;
			snakeTranforms[0].rotation = -90;
			snakeRects[0].x += SNAKEPARTSIZE;
		}
		break;
	case LEFT:
		if (boundaries.xStart + offsetStart <= snakeRects[0].x) {
			snakeTranforms[0].x -= SNAKEPARTSIZE;
			snakeTranforms[0].rotation = 90;

			snakeRects[0].x -= SNAKEPARTSIZE;
		}
		break;
	case UP:
		if (boundaries.yStart + offsetStart <= snakeRects[0].y) {
			snakeTranforms[0].y -= SNAKEPARTSIZE;
			snakeTranforms[0].rotation = 180;
			snakeRects[0].y -= SNAKEPARTSIZE;
		}
		break;
	case DOWN:
		if (snakeRects[0].y <= boundaries.yEnd - offsetEnd) {
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
	int xPosition = GetRandom(boundaries.xStart + SNAKEPARTSIZE, boundaries.xEnd - SNAKEPARTSIZE, SNAKEPARTSIZE);
	int yPosition = GetRandom(boundaries.yStart + SNAKEPARTSIZE, boundaries.yEnd - SNAKEPARTSIZE, SNAKEPARTSIZE);
	fruitRectangle.x = xPosition;
	fruitRectangle.y = yPosition;
	fruitPosition.x = xPosition;
	fruitPosition.y = yPosition;
}
void AddScore(int& score) {
	score += 1;
}
void AddSnakeBody(std::vector<SDL_Rect>& snakeRectangles, std::vector<Transform>& snakeTransforms) {
	Transform newTransform{ snakeHeadTransform.x + SNAKEPARTSIZE , snakeHeadTransform.y, 90 };
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
	SDL_Event e;

	//Init
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	//Initialize window and renderer
	SDL_CreateWindowAndRenderer(SCREENWIDTH, SCREENHEIGHT, 0, &window, &renderer);

	srand(time(0));
	snakeHead = LoadImage(snakeHead, "SnakeHead.png");
	snakeBody = LoadImage(snakeBody, "SnakeBody.png");
	snakeTail = LoadImage(snakeTail, "SnakeTail.png");
	apple = LoadImage(apple, "Apple.png");
	snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, snakeHead);
	snakeBodyTexture = SDL_CreateTextureFromSurface(renderer, snakeBody);
	snakeTailTexture = SDL_CreateTextureFromSurface(renderer, snakeTail);
	fruitTexture = SDL_CreateTextureFromSurface(renderer, apple);
	roboto = TTF_OpenFont("./font/Roboto-Black.ttf", 120);


	while (running) {

		Render(renderer, snakeRects, fruitRects);

		if (TransformEqual(fruitPosition, snakeTransforms[0], 40)) {
			FruitUpdate(fruitRects, fruitPosition);
			AddSnakeBody(snakeRects, snakeTransforms);
			AddScore(score);
		}

		//Logic
		HandleInputs(e);
		MoveSnake(snakeRects, snakeTransforms);
		CollisionsChecks(snakeTransforms);
		SDL_Delay(75);
	}
	//TODO : free space texture and surface for font / UI text
	TTF_Quit();
	SDL_FreeSurface(snakeHead);
	SDL_FreeSurface(apple);
	SDL_DestroyTexture(snakeHeadTexture);
	SDL_DestroyTexture(fruitTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
