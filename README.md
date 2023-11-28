# 2D Snake SDL2

# About the repository
It is a personal project I created while learning about SDL and C++.<br>
After learning theory, I wanted to learn by praticing ! <br>


# How to use 
You can clone the repository or download the snake.zip file (it is windows only). <br>
I only built and ran it on Windows 10, but SDL2 is multiplatform, so you should be able to get it to build on anything so long as you understand your local OS's build system.

# Snake game
It is a snake clone where I wanted to create a game without a game engine with C++ and SDL2.
I created all the sprites for the game.. Musics are royalty-free.

## How to play 
Use the arrows to move around. You have to eat apples to make the snake grow. Eat as many apples as possible to reach a high rank!
Take care of your body and walls, or you'll die.
**Have fun !** <br>

## Code architecture
- I considered that all the code could be done in the `main.cpp` file which, in a more complicated game, would be a bad idea, but for something simple breaking everything apart would make it harder to understand.
- You can find it in the `src` folder.
- All the resources used are located inside `Resources` folder.
- You can find an executable in `bin/x64/Debug/2DSnakeSDL2.exe` or in the `Snake.zip` folder.

## Librairies used
I have used the following librairies : 

 - `SDL2` for user inputs, window and renderer.
 - `SDL2_image` for sprites and textures inside the game.
 - `SDL2_mixer` for the sound effect.
 - `SDL2_ttf` for the fonts used inside the game.
