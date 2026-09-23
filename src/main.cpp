#include "grid/grid.h"
#include "raylib.h"
#include "simulation/rules.h"
#include <cassert>
#include <cstdio>

static void runGridTests() {
  Grid grid(10, 5);
  assert(grid.isAlive(0, 0) == false);
  assert(grid.isAlive(9, 4) == false);

  grid.setAlive(3, 2, true);
  assert(grid.isAlive(3, 2) == true);

  assert(grid.isAlive(3, 3) == false);
  assert(grid.isAlive(4, 2) == false);

  bool threw = false;
  try {
    grid.isAlive(-1, 0);
  } catch (const std::out_of_range &) {
    threw = true;
  }
  assert(threw);
  threw = false;
  try {
    grid.isAlive(10, 0);
  } catch (const std::out_of_range) {
    threw = true;
  }
  assert(threw);

  printf("all grid test passed.\n");
}

static void runRulesTests() {
  // nextCellState: all 9 "alive" cases
  assert(nextCellState(true, 0) == false); // underpopulation
  assert(nextCellState(true, 1) == false); // underpopulation
  assert(nextCellState(true, 2) == true);  // survives
  assert(nextCellState(true, 3) == true);  // survives
  assert(nextCellState(true, 4) == false); // overpopulation
  assert(nextCellState(true, 8) == false); // overpopulation

  // nextCellState: dead cell cases
  assert(nextCellState(false, 2) == false);
  assert(nextCellState(false, 3) == true); // reproduction
  assert(nextCellState(false, 4) == false);

  // countAliveNeighbors: known configuration
  Grid grid(5, 5);
  grid.setAlive(1, 1, true);
  grid.setAlive(2, 1, true);
  grid.setAlive(1, 2, true);
  // (2,2) has exactly those 3 as neighbors
  assert(countAliveNeighbors(grid, 2, 2) == 3);
  // (0,0) is a corner: only 3 possible neighbors exist, none alive
  assert(countAliveNeighbors(grid, 0, 0) == 1);
  // (4,4) is the opposite corner, also isolated
  assert(countAliveNeighbors(grid, 4, 4) == 0);

  printf("All Rules tests passed.\n");
}

int main() {
  runGridTests();
  runRulesTests();
  InitWindow(800, 600, "Conway's Game of Life");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
