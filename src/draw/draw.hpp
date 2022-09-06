#pragma once

#include "../util/std.hpp"
#include <cassert>
#include <utility>

struct Cell {
  Cell(const u32 &x, const u32 &y) {
    this->x = x;
    this->y = y;
    this->alive = false;
    this->nextAlive = false;
  }
  void setLive(const bool &alive) {
    this->alive = alive;
  }
  bool getLive() {
    return this->alive;
  }
  void setNextAlive(const bool &alive) {
    this->nextAlive = alive;
  }
  bool getNextAlive() {
    return this->nextAlive;
  }
private:
  bool alive;
  bool nextAlive;
  u32 x, y;
};

struct CellManager {
  CellManager(const u32 &dim, const u32 &pixelSize) {
    this->dim = dim;
    this->pixelSize = pixelSize;
    assert(dim % pixelSize == 0);
    this->gridSize = dim / pixelSize;
    // Generate cells
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        this->cells.emplace_back(std::move(Cell(x, y)));
      }
    }
  }

  Cell *getCell(const u32 &x, const u32 &y) {
    return &this->cells[y * this->gridSize + x];
  }

  void clear() {
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        Cell *cell = getCell(x, y);
        cell->setLive(false);
        cell->setNextAlive(false);
      }
    }
  }

  bool done() {
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        Cell *cell = getCell(x, y);
        if(cell->getNextAlive() == true) {
          return false;
        }
      }
    }
    return true;
  }

  void setAlive(const u32 &x, const u32 &y) {
    Cell *cell = getCell(x / this->pixelSize, y / this->pixelSize);
    cell->setLive(true);
  }

  void setDead(const u32 &x, const u32 &y) {
    Cell *cell = getCell(x / this->pixelSize, y / this->pixelSize);
    cell->setLive(false);
  }

  void draw() {
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        Cell *cell = getCell(x, y);
        DrawRectangle(x * pixelSize, y * pixelSize, pixelSize, pixelSize,
            cell->getLive() ? BLACK : WHITE);
      }
    }
  }

  u32 countLiveNeighbours(const u32 &x, const u32 &y) {
    u32 ret = 0;
    std::vector<i32> dy = { 1, 1, 1,  0, 0, -1, -1, -1};
    std::vector<i32> dx = {-1, 0, 1, -1, 1, -1,  0,  1};
    for(int i = 0; i < 8; ++i) {
      if(x + dx[i] >= 0 && y + dy[i] >= 0 &&
          x + dx[i] < this->gridSize && y + dy[i] < this->gridSize) {
        if(getCell(x + dx[i], y + dy[i])->getLive() == true) {
          ret += 1;
        }
      }
    }
    return ret;
  }

  void step() {
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        Cell *cell = getCell(x, y);
        u32 neighbours = countLiveNeighbours(x, y);
        if(cell->getLive() && neighbours < 2) {
          // Living cells die if they have < 2 neighbors
          cell->setNextAlive(false);
        } else if(cell->getLive() && neighbours > 3) {
          // Living cells die if they have > 3 neighbors
          cell->setNextAlive(false);
        } else if(!cell->getLive() && neighbours == 3) {
          // Dead cells that have 3 neighbors become alive
          cell->setNextAlive(true);
        } else if(cell->getLive() && (neighbours == 3 || neighbours == 2)) {
          // 2 or 3 neighbours alive == survive
          cell->setNextAlive(true);
        }
      }
    }
    for(u32 y = 0; y < this->gridSize; ++y) {
      for(u32 x = 0; x < this->gridSize; ++x) {
        Cell *cell = getCell(x, y);
        cell->setLive(cell->getNextAlive());
      }
    }
  }
private:
  std::vector<Cell> cells;
  u32 dim, pixelSize, gridSize;
};
