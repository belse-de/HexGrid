#pragma once
#ifndef HEXAGON_LAYOUT_HPP
#define HEXAGON_LAYOUT_HPP

#include "Orientation.hpp"
#include "Layout.hpp"

struct Point;

namespace Hexagon
{
  struct Layout
  {
      static const Orientation pointy;
      static const Orientation flat;
      
      const Orientation orientation;
      const Point size;
      const Point origin;
      Layout(Orientation orientation_, Point size_, Point origin_);
      
      Point corner_offset(int corner);
  };
};

#endif /* HEXAGON_LAYOUT_HPP */
