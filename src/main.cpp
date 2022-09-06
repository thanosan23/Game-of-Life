#include "draw/draw.hpp"
#include "util/std.hpp"

constexpr u32 DIM = 800;
constexpr u8 FPS = 60;

u32 bound(const u32 &num, const u32 &min, const u32 &max) {
  return std::max(std::min(num, max), min);
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

      if(IsKeyDown('C')) {
        cells.clear();
      } else if(IsKeyPressed('S')) {
        cells.step();
      } else if(IsKeyPressed('R')) {
        running = true;
      }
    } else {
      cells.step();
      WaitTime(0.1);
      running = !cells.done();
      if(IsKeyDown('C')) {
        cells.clear();
        running = false;
      }
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
