#pragma once

#include "vector.hpp"
#include "array.hpp"

struct Snake {
  bool dead = false;
  Vec2<f32> direction;
  Vec2<f32> previous_direction;
  Array<Vec2<f32>, 256> positions;

  void reset() {
    this->dead = false;
    this->direction = {}; 
    this->previous_direction = {}; 
  }
};