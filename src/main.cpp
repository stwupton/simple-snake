#include <cassert>
#include <cmath>

#include <SDL2/SDL.h>

#include "base_types.hpp"
#include "vector.hpp"
#include "array.hpp"
#include "game_state.hpp"
#include "simulator.hpp"
#include "renderer.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define GRID_WIDTH 96
#define GRID_HEIGHT 54

// #define GRID_WIDTH 48
// #define GRID_HEIGHT 27

// #define GRID_WIDTH 32
// #define GRID_HEIGHT 18

#define SIM_TIME_MS (1.0f / 4 * 1000)

int main(int argc, char *arg[]) {
  int result = SDL_Init(SDL_INIT_EVERYTHING);
  if (result != 0) {
    SDL_Log(SDL_GetError());
  }

  int window_flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
  SDL_Window *window = SDL_CreateWindow("Project Snake", 0, 0, 1920, 1080, window_flags);
  assert(window != nullptr);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  if (gl_context == NULL) {
    SDL_Log(SDL_GetError());
  }

  result = SDL_GL_SetSwapInterval(-2);
  if (result != 0) {
    SDL_Log(SDL_GetError());
  }

  Settings settings;
  settings.screen_width = SCREEN_WIDTH;
  settings.screen_height = SCREEN_HEIGHT;
  settings.grid_width = GRID_WIDTH;
  settings.grid_height = GRID_HEIGHT;
  settings.sim_time_ms = SIM_TIME_MS;

  Input input;

  Game_State *previous_state = new Game_State();
  Game_State *state = new Game_State();
  start_game(state, &input, settings);

  Snake &snake = state->snake;

  update_food_position(&state->food_position, settings); 

  bool should_close = false;
  s8 sim_speed = 1;
  u64 previous_time = SDL_GetTicks64();
  u64 time_accumulator = 0;

  while (!should_close) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            should_close = true;
            break;

          case SDLK_LEFT:
          case SDLK_a:
            input.set_direction({ -1, 0 });
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            input.set_direction({ 1, 0 });
            break;
          case SDLK_UP:
          case SDLK_w:
            input.set_direction({ 0, -1 });
            break;
          case SDLK_DOWN:
          case SDLK_s:
            input.set_direction({ 0, 1 });
            break;

          case SDLK_EQUALS:
            sim_speed += 1;
            break;
          case SDLK_MINUS:
            sim_speed -= 1;
            break;
        }
      }
    }

    const uint64_t current_time = SDL_GetTicks64();
    sim_speed = fmax(0, fmin(sim_speed, 3));
    time_accumulator += (current_time - previous_time) * sim_speed;
    previous_time = current_time;

    while (time_accumulator >= SIM_TIME_MS) {
      time_accumulator -= SIM_TIME_MS;
      *previous_state = *state;
      *state = simulate(*state, &input, settings);
    }

    const f32 alpha = time_accumulator / SIM_TIME_MS;
    render(window, *previous_state, *state, alpha, settings);
  }

  SDL_Quit();
  return 0;
}