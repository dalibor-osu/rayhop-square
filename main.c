#include "raylib.h"
#include "lib/drawables/drawables.h"
#include <stdio.h>

int window_width;
int window_height;

VerticalLine ground_line;
VerticalLine lines[10] = {0};
SquarePlayer player;

void HandleInput(void);
void CheckBorders(void);
void InitializeGame(void);
void ResetPlayerFall(SquarePlayer* player, Vector2* position);
void CheckPlayerLinesCollisions(SquarePlayer* player, VerticalLine* lines, int number_of_elements);
void RegeneratePlatforms(void);
int PlayerCollidesWithVerticalLine(SquarePlayer* player, VerticalLine* line);

int main(void) {
	InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_NAME);
	SetTargetFPS(TARGET_FPS);

	InitializeGame();

	while (!WindowShouldClose()) {
		BeginDrawing();
        	ClearBackground(BLACK);

			HandleInput();
			MovePlayer(&player);
			CheckBorders();
			CheckPlayerLinesCollisions(&player, lines, 10);
	
			DrawSquarePlayer(&player);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void CheckPlayerLinesCollisions(SquarePlayer* player, VerticalLine* line_array, int number_of_elements) {
	for (int i = 0; i < number_of_elements ; i++) {
		if (line_array[i].length <= 0) {
			continue;
		}

		VerticalLine current_line = line_array[i];
		DrawVerticalLineEx(&current_line);				

		if (PlayerCollidesWithVerticalLine(player, &current_line) == FALSE) {
			if (player->position.y < window_height - player->size) {
				player->is_grounded = FALSE;
			}
		}
		else {
			Vector2 pos = (Vector2) {player->position.x, current_line.position.y - player->size};
			ResetPlayerFall(player, &pos);
		}
	} 
}

int PlayerCollidesWithVerticalLine(SquarePlayer* player, VerticalLine* line) {
	float player_bottom = player->position.y + player->size;
	float player_previous_bottom = player->previous_position.y + player->size;
	
	if (player_bottom < line->position.y || player_previous_bottom > line->position.y) {
		return FALSE;
	}

	float line_left = line->position.x - line->length / 2;
	float line_right = line->position.x + line->length / 2;

	float player_left = player->position.x;
	float player_right = player->position.x + player->size;

	if (player_left < line_left && player_right < line_left) {
		return FALSE;
	}

	if (player_right > line_right && player_left > line_right) {
		return FALSE;
	}

	return TRUE;
}

void ResetPlayerFall(SquarePlayer* player, Vector2* position) {
	player->is_grounded = TRUE;
	player->position = *position;
	player->speed.y = 0.0f;
	player->number_of_jumps = 0;
	player->air_time = 0.0f;
}

void RegeneratePlatforms(void) {
	for (int i = 1; i < 10; i++) {
		int length = window_width / 9;
		int x = GetRandomValue(length / 2, window_width - length / 2);
		int y = GetRandomValue(player.size * 2, window_height - player.size * 3);

		lines[i].position = (Vector2) {x, y};
		lines[i].length = length;
		lines[i].thickness = 5.0f;
		lines[i].color = RAYWHITE;
	}
}

void CheckBorders(void) {
	if (player.position.x < 0) {
		player.position.x = 0;
	}

	if (player.position.y < 0) {
		player.position.y = 0;
		player.speed.y = 0;
	}

	if (player.position.x + player.size > window_width) {
		player.position.x = window_width - player.size;
	}

	if (player.position.y + player.size >= window_height) {
		Vector2 pos = (Vector2) {player.position.x, window_height - player.size};
		ResetPlayerFall(&player, &pos);
	}
}

void HandleInput(void) {
	int delta_time = (int)(GetFrameTime() * 1000);

	if (IsKeyDown(KEY_LEFT)) {
		player.speed.x = -SPEED * delta_time;
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		player.speed.x = SPEED * delta_time;
	}
	else {
		player.speed.x = 0;
	}

	if (IsKeyPressed(KEY_R)) {
		RegeneratePlatforms();
	}

	if (IsKeyPressed(KEY_UP)) {
		if (player.number_of_jumps == 0) {
			player.speed.y = -SPEED * delta_time * JUMP_POWER;
			player.is_grounded = FALSE;
			player.air_time = 0;
		}
		else if (player.number_of_jumps == 1) {	
			player.speed.y = -SPEED * delta_time * JUMP_POWER / 2;
			player.is_grounded = FALSE;
			player.air_time = 0;
		}
		else {
			return;
		}

		player.number_of_jumps++;
	}
}

void InitializeGame(void) {
	int current_monitor = GetCurrentMonitor();
	int monitor_width = GetMonitorWidth(current_monitor);
	int monitor_height = GetMonitorHeight(current_monitor);

	window_width = (int)(monitor_width * 0.8f);
	window_height = (int)(monitor_height * 0.8f);

	SetWindowSize(window_width, window_height);
	SetWindowPosition(monitor_width / 2 - window_width / 2, monitor_height / 2 - window_height / 2);

	int player_size;
	if (window_height < window_width) {
		player_size = window_height / 10;
	}
	else {
		player_size = window_width / 10;
	}

	RegeneratePlatforms();

	lines[0].position = (Vector2) {window_width / 2, window_height - player_size * 2};
	lines[0].length = window_width / 2;
	lines[0].thickness = 5.0f;
	lines[0].color = RAYWHITE;

	player.size = player_size;
	player.color = PLAYER_COLOR;
	player.position = (Vector2) {window_width / 2, window_height - player.size};
	player.previous_position = player.position;
	player.speed = (Vector2) {0, 0};
	player.is_grounded = TRUE;
	player.air_time = 0.0f;
	player.number_of_jumps = 0;
}