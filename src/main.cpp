#include "grid/grid.h"
#include "raylib.h"
#include "simulation/rules.h"
#include "simulation/simulation_state.h"
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

static void runSimulationStateTests() {
  // Still life: 2x2 block should be stable forever.
  {
    SimulationState sim(5, 5);
    sim.setAlive(1, 1, true);
    sim.setAlive(2, 1, true);
    sim.setAlive(1, 2, true);
    sim.setAlive(2, 2, true);

    sim.advanceOneGeneration();

    assert(sim.isAlive(1, 1) == true);
    assert(sim.isAlive(2, 1) == true);
    assert(sim.isAlive(1, 2) == true);
    assert(sim.isAlive(2, 2) == true);
    assert(sim.getPopulation() == 4);
    assert(sim.generation() == 1);
  }

  // Blinker (oscillator): horizontal row of 3 becomes vertical, and back.
  {
    SimulationState sim(5, 5);
    sim.setAlive(1, 2, true);
    sim.setAlive(2, 2, true);
    sim.setAlive(3, 2, true);

    sim.advanceOneGeneration();

    // Should now be vertical, centered on (2,2)
    assert(sim.isAlive(2, 1) == true);
    assert(sim.isAlive(2, 2) == true);
    assert(sim.isAlive(2, 3) == true);
    assert(sim.isAlive(1, 2) == false);
    assert(sim.isAlive(3, 2) == false);

    sim.advanceOneGeneration();

    // Should be horizontal again
    assert(sim.isAlive(1, 2) == true);
    assert(sim.isAlive(2, 2) == true);
    assert(sim.isAlive(3, 2) == true);
  }

  // reset() and setAsInitial()
  {
    SimulationState sim(5, 5);
    sim.setAlive(1, 1, true);
    sim.setAsInitial(); // this alive cell is now "initial"

    sim.advanceOneGeneration(); // lone cell dies (0 neighbors)
    assert(sim.isAlive(1, 1) == false);
    assert(sim.generation() == 1);

    sim.reset();
    assert(sim.isAlive(1, 1) == true); // back to initial state
    assert(sim.generation() == 0);
  }

  printf("All SimulationState tests passed.\n");
}

int main() {
  runGridTests();
  runRulesTests();
  runSimulationStateTests();
  InitWindow(800, 600, "Conway's Game of Life");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
