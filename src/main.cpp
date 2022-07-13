#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>
#include "AppUtil.cpp"

const int screenWidth = 800;
const int screenHeight = 600;
const int segmentSize = 1;
const int distancesFromCenter[5] = {10, 30, 40, 80, 120};
const int segmentWidth = 5;

void handleEvents(SDL_Window *window, bool mainWindow = false){
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0){
		if (event.type == SDL_QUIT){
			SDL_DestroyWindow(window);
			SDL_Quit();
			if (mainWindow){
				std::exit(0);
			}
		}
	}
}


int enlimit(int num, int limit){
	if (num % (limit * 2) < limit){
		return num % limit;
	} else {
		return limit - (num % limit);
	}
}


int main(int argc, char* argv[]) {
	std::vector<std::string> args;
    for(int i = 1; i < argc; i++){
        args.push_back(argv[i]);   
    };
	AppSettings::apply(args);

	if (AppSettings::help){
		std::fstream file;
		file.open("./README.md");
		std::cout << file.rdbuf() << "\n";
		file.close();
		return 0;
	}


	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Error: \e48;2;0;0;0m\e[38;2;255;0;0m" << SDL_GetError() << "\e[0m;\n";
		return -418;
	}
	window = SDL_CreateWindow(
		"GenerateShape",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		std::cerr << "Error: \e48;2;0;0;0m\e[38;2;255;0;0m" << SDL_GetError() << "\e[0m;\n";
		return -404;
	}
	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
	
	SDL_UpdateWindowSurface(window);
	for (int i = 0; i < screenHeight / segmentSize; i++){
		for (int j = 0; j < screenWidth / segmentSize; j++){
			int value =	pow(
				(
					(
						pow(i - (screenHeight / 2), (AppSettings::useSquircle)?(4):(2)) 
					) + (
						pow(j - (screenWidth / 2), (AppSettings::useSquircle)?(4):(2))
					)
				),
				(AppSettings::useSquircle)?(0.25):(0.5)
			);
			if (
				(
					(
						(value < distancesFromCenter[0])
					) or (
						(value > distancesFromCenter[1]) and (value < distancesFromCenter[2])
					) or (
						(value > distancesFromCenter[2]) and (abs((screenWidth / 2) - j) < segmentWidth)
					) or (
						(
							(value > distancesFromCenter[2])
						) and (
							abs(
								(
									abs((screenWidth / 2) - j)
								) - (
									abs((screenHeight / 2) - i) * 0.5
								) 
							) < (
								segmentWidth
							)
						) and (
							i > screenHeight / 2
						)
					)
				) and (
					(
						value < distancesFromCenter[3]
					) or (
						(value < distancesFromCenter[4]) and (i < screenHeight / 2)
					)
				)
			){
					
				SDL_Rect segment {
					j * segmentSize, 
					i * segmentSize, 
					segmentSize,
					segmentSize,
				};
				
				SDL_FillRect(
					screenSurface, 
					&segment,
					SDL_MapRGB(
						SDL_CreateRGBSurface(
							0, 
							segmentSize, 
							segmentSize, 
							32, 0, 
							0, 0, 0
						)->format, 
						/* red */ 255,
						/* green */ 0,
						/* blue */ 0
					)
				);
			}
		}
	}
	while (true){
		SDL_UpdateWindowSurface(window);
		handleEvents(window, true);
	}
	return 0;
}
