#pragma once

#include "../grid/grid.h"
#include "rules.h"

class SimulationState {
public:
  SimulationState(int width, int height)
      : initialGrid_(width, height), currentGrid_(width, height),
        nextGrid_(width, height), generation_(0) {}

  int width() const { return currentGrid_.width(); }
  int height() const { return currentGrid_.height(); }
  long long generation() const { return generation_; }

  bool isAlive(int x, int y) const { return currentGrid_.isAlive(x, y); }
  void setAlive(int x, int y, bool alive) {
    currentGrid_.setAlive(x, y, alive);
  }

  void advanceOneGeneration() {
    for (int y = 0; y < currentGrid_.height(); ++y) {
      for (int x = 0; x < currentGrid_.width(); ++x) {
        bool alive = currentGrid_.isAlive(x, y);
        int neighbors = countAliveNeighbors(currentGrid_, x, y);
        nextGrid_.setAlive(x, y, nextCellState(alive, neighbors));
      }
    }

    std::swap(currentGrid_, nextGrid_);
    ++generation_;
  }

  void setAsInitial() { initialGrid_ = currentGrid_; }

  void reset() {
    currentGrid_ = initialGrid_;
    generation_ = 0;
  }

  int getPopulation() const {
    int count = 0;
    for (int y = 0; y < currentGrid_.height(); ++y) {
      for (int x = 0; x < currentGrid_.width(); ++x) {
        if (currentGrid_.isAlive(x, y)) {
          ++count;
        }
      }
    }
    return count;
  }

private:
  Grid initialGrid_;
  Grid currentGrid_;
  Grid nextGrid_;
  long long generation_;
};