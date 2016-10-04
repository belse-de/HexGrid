#pragma once
#ifndef HEXAGON_HEX_HPP
#define HEXAGON_HEX_HPP

#include <vector>
using std::vector;

#include "Point.hpp"
#include "Layout.hpp"
#include "OffsetCoord.hpp"

namespace Hexagon
{
  
  template <typename T>
  struct Vec3
  {
      const T q;
      const T r;
      const T s;
      // Vec3(); FIXME
      Vec3(T q_, T r_, T s_);

      virtual Vec3 operator+  (const Vec3 &rhs) const;
      virtual Vec3 operator-  (const Vec3 &rhs) const;
      virtual Vec3 operator*  (T rhs) const;
      virtual bool operator== (const Vec3 &rhs) const;
      virtual bool operator!= (const Vec3 &rhs) const;
        
      T length();
      T distance(const Vec3 &rhs) const;    
  };
  
  struct Hex: public Vec3<int>
  {
      Hex(int q_, int r_, int s_);
      Hex(const Vec3<int> &orig);
      Hex(const Hex &orig );
      
      static const vector<Hex> directions;
      static Hex direction(int direction);

      static const vector<Hex> diagonals;
      static Hex diagonal(int diagonal);
      
      Hex neighbor(int direction);
      Hex diagonal_neighbor(int direction);
      
      vector<Hex> drawLine(Hex to);
      Point point(const Layout &layout);
      vector<Point> polygon_corners(Layout layout);
      
      OffsetCoord hex2qoffset(int offset);
      OffsetCoord hex2roffset(int offset);
  };

  struct FractionalHex: public Vec3<float>
  {
      FractionalHex(float q_, float r_, float s_);
      FractionalHex(const Vec3<float> &orig);
      FractionalHex(const FractionalHex &orig );
      
      Hex round2hex() const;
      FractionalHex lerp(const FractionalHex &rhs, float t) const;
  };
};

#endif /* HEXAGON_HEX_HPP */
