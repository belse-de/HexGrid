#pragma once
#ifndef HEXAGON_POINT_HPP
#define HEXAGON_POINT_HPP

# include "Hexagon/Hex.hpp"

namespace Hexagon
{
  struct OffsetCoord
  {
      const int col;
      const int row;
      OffsetCoord(int col_, int row_);
      
      Hex qoffset2hex(int offset);
      Hex roffset2hex(int offset);
  };
};

#endif /* HEXAGON_POINT_HPP */
