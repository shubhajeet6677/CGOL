#include "grid/grid.h"
#include "raylib.h"
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
int main() {
  runGridTests();
  InitWindow(800, 600, "Conway's Game of Life");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
