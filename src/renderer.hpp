#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "game_state.hpp"
#include "settings.hpp"
#include "array.hpp"

#define LERP_SNAKE_POSITION false

// Using dedicated GPU
// https://stackoverflow.com/questions/68469954/how-to-choose-specific-gpu-when-create-opengl-context
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 1;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void draw_rect(
  f32 x, 
  f32 y, 
  f32 width, 
  f32 height, 
  f32 r, 
  f32 g, 
  f32 b,
  const Settings &settings
) {
  glColor3f(r, g, b);

  const f32 width_ratio = 2.f / settings.screen_width;
  const f32 height_ratio = -(2.f / settings.screen_height);

  const f32 x0 = (x * width_ratio) - 1.f;
  const f32 y0 = (y * height_ratio) + 1.f;
  const f32 x1 = ((x + width) * width_ratio) - 1.f;
  const f32 y1 = ((y + height) * height_ratio) + 1.f;

  glBegin(GL_QUADS);
  glVertex2f(x0, y0);
  glVertex2f(x1, y0);
  glVertex2f(x1, y1);
  glVertex2f(x0, y1);
  glEnd();
}

void fill_cell(const Vec2<f32> &cell, const Rgb &colour, const Settings &settings) {
  const u32 cell_width = settings.cell_width();
  const u32 cell_height = settings.cell_height();

  draw_rect(
    cell.x * cell_width + 1, 
    cell.y * cell_height + 1, 
    cell_width - 1, 
    cell_height - 1, 
    colour.r, colour.g, colour.b,
    settings
  );
}

void draw_snake_part(const Vec2<f32> &part, const Settings &settings) {
  fill_cell(part, Rgb(1.f, 0.85f, 0.56f), settings);  
}

void draw_food(const Vec2<f32> &food, const Settings &settings) {
  fill_cell(food, Rgb(0.85f, 0.23f, 0.23f), settings);
}

template<typename T>
T lerp(T a, T b, f32 alpha) {
  alpha = fmin(1.0f, fmax(0.0f, alpha));
  return a * (1.0f - alpha) + b * alpha;
}

void render(
  SDL_Window *window, 
  const Game_State &previous_state, 
  const Game_State &state, 
  f32 alpha,
  const Settings &settings
) {
  glViewport(0, 0, settings.screen_width, settings.screen_height);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  draw_rect(0, 0, 1920, 1080, 0.f, 0.6f, 0.74f, settings);
  for (size_t i = 0; i < state.snake.positions.length; i++) {
#if LERP_SNAKE_POSITION
    const Vec2<f32> previous_position = previous_state.snake.positions[i];
    const Vec2<f32> current_position = state.snake.positions[i];
    const Vec2<f32> part = lerp(previous_position, current_position, alpha); 
#else 
    const Vec2<f32> part = state.snake.positions[i];
#endif
    draw_snake_part(part, settings);
  }

  draw_food(state.food_position, settings);

  SDL_GL_SwapWindow(window);
}