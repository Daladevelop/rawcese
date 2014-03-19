/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Adam Nyberg, Emil Österlund, Christofer Oden
 *	Created on:	09102?
 */

#include "World.h"

void World::generate(int row_count, int col_count, TileType default_tile)
{
    my_row_count = row_count;
    my_col_count = col_count;
    my_default_tile = default_tile;

    // When setting values at tiles, use 'set_tile()', it has bounds checking
    printf("Initializing terrain matrix\n");
	for( int i = 0; i < my_row_count; ++i ) {
		for( int j = 0; j < my_col_count; ++j ) {
			my_tiles.push_back( Tile( i, j, my_default_tile ) );
		}
	}

    printf("Randomizing landscape\n");
    // Generate a random landscape
    // Instead of putting water at the borders, we refrain from putting
    //  anything else there
    for (int row = 1; row < my_row_count - 1; ++row) {
        for (int col = 1; col < my_col_count - 1; ++col) {
            if      (CHANCE(0.20)) set_tile_type(row, col, TILE_GRASS);
            else if (CHANCE(0.70)) set_tile_type(row, col, TILE_TREES);
        }
    }

    // If a land tile is neighbouring a water tile, replace it with water
    //  with the probability pWater
    // Probability for replacement decreases with every successful attempt
    //  and increases for every failed attempt
    printf("Expanding water areas\n");
    double pWater = 0.0;
    for (int row = 1; row < my_row_count - 1; ++row) {
        for (int col = 1; col < my_col_count - 1; ++col) {

            if (get_tile_type(row, col) == TILE_WATER)
                continue;

            if (
                    get_tile_type(row - 1, col) == TILE_WATER ||
                    get_tile_type(row, col - 1) == TILE_WATER ||
                    get_tile_type(row + 1, col) == TILE_WATER ||
                    get_tile_type(row, col + 1) == TILE_WATER
               ) {

                if (CHANCE(pWater)) {
                    set_tile_type(row, col, TILE_WATER);
                    pWater -= 1.0;
                }
                else {
                    pWater += 0.0;
                }
            }
        }
    }

    // Put water at single land tiles surrounded by water or
    //  land tiles lying between two water tiles horizontally or
    //  vertically to create lakes and rivers
    printf( "Joining water tiles close to each other\n" );
    for( int row = 1; row < my_row_count - 1; ++row ) {
        for( int col = 1; col < my_col_count - 1; ++col ) {
            if( get_tile_type(row, col) == TILE_WATER )
                continue;

            if( (get_tile_type(row + 1, col) == TILE_WATER &&
                    get_tile_type(row - 1, col) == TILE_WATER) ||
                    (get_tile_type(row, col + 1) == TILE_WATER &&
                    get_tile_type(row, col - 1) == TILE_WATER) )
			{
                set_tile_type( row, col, TILE_WATER );
			}
        }
    }

    printf( "Removing single water tiles\n" );
    for( int row = 1; row < my_row_count - 1; ++row ) {
        for( int col = 1; col < my_col_count - 1; ++col ) {
            if( get_tile_type(row, col) == TILE_GRASS )
                continue;

            if( get_tile_type(row + 1, col) != TILE_WATER
                    && get_tile_type(row - 1, col) != TILE_WATER
                    && get_tile_type(row, col + 1) != TILE_WATER
                    && get_tile_type(row, col - 1) != TILE_WATER )
			{
                set_tile_type(row, col, TILE_GRASS);
			}
        }
    }
}

void World::load( const char *path ) {
    FILE *fp = NULL;
    char c = 0;

    my_row_count = 0;
    my_col_count = 0;

    fp = fopen( path, "r" );
    if( fp == NULL ) {
        perror( "fopen" );
        return;
    }

    while( c != EOF ) {
        int col = 0;
        while( (c = fgetc(fp) ) != '\n' ) {
            if( c == EOF )
                break;

            my_tiles.push_back( Tile( my_row_count, col, TileType(c) ) );
            ++col;
        }

        ++my_row_count;
        if( col > my_col_count )
            my_col_count = col;
    }

    fclose( fp );
}

inline
bool World::is_out_of_bounds( int row, int col ) {
    if (row < 0 || row >= my_row_count || col < 0 || col >= my_col_count)
        return true;
    else
        return false;
}

inline
TileType World::get_tile_type( int row, int col )
{
    if( is_out_of_bounds( row, col ) ) {
		//fprintf(stderr, "World tile out of bounds at (%d, %d)\n", row, col);
        return my_default_tile;
	}
    else
        return my_tiles[ row * my_col_count + col ].get_type();
}

inline
void World::set_tile_type( int row, int col, TileType tile_type )
{
    // If out of bounds do nothing
    if( is_out_of_bounds( row, col ) )
		return;
	else
        my_tiles[ row * my_col_count + col ].set_type( tile_type );
}
