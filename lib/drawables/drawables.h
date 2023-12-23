#ifndef DRAWABLES_H

#define DRAWABLES_H
#include "raylib.h"
#include "../constants.h"

typedef struct VerticalLine {
	Vector2 position;
	float length;
	float thickness;
	Color color;
} VerticalLine;

typedef struct SquarePlayer {
	int size;
	Color color;
	Vector2 position;
	Vector2 previous_position;
	Vector2 speed;
	int is_grounded;
	int number_of_jumps;
	float air_time;
} SquarePlayer;

void DrawSquarePlayer(SquarePlayer* square_player);
void DrawVerticalLineEx(VerticalLine* line);
void MovePlayer(SquarePlayer* player);

#endif