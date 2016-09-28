#include "libhex.hpp"


#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

using std::abs;
using std::max;
using std::vector;

Point::Point(double x_, double y_): x(x_), y(y_) {}

template <typename T>
Hex::
Hex(T q_, T r_, T s_): q(q_), r(r_), s(s_) {}

template <typename T>
Hex::
Hex operator+ (const Hex &rhs) const 
{
    return Hex(q + rhs.q, r + rhs.r, s + rhs.s);
}

template <typename T>
Hex::
Hex operator- (const Hex &rhs) const
{
    return Hex(q - rhs.q, r - rhs.r, s - rhs.s);
}

template <typename T>
Hex::
Hex operator* (T rhs) const
{
    return Hex(q * rhs, r * rhs, s * rhs);
}

template <typename T>
Hex::
bool operator == (const Hex &rhs) const
{
    return q == rhs.q && r == rhs.r && s == rhs.s;
}

template <typename T>
Hex::
bool operator != (const Hex &rhs) const
{
    return !(*this == rhs);
}

template <typename T>
Hex::
int length()
{
    return int((abs(q) + abs(r) + abs(s)) / 2);
}

template <typename T>
Hex::
int distance(const Hex &rhs) const
{
    return (*this - rhs).length();
}

OffsetCoord::OffsetCoord(int col_, int row_): col(col_), row(row_) {}

Orientation::Orientation(
        double f0_, double f1_, double f2_, double f3_, 
        double b0_, double b1_, double b2_, double b3_, 
        double start_angle_): 
        f0(f0_), f1(f1_), f2(f2_), f3(f3_), 
        b0(b0_), b1(b1_), b2(b2_), b3(b3_), 
        start_angle(start_angle_) {}
        
Layout::Layout(Orientation orientation_, Point size_, Point origin_): 
        orientation(orientation_), size(size_), origin(origin_) {}
