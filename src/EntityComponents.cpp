#include <cstddef>
#include "EntityComponents.h"

using namespace TC::EC;

Position Pos::operator+(const Vel &vel) const
{ /* Relying on withinBoundary() to prevent overflow & underflow */
    return {static_cast<unsigned>(static_cast<int>(z) + vel.z),
        static_cast<unsigned>(static_cast<int>(y) + vel.y),
        static_cast<unsigned>(static_cast<int>(x) + vel.x)};
}