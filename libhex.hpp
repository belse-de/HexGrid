#pragma once
#ifndef LIBHEX_HPP
#define LIBHEX_HPP

#include <vector>
using std::vector;

struct Layout;
struct Hex;
struct FractionalHex;

struct Point
{
    const double x;
    const double y;
    Point(double x_, double y_);
    
    FractionalHex hex(const Layout &layout);
};

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


struct Layout
{
    static const Orientation pointy;
    static const Orientation flat;
    
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_);
};


template <typename T>
struct Vec3
{
    const T q;
    const T r;
    const T s;
    // Vec3(); FIXME
    Vec3(T q_, T r_, T s_);

    Vec3 operator+  (const Vec3 &rhs) const;
    Vec3 operator-  (const Vec3 &rhs) const;
    Vec3 operator*  (T rhs) const;
    bool operator== (const Vec3 &rhs) const;
    bool operator!= (const Vec3 &rhs) const;
      
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
};

struct FractionalHex: public Vec3<float>
{
    FractionalHex(float q_, float r_, float s_);
    FractionalHex(const Vec3<float> &orig);
    FractionalHex(const FractionalHex &orig );
    
    Hex round2hex() const;
    FractionalHex lerp(const FractionalHex &rhs, float t) const;
};

struct OffsetCoord
{
    const int col;
    const int row;
    OffsetCoord(int col_, int row_);
};



const int EVEN =  1;
const int ODD  = -1;

#endif /* LIBHEX_HPP */
