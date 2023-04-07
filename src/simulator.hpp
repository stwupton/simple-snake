#pragma once 

#include "game_state.hpp"
#include "settings.hpp"

void correct_grid_position(Vec2<s32> *position, const Settings &settings) {
  while (position->x < 0) position->x += settings.grid_width;
  while (position->y < 0) position->y += settings.grid_height;

  position->x = (position->x % settings.grid_width);
  position->y = (position->y % settings.grid_height);
}

void start_game(Game_State *state, const Settings &settings) {
  state->input.reset();

  state->snake.reset();
  state->snake.positions.push(Vec2<s32>(settings.grid_width / 2, settings.grid_height / 2));
}

void update_food_position(Vec2<s32> *position, const Settings &settings) {
  position->x = rand() % settings.grid_width;
  position->y = rand() % settings.grid_height;
}

void simulate(Game_State *state, const Settings &settings) {
  Snake &snake = state->snake;

  if (snake.positions.length == 0) {
    start_game(state, settings);
    return;
  }

  if (snake.dead) {
    snake.positions.pop();
    return;
  }

  snake.direction = state->input.consume_direction();
  
  const bool direction_is_allowed = 
    snake.direction.x != -snake.previous_direction.x || 
    snake.direction.y != -snake.previous_direction.y;

  if (snake.direction.magnitude() > 0 && direction_is_allowed) {
    snake.previous_direction = snake.direction;

    Vec2<s32> updated_head_position = snake.positions.data[0] + snake.direction;
    correct_grid_position(&updated_head_position, settings);

    for (size_t i = snake.positions.length - 1; i > 0; i--) {
      if (updated_head_position == snake.positions.data[i]) {
        snake.dead = true;
        return;
      }
    }

    if (updated_head_position == state->food_position) {
      update_food_position(&state->food_position, settings);

      // It doesn't matter where we place the new snake part as it's position 
      // gets updated correctly in the next loop
      snake.positions.push({});
    }

    for (size_t i = snake.positions.length - 1; i > 0; i--) {
      snake.positions.data[i] = snake.positions.data[i - 1];
    }

    snake.positions.data[0] = updated_head_position;
  }
}