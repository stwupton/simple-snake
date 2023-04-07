#pragma once

#include "base_types.hpp"

struct Settings {
  u32 screen_width;
  u32 screen_height;

  u32 grid_width;
  u32 grid_height;

  u32 sim_time_ms;

  u32 cell_width() const {
    return this->screen_width / this->grid_width;
  }

  u32 cell_height() const {
    return this->screen_height / this->grid_height;
  }
};