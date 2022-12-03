#include "BaseSDL.h"

MyApp::MyApp(std::string windowName, int width, int height)
{

	mScreenWidth = width;
	mScreenHeight = height;
	mTitle = windowName;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		IsRunning = false;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			printf("Warning: Linear texture filtering not enabled!");

		mWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN);
		if (mWindow == nullptr) {
			printf("Windows could not be created! SDL Error: %s\n", SDL_GetError());
			IsRunning = false;
		}
		else {


			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				IsRunning = false;
			}
		}
	}
}

MyApp::~MyApp()
{
	SDL_FreeSurface(mSurfaceBuffer);
	SDL_DestroyTexture(mBuffer);
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

void MyApp::Run()
{
	mSurfaceBuffer = SDL_CreateRGBSurface(0, mScreenWidth, mScreenHeight, 32, 0, 0, 0, 0);
	mPoints.reserve(INITIAL_POINT_COUNT);
	mPointsRGBA.reserve(INITIAL_POINT_COUNT);
	srand(time(0));


	for (int i = 0; i < INITIAL_POINT_COUNT; i++) {
		AddPointWithRandomColor(rand() % mScreenWidth, rand() % mScreenHeight);
	}

	mBuffer = SDL_CreateTexture(mRenderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		mScreenWidth,
		mScreenHeight);


	while (IsRunning) {
		mFpsTimer.start();

		PollEvents();
		UpdateBuffer();
		DrawFromBuffer();
		//Draw();

		float FPS = 1000.f / mFpsTimer.getTicks();
		std::string newTitle = mTitle + ": " + std::to_string(FPS);
		SDL_SetWindowTitle(mWindow, newTitle.c_str());
	}
}


/*
Draws pixels directly into renderer
even less eficient
*/
void MyApp::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	for (int y = 0; y < mScreenHeight; y++) {
		for (int x = 0; x < mScreenWidth; x++) {

			int minDistInd = INT_MAX;
			int minDist = INT_MAX;

			for (int i = 0; i < mPoints.size(); i++) {
				int dist = get_distance_sqr(x, y, mPoints[i].x, mPoints[i].y);

				if (dist < minDist) {
					minDist = dist;
					minDistInd = i;
				}
			}

			set_pixel(mRenderer, x, y, mPointsRGBA[minDistInd]);
		}
	}


	for (auto point : mPoints) {
		fill_circle(mRenderer, point.x, point.y, POINT_SIZE, 255, 255, 255, 255);
	}

	SDL_RenderPresent(mRenderer);
}

void MyApp::UpdateBuffer()
{
	bool render = mPoints.size() > 0;
	SDL_LockTexture(mBuffer, nullptr, &mSurfaceBuffer->pixels, &mSurfaceBuffer->pitch);

	for (int y = 0; y < mScreenHeight; y++) {
		for (int x = 0; x < mScreenWidth; x++) {
			if (render)
			{
				int minDistInd = INT_MAX;
				int minDist = INT_MAX;

				for (int i = 0; i < mPoints.size(); i++) {
					auto& point = mPoints[i];
					int dist = get_distance_sqr(x, y, point.x, point.y);

					if (dist < minDist) {
						minDist = dist;
						minDistInd = i;
					}
				}
				put_pixel_on_surface(mSurfaceBuffer, x, y, mPointsRGBA[minDistInd]);
			}
			else
			{
				put_pixel_on_surface(mSurfaceBuffer, x, y, DEFAULT_BACKGROUND_COLOR);
			}
		}
	}

	SDL_UnlockTexture(mBuffer);
}

void MyApp::DrawFromBuffer()
{
	SDL_Rect screen = { 0,0,mScreenWidth, mScreenHeight };
	SDL_RenderCopy(mRenderer, mBuffer, &screen, &screen);

	for (auto point : mPoints) {
		fill_circle(mRenderer, point.x, point.y, POINT_SIZE, 255, 255, 255, 255);
	}
	SDL_RenderPresent(mRenderer);
}

void MyApp::PollEvents()
{
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type)
		{
		case SDL_MOUSEMOTION:
			HandleMouseMoution(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			HandleMouseButtonClick(e);
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:
			HandleKeyboardInput(e);
			break;

		case SDL_QUIT:
			IsRunning = false;
			break;
		}
	}
}

void MyApp::HandleKeyboardInput(SDL_Event e)
{
	bool pressed = (e.type == SDL_KEYDOWN);

	switch (e.key.keysym.sym) {
	case SDLK_LSHIFT:
	case SDLK_SPACE:
		mMovePoint = pressed;
		break;
	}
}

void MyApp::HandleMouseMoution(SDL_Event e)
{
	if (mMovePoint && mSelectedPoint > -1) {
		int x = e.motion.x;
		int y = e.motion.y;

		mPoints[mSelectedPoint].x = x;
		mPoints[mSelectedPoint].y = y;
	}


}

int MyApp::GetPointFromCoords(int x, int y)
{
	int ind = -1;
	for (int i = 0; i < mPoints.size(); i++) {
		int dist = get_distance_sqr(x, y, mPoints[i].x, mPoints[i].y);

		if ((dist < POINT_SIZE * POINT_SIZE)) {
			ind = i;
		}
	}
	return ind;
}

void MyApp::AddPointWithRandomColor(int x, int y)
{
	mPoints.push_back(Point(x, y));
	mPointsRGBA.push_back(RGBA(rand() % 256, rand() % 256, rand() % 256, 255));
}

void MyApp::ErasePoint(int index)
{
	mPoints.erase(mPoints.begin() + index);
	mPointsRGBA.erase(mPointsRGBA.begin() + index);
}

//TODO: split this cringe case
void MyApp::HandleMouseButtonClick(SDL_Event e)
{
	int minDistInd = INT_MAX;
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			minDistInd = GetPointFromCoords(e.button.x, e.button.y);

			if (minDistInd > -1) {
				if (mMovePoint)
					mSelectedPoint = minDistInd;
				else
					ErasePoint(minDistInd);
			}
			else
			{
				AddPointWithRandomColor(e.button.x, e.button.y);
			}
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			mSelectedPoint = -1;
			break;
		}
		break;
	}
}

