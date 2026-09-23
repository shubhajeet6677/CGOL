#pragma once
#include "../grid/grid.h"
inline int countAliveNeighbors(const Grid &grid, int x, int y) {
  int count = 0;
  for (int dy = -1; dy <= 1; ++dy) {
    for (int dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      int nx = x + dx;
      int ny = y + dy;
      if (nx < 0 || nx >= grid.width() || ny < 0 || ny >= grid.height()) {
        continue;
      }
      if (grid.isAlive(nx, ny)) {
        ++count;
      }
    }
  }
  return count;
}
inline bool nextCellState(bool currentlyAlive, int aliveNeighbors) {
  if (currentlyAlive) {
    return aliveNeighbors == 2 || aliveNeighbors == 3;
  } else {
    return aliveNeighbors == 3;
  }
}