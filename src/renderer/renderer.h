#pragma once
#include "../simulation/simulation_state.h"
#include "raylib.h"

inline void drawSimulationGrid(const SimulationState &sim, int cellSize) {
  for (int y = 0; y < sim.height(); ++y) {
    for (int x = 0; x < sim.width(); ++x) {
      int screenx = x * cellSize;
      int screeny = y * cellSize;

      Color color = sim.isAlive(x, y) ? BLACK : RAYWHITE;
      DrawRectangle(screenx, screeny, cellSize, cellSize, color);
      DrawRectangleLines(screenx, screeny, cellSize, cellSize, LIGHTGRAY);
    }
  }
}