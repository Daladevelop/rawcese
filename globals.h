/*
 *	Licensed under GNU GPL v2
 *	Author(s): Emil Österlund, Christofer Oden, 
 *	Created on:	09102?
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cstdio>

#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		600
#define SCREEN_BPP			32
#define UPDATES_PER_SECOND	999
#define PANEL_WIDTH			160
#define TILE_SIZE			32

#define NW 1
#define N  2
#define NE 3
#define E  4
#define SE 5
#define S  6
#define SW 7
#define W  8

// Returns a random value between 0.0 and 1.0
#define CHANCE(p) ((rand() / (double)RAND_MAX) < p ? true : false)

enum UnitType {
	UNIT_DRAGON,
	UNIT_INFANTRY,
	UNIT_ARMOR,
	UNIT_HOWITZER,
	UNIT_MGUNNERORC,
	UNIT_BEHOLDER
};

enum BattleOutcome {
	OUTCOME_NONE,
	OUTCOME_VICTORY,
	OUTCOME_DRAW,
	OUTCOME_DEFEAT
};

// Terrain sprites
extern SDL_Rect RECT_GRASS;
extern SDL_Rect RECT_CLIFF_NW;
extern SDL_Rect RECT_CLIFF_N;
extern SDL_Rect RECT_CLIFF_NE;
extern SDL_Rect RECT_CLIFF_W;
extern SDL_Rect RECT_WATER;
extern SDL_Rect RECT_CLIFF_E;
extern SDL_Rect RECT_CLIFF_SW;
extern SDL_Rect RECT_CLIFF_S;
extern SDL_Rect RECT_CLIFF_SE;
extern SDL_Rect RECT_OUTER_CLIFF_SE;
extern SDL_Rect RECT_OUTER_CLIFF_SW;
extern SDL_Rect RECT_OUTER_CLIFF_NE;
extern SDL_Rect RECT_OUTER_CLIFF_NW;
extern SDL_Rect RECT_TREES_NW;
extern SDL_Rect RECT_TREES_N;
extern SDL_Rect RECT_TREES_NE;
extern SDL_Rect RECT_TREES_W;
extern SDL_Rect RECT_TREES;
extern SDL_Rect RECT_TREES_E;
extern SDL_Rect RECT_TREES_SW;
extern SDL_Rect RECT_TREES_S;
extern SDL_Rect RECT_TREES_SE;
// Unit sprites;
extern SDL_Rect RECT_DRAGON;
extern SDL_Rect RECT_INFANTRY;
extern SDL_Rect RECT_ARMOR;
extern SDL_Rect RECT_HOWITZER;
extern SDL_Rect RECT_MGUNNERORC;
extern SDL_Rect RECT_BEHOLDER;
extern SDL_Rect RECT_FACTORY;
// Interface sprites
extern SDL_Rect RECT_MARKER;
extern SDL_Rect RECT_SHROUD;

SDL_Surface *load_image(const char* filename);

TTF_Font *load_font(const char* fontname, int size);

void apply_surface(int x, int y, SDL_Surface* source,
		SDL_Surface* canvas, SDL_Rect* clip = NULL);

SDL_Rect *rectcpy(SDL_Rect *dest, SDL_Rect *source);

void blit_string(int x, int y, const char *str,
		SDL_Surface *dest, SDL_Surface *fontMap);

void put_pixel(int x, int y, Uint32 pixel, SDL_Surface *dest);

#endif
