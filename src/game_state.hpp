#pragma once 

#include "snake.hpp"
#include "input.hpp"

struct Game_State {
  Snake snake;
  Vec2<f32> food_position;
};