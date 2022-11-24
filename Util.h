#pragma once

#include <SDL.h>

struct RGBA {
	RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Uint8 r, g, b, a = 0;
};

void set_pixel(SDL_Renderer* rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void set_pixel(SDL_Renderer* rend, int x, int y, RGBA col);


void draw_circle(SDL_Renderer* surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void fill_circle(SDL_Renderer* surface, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

double get_distance(int x1, int y1, int x2, int y2);
int get_distance_sqr(int x1, int y1, int x2, int y2);

void put_pixel_on_surface(SDL_Surface* surface, int x, int y, RGBA color);
void put_pixel_on_surface(SDL_Surface* surface, int ind, RGBA color);