#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>

const int screenWidth = 750;
const int screenHeight = 750;
const int rectSize = 15;
const int arraySize = 50;
int arrOrig[arraySize];
int arr[arraySize];
bool inOrder = false;
int timeDelay = 5;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


void close()
{
	//Destroy renderer and window, then quit
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}


void init()
{
	//Creates the window
	window = SDL_CreateWindow("Collier's C++ Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "There was error initiailizing the window!" << std::endl;
		std::cout << "SDL Error Code: " << SDL_GetError() << std::endl;
		close();
	}
	else
	{
		//Creates the renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			std::cout << "There was error initiailizing the renderer!" << std::endl;
			std::cout << "SDL Error Code: " << SDL_GetError() << std::endl;
			close();
		}
	}
}


void consoleHelp()
{
	//Help prompt for console
	std::cout << "\nControls for Program:" << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "Press 'H' to access this help information." << std::endl;
	std::cout << "Press 'Q' to quit the program." << std::endl;
	std::cout << "Press '0' to generate new array of data." << std::endl;
	std::cout << "Press '1' to run Selection Sort." << std::endl;
	std::cout << "Press '2' to run Bubble Sort." << std::endl;
	std::cout << "Press 'Left Arrow Key' to increase sorting speed." << std::endl;
	std::cout << "Press 'Right Arrow Key' to decrease sorting speed.\n" << std::endl;
}


void createArray()
{
	//Creates a random set of data for the original array
	std::random_device rd;
	std::uniform_int_distribution<> d(1, screenHeight);

	for (int i = 0; i < arraySize; i++)
	{
		arrOrig[i] = d(rd);
	}

	std::cout << "Loaded a new array of data." << std::endl;
}


void loadArray()
{
	//Makes copy of original array and loads this copy
	std::copy(std::begin(arrOrig), std::end(arrOrig), std::begin(arr));
}


void visualize(int x = -1, int y = -1, int z = -1)
{
	//Clears the window to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white
	SDL_RenderClear(renderer);

	int j = 0;
	for (int i = 0; i <= screenWidth - rectSize; i += rectSize)
	{
		SDL_PumpEvents(); //prevents idling window if you move window during execution

		//System to visualize algorithm sorting.
		//Each loop checks for x,y,z passed into function to visualize colored stack.
		//Draws rectangles for each stack for given color.
		SDL_Rect rect = { i, (screenHeight - arr[j]), rectSize, arr[j]};
		bool complete = false;
		
		if (inOrder)
		{
			SDL_SetRenderDrawColor(renderer, 76, 206, 84, 0); //GREEN
		}
		else if (j == x)
		{
			SDL_SetRenderDrawColor(renderer, 76, 206, 84, 0); //GREEN
		}
		else if (j == y)
		{
			SDL_SetRenderDrawColor(renderer, 208, 60, 60, 0); //RED
		}
		else if (j == z)
		{
			SDL_SetRenderDrawColor(renderer, 50, 70, 240, 0); //BLUE
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //black
		}
		SDL_RenderFillRect(renderer, &rect);
		
		//Draws white border around stacks to show visual separation
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white
		SDL_RenderDrawRect(renderer, &rect);
		j++;
	}
	SDL_RenderPresent(renderer);
}


void selectionSort()
{
	std::cout << "Running 'Selection Sort' algorithm..." << std::endl;
	int minSlot;
	for (unsigned int i = 0; i < arraySize; i++)
	{
		minSlot = i;
		for (unsigned int j = i; j < arraySize; j++)
		{
			
			if (arr[j] < arr[minSlot])
			{
				minSlot = j;
			}
			visualize(i, j, minSlot); //(green, red, blue)
			SDL_Delay(timeDelay);
		}
		std::swap(arr[minSlot], arr[i]);
	}
	std::cout << "Finished sorting." << std::endl;
}


void bubbleSort()
{
	std::cout << "Running 'Bubble Sort' algorithm..." << std::endl;
	for (unsigned int i = 0; i < arraySize-1; i++)
	{
		for (unsigned int j = 0; j < arraySize-1-i; j++)
		{
			if (arr[j] > arr[j+1])
			{
				std::swap(arr[j], arr[j + 1]);
			}
			visualize(j, j+1); //(green, red, blue)
			SDL_Delay(timeDelay);
		}
	}
	std::cout << "Finished sorting." << std::endl;
}


void execute()
{
	createArray();
	loadArray();
	visualize();
	SDL_Event event;
	bool running = true;
	
	
	while (running)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_q:
					running = false;
					break;

				case SDLK_h:
					consoleHelp();
					break;
				
				case SDLK_0:
					createArray();
					loadArray();
					inOrder = false;
					visualize();
					break;

				case SDLK_1:
					loadArray();
					inOrder = false;
					selectionSort();
					inOrder = true;
					visualize();
					break;

				case SDLK_2:
					loadArray();
					inOrder = false;
					bubbleSort();
					inOrder = true;
					visualize();
					break;

				case SDLK_RIGHT:
					if (timeDelay < 20)
					{
						timeDelay += 1;
					}
					std::cout << "Time Delay: " << timeDelay << " ms" << std::endl;
					break;
				
				case SDLK_LEFT:
					if (timeDelay > 1)
					{
						timeDelay -= 1;
					}
					std::cout << "Time Delay: " << timeDelay << " ms" << std::endl;
					break;
				}
			}
		}
	}
}


int main()
{
	init();
	consoleHelp();
	execute();
	close();
}
