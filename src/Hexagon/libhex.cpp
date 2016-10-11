#include "libhex.hpp"

#include <iostream>

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <algorithm>
#include <iterator>

#include <glm/ext.hpp>

using namespace Hexagon;
//using std::abs;
using std::max;
//using std::vector;

Point::Point(double x_, double y_): glm::f64vec2(x_,y_) {} // x(x_), y(y_) {}


FractionalHex Point::hex(const Layout &layout) const
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    Point pt = Point((x - origin.x) / size.x, (y - origin.y) / size.y);
    double q_ = M.b0 * pt.x + M.b1 * pt.y;
    double r_ = M.b2 * pt.x + M.b3 * pt.y;
    return FractionalHex(q_, r_, -q_-r_);
}

/*
template <typename T>
T Vec3<T>::length()
{
    return T((abs(q) + abs(r) + abs(s)) / 2);
}

template <typename T>
T Vec3<T>::distance(const Vec3 &rhs) const
{
    return (*this - rhs).length();
}
*/

Hex::Hex(int q_, int r_, int s_) : glm::i32vec3(q_, r_, s_) { assert( (q_ + r_ + s_) == 0); }

Hex::Hex(const glm::i32vec3 &orig) : glm::i32vec3(orig) {}
Hex::Hex(const Hex &orig ) : Hex(static_cast<glm::i32vec3>(orig)) {}

int Hex::norm_hex(void) const
{
    return int((abs(x) + abs(y) + abs(z)) / 2);
}

int Hex::distance_hex(const Hex &rhs) const
{
    return Hex(*this - rhs).norm_hex();
}

const vector<Hex> Hex::directions = 
{
    Hex( 1,  0, -1), 
    Hex( 1, -1,  0), 
    Hex( 0, -1,  1), 
    Hex(-1,  0,  1), 
    Hex(-1,  1,  0), 
    Hex( 0,  1, -1)
};

Hex Hex::direction(int direction)
{
    direction = (6 + (direction % 6)) % 6;
    assert(direction >= 0 && direction < 6);
    return directions[direction];
}

Hex Hex::neighbor(int dir)
{
    return Hex(*this + direction(dir));
}

const vector<Hex> Hex::diagonals = 
{
    Hex( 2, -1, -1), 
    Hex( 1, -2,  1), 
    Hex(-1, -1,  2), 
    Hex(-2,  1,  1), 
    Hex(-1,  2, -1), 
    Hex( 1,  1, -2)
};

Hex Hex::diagonal(int diagonal)
{
    diagonal = (6 + (diagonal % 6)) % 6;
    assert(0 >= diagonal && diagonal < 6);
    return diagonals[diagonal];
}

Hex Hex::diagonal_neighbor(int direction)
{
    return Hex(*this + diagonals[direction]);
}

vector<Hex> Hex::drawLine(Hex to)
{
    int N = distance_hex(to);
    FractionalHex from_nudge = FractionalHex(   x + 0.000001,    y + 0.000001,    z - 0.000002);
    FractionalHex to_nudge   = FractionalHex(to.x + 0.000001, to.y + 0.000001, to.z - 0.000002);
    vector<Hex> results = {};
    double step = 1.0 / max(N, 1);
    for (int i = 0; i <= N; i++)
    {
        results.push_back( from_nudge.lerp(to_nudge, step * i).round2hex() );
    }
    return results;
}

Point Hex::point(const Layout &layout) 
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    
    /*
    std::cerr << __FUNCTION__ << ": " 
        << glm::to_string(static_cast<glm::vec2>(size)) << std::endl;
    std::cerr << __FUNCTION__ << ": " 
        << glm::to_string(static_cast<glm::vec2>(origin)) << std::endl;
    */
    
    double x_ = (M.f0 * x + M.f1 * y) * size.x;
    double y_ = (M.f2 * x + M.f3 * y) * size.y;
    /*
    std::cerr << __FUNCTION__ << ": " << M.f0 << ":" << M.f1 <<std::endl;
    std::cerr << __FUNCTION__ << ": " << M.f2 << ":" << M.f3 <<std::endl;
    std::cerr << __FUNCTION__ << ": " << x    << ":" << y    <<std::endl;
    std::cerr << __FUNCTION__ << ": " << x_   << ":" << y_   <<std::endl;
    */
    
    x_ = x_ + origin.x;
    y_ = y_ + origin.y;
    return Point(x_, y_);
   
}

