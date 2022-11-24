#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <vector>
#include "Point.h"
#include "Util.h"
#include "LTimer.h"

constexpr int POINT_SIZE = 10;
constexpr int INITIAL_POINT_COUNT = 10;
//мое приложение
class MyApp
{
public:
	MyApp(std::string windowName, int width, int height);
	~MyApp();
	void Run();


private:

	void Draw();
	void UpdateBuffer();
	void DrawFromBuffer();
	void PollEvents();
	void HandleMousseButtonClick(SDL_Event e);

	bool IsRunning = true;
	std::string mTitle;
	LTimer mFpsTimer;
	LTimer mCapTimer;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Texture* mBuffer = nullptr;
	SDL_Surface* mSurfaceBuffer = nullptr;

	SDL_Event e;

	int mScreenWidth;
	int mScreenHeight;
	int mBlockSize;
	
	std::vector<Point> mPoints;
	std::vector<RGBA> mPointsRGBA;
	
};

