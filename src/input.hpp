#pragma once 

#include "base_types.hpp"
#include "vector.hpp"

struct Input {
  bool input_handled = true;
  bool input_queued = false;

  Vec2<s32> direction;
  Vec2<s32> queued_direction;

  void reset() {
    this->input_handled = true;
    this->input_queued = false;

    this->direction = {};
    this->queued_direction = {};
  }

  void input_direction(Vec2<s32> direction) {
    direction.x = fmax(-1, fmin(direction.x, 1));
    direction.y = fmax(-1, fmin(direction.y, 1));

    if (!this->input_handled) {
      this->queued_direction = direction;
      this->input_queued = true;
    } else {
      this->direction = direction;
      this->input_handled = false;
    }
  }

  Vec2<s32> consume_direction() {
    const Vec2<s32> direction = this->direction;
    if (this->input_queued) {
      this->direction = this->queued_direction;
      this->input_queued = false;
    } else {
      this->input_handled = true;
    }
    return direction;
  }
};