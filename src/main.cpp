#include "draw/draw.hpp"
#include "util/std.hpp"
#include <functional>

constexpr u32 DIM = 800, FPS = 60;

u32 bound(const u32 &num, const u32 &min, const u32 &max) {
  return std::max(std::min(num, max), min);
}

void checkKeyEvent(char key, std::function<void(void)> callback,
    bool heldDown=false) {
  if(heldDown) {
    if(IsKeyDown(key)) {
      callback();
    }
  } else {
    if(IsKeyPressed(key)) {
      callback();
    }
  }
}

i32 main(UNUSED i32 argc, UNUSED char **argv) {
  InitWindow(DIM, DIM, "Cellular Automaton");
  SetTargetFPS(FPS);

  Vector2 mousePos;
  u32 x, y;

  bool running = false;

  CellManager cells(DIM, 16);

  while(!WindowShouldClose()) {
    // updating
    if(!running) {
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Get the current mouse position
        mousePos = GetMousePosition();
        x = bound(static_cast<u32>(mousePos.x), 0, DIM);
        y = bound(static_cast<u32>(mousePos.y), 0, DIM);
        cells.setAlive(x, y);
      } else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        mousePos = GetMousePosition();
        x = bound(static_cast<u32>(mousePos.x), 0, DIM);
        y = bound(static_cast<u32>(mousePos.y), 0, DIM);
        cells.setDead(x, y);
      }
      // check key clicks
      checkKeyEvent('C', [&]() { cells.clear(); }, true);
      checkKeyEvent('S', [&]() { cells.step(); });
      checkKeyEvent('R', [&]() { running = true; });
    } else {
      cells.step();
      WaitTime(0.1);
      running = !cells.done();
      // check key click
      checkKeyEvent('C', [&]() {
        cells.clear();
        running = false;
      }, true);
    }

    // rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);
    cells.draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
