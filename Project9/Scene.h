#pragma once
#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
using namespace std;

#define width 800
#define heigh 600
#define SnowNum 100
#define StarNum 10

class Spot {
public:
	Spot() {
		x = 0;
		y = 0;
		r = 0;
		speed = 0;
		alpha = 0;
	}
	int x;
	int y;
	int r;
	int speed;
	int alpha;
	void moveSpot(Spot * spot, int mode) {
		if (mode == 1) {
			spot->y = spot->y + spot->speed;
			if (spot->y >= 600) spot->y = 0;
		}
		else if(mode == 0){
			spot->x = spot->x + spot->speed;
			if (spot->x >= 690) spot->x = 310;
		}
		else if (mode == 2) {
			spot->y = spot->y - spot->speed;
			if (spot->y <= 110) spot->y = 480;
		}
	}
	void drawSpot(Spot *spot, SDL_Renderer *rend, SDL_Texture * circle) {
		SDL_Rect rect;
		rect.x = spot->x;
		rect.y = spot->y;
		rect.w = spot->r;
		rect.h = spot->r;
		SDL_SetTextureAlphaMod(circle, spot->alpha);
		SDL_RenderCopy(rend, circle, NULL, &rect);
	}
};

class Star {
public:
	Star() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
		vspeed = 0;
		unvspeed = 0;
		alpha = 255;
		visual = true;
	}
	int x, y, w, h;
	int alpha, vspeed, unvspeed;
	bool visual;
	void flashing(Star * star) {
		if (star->visual) {
			star->alpha -= star->unvspeed;
			if (star->alpha <= 0) star->visual = false;
		}
		else {
			star->alpha += star->vspeed;
			if (star->alpha >= 255) star->visual = true;
		}
	}

	void drawStar(Star *Bstar, SDL_Renderer *rend, SDL_Texture * star) {
		SDL_Rect rect;
		rect.x = Bstar->x;
		rect.y = Bstar->y;
		rect.w = Bstar->w;
		rect.h = Bstar->h;
		SDL_SetTextureAlphaMod(star, Bstar->alpha);
		SDL_RenderCopy(rend, star, NULL, &rect);
	}
};