vector<Point> Hex::polygon_corners(const Layout &layout)
{
    vector<Point> corners = {};
    Point center = point(layout);
    /*
    std::cerr << __FUNCTION__ << ": " 
        << glm::to_string(static_cast<glm::vec2>(center)) << std::endl;
    */
    for (int i = 0; i < 6; i++)
    {
        Point offset = layout.corner_offset(i);
        corners.push_back(Point(center.x + offset.x, center.y + offset.y));
    }
    return corners;
}

OffsetCoord Hex::hex2qoffset(int offset)
{
    int col = x;
    int row = y + int((x + offset * (x & 1)) / 2);
    return OffsetCoord(col, row);
}


OffsetCoord Hex::hex2roffset(int offset)
{
    int col = x + int((y + offset * (y & 1)) / 2);
    int row = y;
    return OffsetCoord(col, row);
}


FractionalHex::FractionalHex(double q_, double r_, double s_) : glm::f64vec3(q_, r_, s_) {}
FractionalHex::FractionalHex(const glm::f64vec3 &orig)      : glm::f64vec3(orig.x, orig.y, orig.z) {}
FractionalHex::FractionalHex(const FractionalHex &orig )   : FractionalHex( static_cast<glm::f64vec3>(orig) ) {}


Hex FractionalHex::round2hex() const
{
    int tmp_q = int(round(x));
    int tmp_r = int(round(y));
    int tmp_s = int(round(z));
    double q_diff = abs(tmp_q - x);
    double r_diff = abs(tmp_r - y);
    double s_diff = abs(tmp_s - z);
    if (q_diff > r_diff and q_diff > s_diff)
    {
        tmp_q = -tmp_r - tmp_s;
    }
    else
        if (r_diff > s_diff)
        {
            tmp_r = -tmp_q - tmp_s;
        }
        else
        {
            tmp_s = -tmp_q - tmp_r;
        }
    return Hex(tmp_q, tmp_r, tmp_s);
}

FractionalHex FractionalHex::lerp(const FractionalHex &rhs, double t) const
{
    return FractionalHex(x * (1. - t) + rhs.x * t, y * (1. - t) + rhs.y * t, z * (1. - t) + rhs.z * t);
}

const Orientation Layout::pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 
          3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 
          0.0, 2.0 / 3.0, 0.5);
      
const Orientation Layout::flat   = 
        Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, 
          sqrt(3.0), 2.0 / 3.0, 0.0, 
          -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);



OffsetCoord::OffsetCoord(int col_, int row_): glm::i32vec2(col_,row_) {} //col(col_), row(row_) {}

Hex OffsetCoord::qoffset2hex(int offset)
{
    int q = x;
    int r = y - int((x + offset * (x & 1)) / 2);
    int s = -q - r;
    return Hex(q, r, s);
}

Hex OffsetCoord::roffset2hex(int offset)
{
    int q = x- int((y + offset * (y & 1)) / 2);
    int r = y;
    int s = -q - r;
    return Hex(q, r, s);
}


Orientation::Orientation(
        double f0_, double f1_, double f2_, double f3_, 
        double b0_, double b1_, double b2_, double b3_, 
        double start_angle_): 
        f0(f0_), f1(f1_), f2(f2_), f3(f3_), 
        b0(b0_), b1(b1_), b2(b2_), b3(b3_), 
        start_angle(start_angle_) {}

Orientation::Orientation( const Orientation &o ): 
        f0(o.f0), f1(o.f1), f2(o.f2), f3(o.f3), 
        b0(o.b0), b1(o.b1), b2(o.b2), b3(o.b3), 
        start_angle(o.start_angle) {}
        
Orientation& Orientation::operator=(const Orientation& o)
{
    f0 = o.f0; f1 = o.f1; f2 = o.f2; f3 = o.f3; 
    b0 = o.b0; b1 = o.b1; b2 = o.b2; b3 =o.b3;
    start_angle = o.start_angle;
    return *this;
}

Layout::Layout(Orientation orientation_, Point size_, Point origin_): 
        orientation(orientation_), size(size_), origin(origin_) {}

Layout& Layout::operator=(const Layout &rhs)
{
  orientation = rhs.orientation;
  size = rhs.size;
  origin = rhs.origin;
  return *this;
}
        
Point Layout::corner_offset(int corner) const
{
    corner = (6 + (corner % 6)) % 6;
    assert(corner >= 0 && corner < 6);
    
    Orientation M = orientation;
    double angle = 2.0 * M_PI * (M.start_angle - corner) / 6;
    return Point(size.x * cos(angle), size.y * sin(angle));
}

