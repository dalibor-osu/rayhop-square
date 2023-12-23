#include "drawables.h"

void DrawSquarePlayer(SquarePlayer* square_player) {
	DrawRectangle(square_player->position.x, square_player->position.y, square_player->size, square_player->size, square_player->color);
}

void DrawVerticalLineEx(VerticalLine* line) {
	float y = line->position.y + line->thickness / 2;
	Vector2 start_position = (Vector2){line->position.x - line->length / 2, y};
	Vector2 end_position = (Vector2){line->position.x + line->length / 2, y};
	DrawLineEx(start_position, end_position, line->thickness, line->color);
}

void MovePlayer(SquarePlayer* player) {
	player->previous_position = player->position;
	player->position.x += player->speed.x;
	player->position.y += player->speed.y;

	if (player->is_grounded == FALSE) {
		player->air_time += GetFrameTime();
		player->speed.y += GRAVITY_POWER * player->air_time;
	}
}