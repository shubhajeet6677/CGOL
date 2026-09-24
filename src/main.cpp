#include "grid/grid.h"
#include "raylib.h"
#include "renderer/renderer.h"
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
  } catch (const std::out_of_range &) {
    threw = true;
  }
  assert(threw);

  printf("All Grid tests passed.\n");
}

static void runRulesTests() {
  assert(nextCellState(true, 0) == false);
  assert(nextCellState(true, 1) == false);
  assert(nextCellState(true, 2) == true);
  assert(nextCellState(true, 3) == true);
  assert(nextCellState(true, 4) == false);
  assert(nextCellState(true, 8) == false);
  assert(nextCellState(false, 2) == false);
  assert(nextCellState(false, 3) == true);
  assert(nextCellState(false, 4) == false);

  Grid grid(5, 5);
  grid.setAlive(1, 1, true);
  grid.setAlive(2, 1, true);
  grid.setAlive(1, 2, true);
  assert(countAliveNeighbors(grid, 2, 2) == 3);
  assert(countAliveNeighbors(grid, 0, 0) == 1);
  assert(countAliveNeighbors(grid, 4, 4) == 0);

  printf("All Rules tests passed.\n");
}

static void runSimulationStateTests() {
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
  {
    SimulationState sim(5, 5);
    sim.setAlive(1, 2, true);
    sim.setAlive(2, 2, true);
    sim.setAlive(3, 2, true);
    sim.advanceOneGeneration();
    assert(sim.isAlive(2, 1) == true);
    assert(sim.isAlive(2, 2) == true);
    assert(sim.isAlive(2, 3) == true);
    assert(sim.isAlive(1, 2) == false);
    assert(sim.isAlive(3, 2) == false);
    sim.advanceOneGeneration();
    assert(sim.isAlive(1, 2) == true);
    assert(sim.isAlive(2, 2) == true);
    assert(sim.isAlive(3, 2) == true);
  }
  {
    SimulationState sim(5, 5);
    sim.setAlive(1, 1, true);
    sim.setAsInitial();
    sim.advanceOneGeneration();
    assert(sim.isAlive(1, 1) == false);
    assert(sim.generation() == 1);
    sim.reset();
    assert(sim.isAlive(1, 1) == true);
    assert(sim.generation() == 0);
  }

  printf("All SimulationState tests passed.\n");
}

int main() {
  runGridTests();
  runRulesTests();
  runSimulationStateTests();

  const int gridWidth = 40;
  const int gridHeight = 30;
  const int cellSize = 20;

  SimulationState sim(gridWidth, gridHeight);
  sim.setAlive(1, 0, true);
  sim.setAlive(2, 1, true);
  sim.setAlive(0, 2, true);
  sim.setAlive(1, 2, true);
  sim.setAlive(2, 2, true);
  sim.setAsInitial();

  InitWindow(gridWidth * cellSize, gridHeight * cellSize,
             "Conway's Game of Life");
  SetTargetFPS(60);
  bool running = false;
  double targetGenerationsPerSecond = 5.0;
  double timeAccumulator = 0.0;
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    if (IsKeyPressed(KEY_SPACE)) {
      running = !running;
    }
    if (IsKeyPressed(KEY_RIGHT) && !running) {
      sim.advanceOneGeneration();
    }
    if (IsKeyPressed(KEY_UP)) {
      targetGenerationsPerSecond += 1.0;
    }
    if (IsKeyPressed(KEY_DOWN)) {
      targetGenerationsPerSecond = (targetGenerationsPerSecond > 1.0)
                                       ? targetGenerationsPerSecond - 1.0
                                       : 1.0;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      int gx = GetMousePosition().x / cellSize;
      int gy = GetMousePosition().y / cellSize;
      if (gx >= 0 && gx < sim.width() && gy >= 0 && gy < sim.height()) {
        sim.setAlive(gx, gy, IsMouseButtonDown(MOUSE_BUTTON_LEFT));
      }
    }
    if (running) {
      timeAccumulator += dt;
      double secondsPerGeneration = 1.0 / targetGenerationsPerSecond;
      while (timeAccumulator >= secondsPerGeneration) {
        sim.advanceOneGeneration();
        timeAccumulator -= secondsPerGeneration;
      }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawSimulationGrid(sim, cellSize);
    DrawText(TextFormat("Gen: %lld Pop: %d Speed: %.0f/s [%s]",
                        sim.generation(), sim.getPopulation(),
                        targetGenerationsPerSecond,
                        running ? "RUNNING" : "PAUSED"),
             10, gridHeight * cellSize - 25, 18, DARKGRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}