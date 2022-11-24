#include "Util.h"

void draw_circle(SDL_Renderer* surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		set_pixel(surface, (int)(cx + x), (int)(cy + y), r, g, b, a);
		set_pixel(surface, (int)(cx + y), (int)(cy + x), r, g, b, a);

		if (x != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy + y), r, g, b, a);
			set_pixel(surface, (int)(cx + y), (int)(cy - x), r, g, b, a);
		}

		if (y != 0)
		{
			set_pixel(surface, (int)(cx + x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy + x), r, g, b, a);
		}

		if (x != 0 && y != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy - x), r, g, b, a);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

void fill_circle(SDL_Renderer* surface, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// Note that there is more to altering the bitrate of this 
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html
	static const int BPP = 4;

	//double ra = (double)radius;

	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(surface, r, g, b, a);
		SDL_RenderDrawLine(surface, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(surface, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
}

double get_distance(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int get_distance_sqr(int x1, int y1, int x2, int y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

void set_pixel(SDL_Renderer* rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(rend, r, g, b, a);
	SDL_RenderDrawPoint(rend, x, y);
}

void set_pixel(SDL_Renderer* rend, int x, int y, RGBA col) 
{
	set_pixel(rend, x, y, col.r, col.g, col.b, col.a);
}

void put_pixel_on_surface(SDL_Surface* surface, int x, int y, RGBA color)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	pixel[3] = color.r;
	pixel[2] = color.g;
	pixel[1] = color.b;
	pixel[0] = color.a;
}

void put_pixel_on_surface(SDL_Surface* surface, int ind, RGBA color)
{
	Uint8* pixel = (Uint8*)surface->pixels + ind;
	pixel[3] = color.r;
	pixel[2] = color.g;
	pixel[1] = color.b;
	pixel[0] = color.a;
}
