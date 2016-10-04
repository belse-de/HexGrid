#pragma once
#ifndef HEXAGON_POINT_HPP
#define HEXAGON_POINT_HPP

# include "Layout.hpp"
# include "Hex.hpp"

namespace Hexagon
{
  struct Point
  {
      const double x;
      const double y;
      Point(double x_, double y_);
      
      FractionalHex hex(const Layout &layout);
  };
};

#endif /* HEXAGON_POINT_HPP */
