#pragma once
#ifndef LIBHEX_HPP
#define LIBHEX_HPP

#include <vector>
using std::vector;
#include <functional>
using std::hash;

#include <glm/glm.hpp>

namespace Hexagon
{
  struct Layout;
  struct Hex;
  struct FractionalHex;
  
  struct Orientation
  {
    double f0;
    double f1;
    double f2;
    double f3;
    double b0;
    double b1;
    double b2;
    double b3;
    double start_angle;
    Orientation(
        double f0_, double f1_, double f2_, double f3_, 
        double b0_, double b1_, double b2_, double b3_, 
        double start_angle_);
    Orientation( const Orientation &o ); 
    Orientation& operator=(const Orientation&);   
  };
  
  struct Point : public glm::f64vec2
  {
    Point(double x_, double y_);
    
    FractionalHex hex(const Layout &layout) const;
  };
  
  struct Layout
  {
    static const Orientation pointy;
    static const Orientation flat;
    
    Orientation orientation;
    Point size;
    Point origin;
    Layout(Orientation orientation_=Layout::pointy, Point size_=Point(1,1), Point origin_=Point(0,0));
    Layout& operator=(const Layout&);
    
    Point corner_offset(int corner) const;
  };
  
  
   struct OffsetCoord : public glm::i32vec2
  {
    OffsetCoord(int col_, int row_);
    
    Hex qoffset2hex(int offset);
    Hex roffset2hex(int offset);
  };
  
  struct Hex: public glm::i32vec3
  {
    Hex(int q_, int r_, int s_);
    Hex(const glm::i32vec3 &orig);
    Hex(const Hex &orig );
    
    static const vector<Hex> directions;
    static Hex direction(int direction);

    static const vector<Hex> diagonals;
    static Hex diagonal(int diagonal);
    
    int norm_hex(void) const;
    int distance_hex(const Hex &rhs) const;
    
    Hex neighbor(int direction);
    Hex diagonal_neighbor(int direction);
    
    vector<Hex> drawLine(Hex to);
    Point point(const Layout &layout);
    vector<Point> polygon_corners(const Layout &layout);
    
    OffsetCoord hex2qoffset(int offset);
    OffsetCoord hex2roffset(int offset);
  };

  struct FractionalHex: public glm::f64vec3
  {
    FractionalHex(double q_, double r_, double s_);
    FractionalHex(const glm::f64vec3 &orig);
    FractionalHex(const FractionalHex &orig );
    
    Hex round2hex() const;
    FractionalHex lerp(const FractionalHex &rhs, double t) const;
  };



const int EVEN =  1;
const int ODD  = -1;

};

namespace std {
  template <> struct hash<Hexagon::Hex> {
    size_t operator()(const Hexagon::Hex& h) const {
      hash<int> int_hash;
      size_t hq = int_hash(h.x);
      size_t hr = int_hash(h.y);
      return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
  };
}

#endif /* LIBHEX_HPP */
