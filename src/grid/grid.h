#pragma once

#include "cell.h"
#include <stdexcept>
#include <vector>

class Grid {
public:
  Grid(int width, int height)
      : width_(width), height_(height),
        cells_(static_cast<size_t>(width) * height) {}
  int width() const { return width_; }
  int height() const { return height_; }
  bool isAlive(int x, int y) const { return cells_[index(x, y)].alive; }
  void setAlive(int x, int y, bool alive) { cells_[index(x, y)].alive = alive; }

private:
  size_t index(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
      throw std::out_of_range("Grid index out of bounds");
    }
    return static_cast<size_t>(y) * width_ + x;
  }
  int width_;
  int height_;
  std::vector<Cell> cells_;
};
