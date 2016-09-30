#include "libhex.hpp"


#include <cmath>
#include <cstdlib>
#include <cassert>

#include <algorithm>
#include <iterator>


using std::abs;
using std::max;
using std::vector;

Point::Point(double x_, double y_): x(x_), y(y_) {}

FractionalHex Point::hex(const Layout &layout)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    Point pt = Point((x - origin.x) / size.x, (y - origin.y) / size.y);
    double q_ = M.b0 * pt.x + M.b1 * pt.y;
    double r_ = M.b2 * pt.x + M.b3 * pt.y;
    return FractionalHex(q_, r_, -q_-r_);
}


/* FIXME
template <typename T>
Vec3<T>::Vec3() : Vec3<T>(0, 0, 0) {}
*/

template <typename T>
Vec3<T>::Vec3(T q_, T r_, T s_): q(q_), r(r_), s(s_) {}

template <typename T>
Vec3<T> Vec3<T>::operator+ (const Vec3 &rhs) const 
{
    return Vec3(q + rhs.q, r + rhs.r, s + rhs.s);
}

template <typename T>
Vec3<T> Vec3<T>::operator- (const Vec3 &rhs) const
{
    return Vec3(q - rhs.q, r - rhs.r, s - rhs.s);
}

template <typename T>
Vec3<T> Vec3<T>::operator* (T rhs) const
{
    return Vec3(q * rhs, r * rhs, s * rhs);
}

template <typename T>
bool Vec3<T>::operator == (const Vec3 &rhs) const
{
    return q == rhs.q && r == rhs.r && s == rhs.s;
}

template <typename T>
bool Vec3<T>::operator != (const Vec3 &rhs) const
{
    return !(*this == rhs);
}

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

Hex::Hex(int q_, int r_, int s_) : Vec3<int>(q_, r_, s_) { assert( (q_ + r_ + s_) == 0); }

Hex::Hex(const Vec3<int> &orig) : Vec3<int>(orig.q, orig.r, orig.s) {}
Hex::Hex(const Hex &orig ) : Hex(static_cast<Vec3<int>>(orig)) {}

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
    assert(0 >= direction && direction < 6);
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
    int N = distance(to);
    FractionalHex from_nudge = FractionalHex(q + 0.000001, r + 0.000001, s - 0.000002);
    FractionalHex to_nudge = FractionalHex(to.q + 0.000001, to.r + 0.000001, to.s - 0.000002);
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
    double x_ = (M.f0 * q + M.f1 * r) * size.x;
    double y_ = (M.f2 * q + M.f3 * r) * size.y;
    x_ = x_ + origin.x;
    y_ = y_ + origin.y;
    return Point(x_, y_);
   
}

FractionalHex::FractionalHex(float q_, float r_, float s_) : Vec3<float>(q_, r_, s_) {}
FractionalHex::FractionalHex(const Vec3<float> &orig)      : Vec3<float>(orig.q, orig.r, orig.s) {}
FractionalHex::FractionalHex(const FractionalHex &orig )   : FractionalHex( static_cast<Vec3<float>>(orig) ) {}


Hex FractionalHex::round2hex() const
{
    int tmp_q = int(round(q));
    int tmp_r = int(round(r));
    int tmp_s = int(round(s));
    float q_diff = abs(tmp_q - q);
    float r_diff = abs(tmp_r - r);
    float s_diff = abs(tmp_s - s);
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

FractionalHex FractionalHex::lerp(const FractionalHex &rhs, float t) const
{
    return FractionalHex(q * (1. - t) + rhs.q * t, r * (1. - t) + rhs.r * t, s * (1. - t) + rhs.s * t);
}





OffsetCoord::OffsetCoord(int col_, int row_): col(col_), row(row_) {}

Orientation::Orientation(
        double f0_, double f1_, double f2_, double f3_, 
        double b0_, double b1_, double b2_, double b3_, 
        double start_angle_): 
        f0(f0_), f1(f1_), f2(f2_), f3(f3_), 
        b0(b0_), b1(b1_), b2(b2_), b3(b3_), 
        start_angle(start_angle_) {}


const Orientation Layout::pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const Orientation Layout::flat   = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

Layout::Layout(Orientation orientation_, Point size_, Point origin_): 
        orientation(orientation_), size(size_), origin(origin_) {}
