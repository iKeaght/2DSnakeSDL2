#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>


#pragma region Variables
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;
const char* IMAGE_PATH = "./images/";
const char* SOUND_PATH = "./audio/";
const int SNAKEPARTSIZE = 48;
struct Transform {
	int x;
	int y;
	int rotation;
};
struct Vector2 {
	union {
		struct { int width, height; } size;
		struct { int x, y; } position;
	};
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

std::vector<SDL_Rect> snakeRects = { snakeHeadRectangle };
std::vector<Transform> snakeTransforms = { snakeHeadTransform };


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
bool restart = false;
int score = 0;
TTF_Font* roboto;
Mix_Music* eatingSoundEffect;
Mix_Music* restartSoundEffect;
SDL_Color white = { 255, 255, 255 };

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


#pragma region Utility Methods
int GetRandom(int min_value, int max_value, int step)
{
	//Include max value 
	int max_included = max_value + step;
	int random_value = (rand() % ((++max_included - min_value) / step)) * step + min_value;
	return random_value;
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

Mix_Music* LoadMix(Mix_Music* music, const char* mixName) {
	char fullPath[30];
	strcpy_s(fullPath, SOUND_PATH);
	strcat_s(fullPath, mixName);
	music = Mix_LoadMUS(fullPath);
	if (!music) {
		std::cout << "Music : " << mixName << "not loaded" << std::endl;
	}
	return music;
}

void AddScore(int& score) {
	score += 1;
}

void RotationToDirection(int rotation) {
	switch (rotation)
	{
	case -90:
		direction = RIGHT;
		break;
	case 0:
		direction = DOWN;
		break;
	case 90:
		direction = LEFT;
		break;
	case 180:
		direction = UP;
		break;
	default:
		break;
	}
}

#pragma endregion

#pragma region Draw Methods
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
void DrawText(SDL_Renderer* renderer, std::string str, TTF_Font* font, SDL_Color color, Vector2 textPosition, Vector2 textSize) {

	if (!font) {
		std::cout << "Font : " << font << "not loaded" << std::endl;
	}
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, str.c_str(), color);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect messageRect{ textPosition.position.x, textPosition.position.y ,textSize.size.width, textSize.size.height };
	SDL_RenderCopy(renderer, message, NULL, &messageRect);
}
void Render(SDL_Renderer* renderer, std::vector<SDL_Rect>& snakeRectangles, SDL_Rect& fruitRect) {
	int indexTail = snakeTransforms.size() - 1;

	//Background
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
	SDL_RenderClear(renderer);
	DrawBorder(renderer);
	std::string str = "Score : " + std::to_string(score);
	DrawText(renderer, str, roboto, white, { 1520, 30 }, { 375, 125 });


	//Fruit
	SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);

	//Snake
	SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &snakeRectangles[0], snakeTransforms[0].rotation, NULL, SDL_FLIP_NONE);
	for (int i = 1; i < snakeTransforms.size() - 1; i++) {
		SDL_RenderCopyEx(renderer, snakeBodyTexture, NULL, &snakeRectangles[i], snakeTransforms[i].rotation, NULL, SDL_FLIP_NONE);
	}
	if (snakeTransforms.size() > 1) {
		SDL_RenderCopyEx(renderer, snakeTailTexture, NULL, &snakeRectangles[indexTail], snakeTransforms[indexTail].rotation, NULL, SDL_FLIP_NONE);
	}


	SDL_RenderPresent(renderer);
}

void RenderRestartMenu(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 80, 80, 80, 200);
	SDL_RenderClear(renderer);
	std::string str = "Press space to restart....";
	Vector2 textSize = { 1080, 150 };
	Vector2 textPosition = { SCREENWIDTH / 2 - textSize.size.width / 2, SCREENHEIGHT / 2 - textSize.size.height / 2 };
	DrawText(renderer, str, roboto, white, textPosition, textSize);
	SDL_RenderPresent(renderer);
}


#pragma endregion

#pragma region InGame Methods
bool CollisionsChecks(std::vector<Transform> snakeTransforms) {

	////Check snake collisions with itself
	for (int i = 0; i < snakeTransforms.size(); i++) {
		for (int j = i + 1; j < snakeTransforms.size() - i; j++) {
			if (snakeTransforms[i].x == snakeTransforms[j].x && snakeTransforms[i].y == snakeTransforms[j].y) {

				//Optimisation 
				snakeTransforms.erase(snakeTransforms.begin());
				return true;
			}
		}
	}

	//Check snake collisions with walls
	if (snakeTransforms[0].x <= boundaries.xStart + 20 || snakeTransforms[0].x >= boundaries.xEnd - 20 || snakeTransforms[0].y <= boundaries.yStart + 20 || snakeTransforms[0].y >= boundaries.yEnd - 20) {
		return true;
	}
	return false;
}

void HandleInputs(SDL_Event& event) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			restart = false;
			running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				if (direction != LEFT) {
					direction = RIGHT;
				}
				break;
			case SDLK_LEFT:
				if (direction != RIGHT) {
					direction = LEFT;
				}
				break;
			case SDLK_UP:
				if (direction != DOWN) {
					direction = UP;
				}
				break;
			case SDLK_DOWN:
				if (direction != UP) {
					direction = DOWN;
				}
				break;

			case SDLK_SPACE:
				if (restart) {
					restart = false;
				}
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
			currentTransform = { snakeTranforms[i].x, snakeTranforms[i].y, snakeTranforms[i].rotation };
			snakeTranforms[i] = { nextTransform.x, nextTransform.y, nextTransform.rotation };
			snakeRects[i] = { nextTransform.x, nextTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
			nextTransform = { currentTransform.x, currentTransform.y, currentTransform.rotation };
		}
	}

}

