#pragma once
#ifndef HEXAGON_ORIENTATION_HPP
#define HEXAGON_ORIENTATION_HPP

namespace Hexagon
{
  struct Orientation
  {
      const double f0;
      const double f1;
      const double f2;
      const double f3;
      const double b0;
      const double b1;
      const double b2;
      const double b3;
      const double start_angle;
      Orientation(
          double f0_, double f1_, double f2_, double f3_, 
          double b0_, double b1_, double b2_, double b3_, 
          double start_angle_);
  };
};

#endif /* HEXAGON_ORIENTATION_HPP */
