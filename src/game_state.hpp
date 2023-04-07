#pragma once 

#include "snake.hpp"
#include "input.hpp"

struct Game_State {
  Snake snake;
  Input input;
  Vec2<s32> food_position;
};