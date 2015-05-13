#include "render_grid.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define XP 0
#define YP 1
#define ZP 2
#define XN 3
#define YN 4
#define ZN 5

RenderGrid::RenderGrid(unsigned int numberOfCubesInX,
                       unsigned int numberOfCubesInY,
                       unsigned int numberOfCubesInZ,
                       float _minX, float _maxX,
                       float _minY, float _maxY,
                       float _minZ, float _maxZ) :
                       grid(numberOfCubesInX, numberOfCubesInY, numberOfCubesInZ,
                            _minX, _maxX, _minY, _maxY, _minZ, _maxZ) {}

void RenderGrid::initialize() {
  dirty = false;
  RenderGridItem item;
  item.object = NULL;
  item.planes[0] = NULL;
  item.planes[1] = NULL;
  item.planes[2] = NULL;
  item.planes[3] = NULL;
  item.planes[4] = NULL;
  item.planes[5] = NULL;
  item.renderElement = NULL;
  item.dirty = true;
  grid.initialize(item);
}

void RenderGrid::createPlane(int x, int y, int z, int dir) {
  RenderGridItem center = grid(x, y, z);
  int left = x, right = x, top = y, bottom = y, front = z, back = z, i, j, k;
  int xN = 0, yN = 0, zN = 0;
  bool checkN = true, done = false;

  if(center.planes[dir] != NULL) {
    return;
  }

  if(dir == XP) {
    xN++;
  }
  if(dir == XN) {
    xN--;
  }
  if(dir == YP) {
    yN++;
  }
  if(dir == YN) {
    yN--;
  }
  if(dir == ZP) {
    zN++;
  }
  if(dir == ZN) {
    zN--;
  }
  checkN = x + xN >= 0 && x + xN < grid.getSizeX() &&
           y + yN >= 0 && y + yN < grid.getSizeY() &&
           z + zN >= 0 && z + zN < grid.getSizeZ();

  if(checkN && grid(x + xN, y + yN, z + zN).object != NULL) {
    return;
  }

  if(dir != XN && dir != XP) {
    done = false;
    for(left = x; left >= 0 && !done; left--) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!grid(left, j, k).isEqual(&center) || grid(left, j, k).object == NULL || grid(left, j, k).planes[dir] != NULL || (checkN && grid(left + xN, j + yN, k + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    left++;

    done = false;
    for(right = x; right < grid.getSizeX() && !done; right++) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!grid(right, j, k).isEqual(&center) || grid(right, j, k).object == NULL || grid(right, j, k).planes[dir] != NULL || (checkN && grid(right + xN, j + yN, k + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    right--;
  }
  
  if(dir != YN && dir != YP) {
    done = false;
    for(bottom = y; bottom >= 0 && !done; bottom--) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!grid(i, bottom, k).isEqual(&center) || grid(i, bottom, k).object == NULL || grid(i, bottom, k).planes[dir] != NULL || (checkN && grid(i + xN, bottom + yN, k + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    bottom++;

    done = false;
    for(top = y; top < grid.getSizeY() && !done; top++) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!grid(i, top, k).isEqual(&center) || grid(i, top, k).object == NULL || grid(i, top, k).planes[dir] != NULL || (checkN && grid(i + xN, top + yN, k + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    top--;
  }

  if(dir != ZN && dir != ZP) {
    done = false;
    for(back = z; back >= 0 && done; back--) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!grid(i, j, back).isEqual(&center) || grid(i, j, back).object == NULL || grid(i, j, back).planes[dir] != NULL || (checkN && grid(i + xN, j + yN, back + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    back++;

    done = false;
    for(front = z; front < grid.getSizeZ() && done; front++) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!grid(i, j, front).isEqual(&center) || grid(i, j, front).object == NULL || grid(i, j, front).planes[dir] != NULL || (checkN && grid(i + xN, j + yN, front + zN).object != NULL)) {
            done = true;
          }
        }
      }
    }
    front--;
  }

  Plane *plane = new Plane(left, right, bottom, top, back, front, dir, x, y, z,
                           grid.getMinX(), grid.getMaxY(), grid.getMinZ(),
                           grid.getEdgeSizeX(), grid.getEdgeSizeY(), grid.getEdgeSizeZ(),
                           center.object);
  plane->setup();

  for(i = left; i <= right; i++) {
    for(j = bottom; j <= top; j++) {
      for(k = back; k <= front; k++) {
        grid(i, j, k).planes[dir] = plane;
      }
    }
  }

  center.renderElement->addObject(plane->getObject());
}

void RenderGrid::addObject(Object *object, RenderElement *renderElement) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  RenderGridItem item;
  item.object = object;
  item.planes[0] = NULL;
  item.planes[1] = NULL;
  item.planes[2] = NULL;
  item.planes[3] = NULL;
  item.planes[4] = NULL;
  item.planes[5] = NULL;
  item.renderElement = renderElement;
  item.dirty = true;
  grid.setValue(pos.x, pos.y, pos.z, item);
  dirtyAdjacent(pos);
}

void RenderGrid::removeObject(Object *object) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  RenderGridItem item;
  item.object = NULL;
  item.renderElement = NULL;
  item.dirty = true;
  grid.setValue(pos.x, pos.y, pos.z, item);
  dirtyAdjacent(pos);
}

void RenderGrid::dirtyAdjacent(glm::vec4 pos) {
  dirtyCell(pos);
  dirtyCell(pos + glm::vec4(grid.getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(-grid.getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, grid.getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, -grid.getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, grid.getEdgeSizeZ(), 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, -grid.getEdgeSizeZ(), 0.0f));
}

void RenderGrid::dirtyCell(glm::vec4 pos) {
  if(grid.inGrid(pos.x, pos.y, pos.z)) {
    RenderGridItem item = grid.getValue(pos.x, pos.y, pos.z);
    int i;
    for(i = 0; i < 6; i++) {
      removePlane(item.planes[i]);
    }
    dirty = true;
  }
}

void RenderGrid::clean() {
  if(dirty) {
    int i, j, k;
    dirty = false;

    for(i = 0; i < grid.getSizeX(); i++) {
      for(j = 0; j < grid.getSizeY(); j++) {
        for(k = 0; k < grid.getSizeZ(); k++) {
          if(grid(i, j, k).dirty) {
            if(grid(i, j, k).object != NULL) {
              createPlane(i, j, k, XP);
              createPlane(i, j, k, XN);
              createPlane(i, j, k, YP);
              createPlane(i, j, k, YN);
              createPlane(i, j, k, ZP);
              createPlane(i, j, k, ZN);
            }
            grid(i, j, k).dirty = false;
          }
        }
      }
    }
  }
}

void RenderGrid::removePlane(Plane *plane) {
  if(plane == NULL) {
    return;
  }
  int i, j, k;
  grid(plane->getCenterX(), plane->getCenterY(), plane->getCenterZ()).renderElement->removeObject(plane->getObject());
  for(i = plane->getLeft(); i <= plane->getRight(); i++) {
    for(j = plane->getBottom(); j <= plane->getTop(); j++) {
      for(k = plane->getBack(); k <= plane->getFront(); k++) {
        grid(i, j, k).planes[plane->getDirection()] = NULL;
        grid(i, j, k).dirty = true;
      }
    }
  }
}