void FruitUpdate(SDL_Rect& fruitRectangle, Transform& fruitPosition) {
	int xPosition = GetRandom(boundaries.xStart + SNAKEPARTSIZE, boundaries.xEnd - SNAKEPARTSIZE, SNAKEPARTSIZE);
	int yPosition = GetRandom(boundaries.yStart + SNAKEPARTSIZE, boundaries.yEnd - SNAKEPARTSIZE, SNAKEPARTSIZE);
	fruitRectangle.x = xPosition;
	fruitRectangle.y = yPosition;
	fruitPosition.x = xPosition;
	fruitPosition.y = yPosition;
}

void AddSnakeBody(std::vector<SDL_Rect>& snakeRectangles, std::vector<Transform>& snakeTransforms) {
	Transform newTransform{ snakeHeadTransform.x + SNAKEPARTSIZE , snakeHeadTransform.y, 90 };
	snakeTransforms.push_back(newTransform);
	SDL_Rect newRectangle = { newTransform.x, newTransform.y, SNAKEPARTSIZE, SNAKEPARTSIZE };
	snakeRectangles.push_back(newRectangle);

}

void Restart() {

	FruitUpdate(fruitRects, fruitPosition);

	//restart snake
	snakeTransforms.resize(1);
	snakeRects.resize(1);

	int rot = GetRandom(-90, 180, 90);
	RotationToDirection(rot);
	//1248 and 720 are values to avoid spawning near borders.
	snakeTransforms[0] = { GetRandom(192,  1248, SNAKEPARTSIZE), GetRandom(192,  720, SNAKEPARTSIZE), rot };
	snakeRects[0] = { snakeTransforms[0].x, snakeTransforms[0].y, SNAKEPARTSIZE, SNAKEPARTSIZE };

	score = 0;
	restart = false;
}

void HandleRestartInput(SDL_Event& event) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			restart = false;
			running = false;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				if (restart) {
					Restart();

				}
				break;
			}

		}

	}
}
#pragma endregion


int main(int argc, char* args[]) {


	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;

	//Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error with SDL_Init : " << SDL_GetError() << std::endl;
		return -1;
	};
	
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cout << "Error with IMG_Init : " << IMG_GetError() << std::endl;
		return -1;
	};
	if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
		std::cout << "Error with Mix_Init : " << Mix_GetError() << std::endl;
		return -1;
	};
	if (TTF_Init() != 0) {
		std::cout << "Error with TTF_Init : " << TTF_GetError() << std::endl;
		return -1;
	};
	

	//Initialize window and renderer
	SDL_CreateWindowAndRenderer(SCREENWIDTH, SCREENHEIGHT, 0, &window, &renderer);

	//Global config
	srand(time(0));
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_VolumeMusic(10);

	//Load Image, Sound and Font inside datastructures
	snakeHead = LoadImage(snakeHead, "SnakeHead.png");
	snakeBody = LoadImage(snakeBody, "SnakeBody.png");
	snakeTail = LoadImage(snakeTail, "SnakeTail.png");
	apple = LoadImage(apple, "Apple.png");
	eatingSoundEffect = LoadMix(eatingSoundEffect, "eatingSoundEffect.mp3");
	restartSoundEffect = LoadMix(restartSoundEffect, "restart.mp3");
	roboto = TTF_OpenFont("./font/Roboto-Black.ttf", 120);

	//Load Textures
	snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, snakeHead);
	snakeBodyTexture = SDL_CreateTextureFromSurface(renderer, snakeBody);
	snakeTailTexture = SDL_CreateTextureFromSurface(renderer, snakeTail);
	fruitTexture = SDL_CreateTextureFromSurface(renderer, apple);


	while (running) {
		while (!restart && running) {
			Render(renderer, snakeRects, fruitRects);

			if (TransformEqual(fruitPosition, snakeTransforms[0], 40)) {

				Mix_PlayMusic(eatingSoundEffect, 1);
				FruitUpdate(fruitRects, fruitPosition);
				AddSnakeBody(snakeRects, snakeTransforms);
				AddScore(score);
			}

			HandleInputs(e);
			MoveSnake(snakeRects, snakeTransforms);

			if (CollisionsChecks(snakeTransforms)) {
				Mix_PlayMusic(restartSoundEffect, 1);
				RenderRestartMenu(renderer);
				restart = true;
			}

			while (restart && running) {
				HandleRestartInput(e);
			}

			SDL_Delay(100);
		}
	}
	//TODO : free space texture and surface for font / UI text
	Mix_FreeMusic(eatingSoundEffect);
	Mix_FreeMusic(restartSoundEffect);
	SDL_FreeSurface(snakeHead);
	SDL_FreeSurface(snakeBody);
	SDL_FreeSurface(snakeTail);
	SDL_FreeSurface(apple);
	SDL_DestroyTexture(snakeHeadTexture);
	SDL_DestroyTexture(snakeBodyTexture);
	SDL_DestroyTexture(snakeTailTexture);
	SDL_DestroyTexture(fruitTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}


