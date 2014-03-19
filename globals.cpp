/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Emil Österlund, Christofer Oden
 *	Created on:	09102?
 */

#include "globals.h"

#include <SDL/SDL.h>

// Terrain sprites
SDL_Rect RECT_GRASS             = {  48, 128, 32, 32 };
SDL_Rect RECT_CLIFF_NW          = {   0,  96, 16, 16 };
SDL_Rect RECT_CLIFF_N           = {  16,  96, 16, 16 };
SDL_Rect RECT_CLIFF_NE          = {  32,  96, 16, 16 };
SDL_Rect RECT_CLIFF_W           = {   0, 112, 16, 16 };
SDL_Rect RECT_WATER             = {  16, 112, 16, 16 };
SDL_Rect RECT_CLIFF_E           = {  32, 112, 16, 16 };
SDL_Rect RECT_CLIFF_SW          = {   0, 128, 16, 16 };
SDL_Rect RECT_CLIFF_S           = {  16, 128, 16, 16 };
SDL_Rect RECT_CLIFF_SE          = {  32, 128, 16, 16 };
SDL_Rect RECT_OUTER_CLIFF_SE    = {  48,  96, 16, 16 };
SDL_Rect RECT_OUTER_CLIFF_SW    = {  64,  96, 16, 16 };
SDL_Rect RECT_OUTER_CLIFF_NE    = {  48, 112, 16, 16 };
SDL_Rect RECT_OUTER_CLIFF_NW    = {  64, 112, 16, 16 };
SDL_Rect RECT_TREES_NW          = { 112,  96, 16, 16 };
SDL_Rect RECT_TREES_N           = { 128,  96, 16, 16 };
SDL_Rect RECT_TREES_NE          = { 144,  96, 16, 16 };
SDL_Rect RECT_TREES_W           = { 112, 112, 16, 16 };
SDL_Rect RECT_TREES             = { 128, 112, 16, 16 };
SDL_Rect RECT_TREES_E           = { 144, 112, 16, 16 };
SDL_Rect RECT_TREES_SW          = { 112, 128, 16, 16 };
SDL_Rect RECT_TREES_S           = { 128, 128, 16, 16 };
SDL_Rect RECT_TREES_SE          = { 144, 128, 16, 16 };
// Unit sprites
SDL_Rect RECT_DRAGON            = {   0,   0, 32, 32 };
SDL_Rect RECT_INFANTRY          = {  32,   0, 32, 32 };
SDL_Rect RECT_ARMOR             = {  64,   0, 32, 32 };
SDL_Rect RECT_HOWITZER          = {  96,   0, 32, 32 };
SDL_Rect RECT_MGUNNERORC        = { 128,   0, 32, 32 };
SDL_Rect RECT_BEHOLDER			= { 192,  64, 32, 32 };
SDL_Rect RECT_FACTORY           = { 160,   0, 32, 32 };
// Interface sprites
SDL_Rect RECT_MARKER            = { 192,   0, 32, 32 };
SDL_Rect RECT_SHROUD            = { 192,  32, 32, 32 };

SDL_Surface *load_image(const char* filename)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load(filename);
    if (loadedImage == NULL) {
        printf("IMG_Load: %s\n", SDL_GetError());
        return NULL;
    }

    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
    if (optimizedImage == NULL) {
        printf("SDL_DisplayFormat: %s\n", SDL_GetError());
        return NULL;
    }

    if (SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
            SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF)) == -1) {
        printf("SDL_SetColorKey: %s:", SDL_GetError());
    }

    return optimizedImage;
}

TTF_Font *load_font(const char* path, int size)
{
    TTF_Font *font = NULL;

    font = TTF_OpenFont(path, size);
    if(font == NULL) {
        printf("TTF_OpenFont: %s", TTF_GetError());
    }

    return font;
}

void apply_surface(int x, int y, SDL_Surface* source,
        SDL_Surface* canvas, SDL_Rect* clip)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    if (SDL_BlitSurface(source, clip, canvas, &offset) == -1) {
        printf("SDL_BlitSurface: %s\n", SDL_GetError());
    }
}

SDL_Rect *rectcpy(SDL_Rect *dest, SDL_Rect *source) {

    dest->x = source->x;
    dest->y = source->y;
    dest->w = source->w;
    dest->h = source->h;

    return dest;
}

void blit_string( int x, int y, const char *str,
        SDL_Surface *dest, SDL_Surface *font_map )
{
    const int FONT_WIDTH  = 5;
    const int FONT_HEIGHT = 6;

    char *p = const_cast<char *>( str );
	int i = 0;

	while( *p != '\0' ) {
        SDL_Rect rect = { 0, 0, FONT_WIDTH, FONT_HEIGHT };

        // Convert ASCII value to offset
        if (isalpha(*p)) {
            int n = int(toupper(*p) - 'A');
            rect.x += n * FONT_WIDTH; 
        }
        else if (isdigit(*p)) {
            int n = int((*p) - '0');
            rect.x += n * FONT_WIDTH;
            rect.y += FONT_HEIGHT;
        }
        else if (*p >= '!' && *p <= '/') {
            int n = int((*p) - '!');
            rect.x += n * FONT_WIDTH;
            rect.y += (2 * FONT_HEIGHT);
        }
        else if (*p >= ':' && *p <= '@') {
            int n = int((*p) - ':');
            rect.x += n * FONT_WIDTH;
            rect.y += (3 * FONT_HEIGHT);
        }
        else if (*p >= '[' && *p <= '_') {
            int n = int((*p) - '[');
            rect.x += n * FONT_WIDTH;
            rect.y += (4 * FONT_HEIGHT);
        }
        else if( *p == '\n' ) {
			i = 0;
			y += FONT_HEIGHT;
			++p;
			continue;
		}
		else {
            rect.x = 25;
            rect.y = 24;
        }

        apply_surface(x + i * FONT_WIDTH, y, font_map, dest, &rect);
		++i; ++p;
    }
}

void put_pixel( int x, int y, Uint32 pixel, SDL_Surface *surface ) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ y * surface->w + x ] = pixel;
}
