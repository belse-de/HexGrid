#pragma once
#ifndef LIBHEX_HPP
#define LIBHEX_HPP

struct Point
{
    const double x;
    const double y;
    Point(double x_, double y_);
};

template <typename T>
struct Hex
{
    const T q;
    const T r;
    const T s;
    Hex(T q_, T r_, T s_);

    Hex operator+ (const Hex &rhs) const,
    Hex operator- (const Hex &rhs) const;
    Hex operator* (T rhs) const,
    bool operator == (const Hex &rhs) const;
    bool operator != (const Hex &rhs) const;
      
    int length();
    int distance(const Hex &rhs) const;
};

struct OffsetCoord
{
    const int col;
    const int row;
    OffsetCoord(int col_, int row_);
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
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_);
};

static const vector<Hex<int>> hex_directions = 
{
    Hex<int>( 1,  0, -1), 
    Hex<int>( 1, -1,  0), 
    Hex<int>( 0, -1,  1), 
    Hex<int>(-1,  0,  1), 
    Hex<int>(-1,  1,  0), 
    Hex<int>( 0,  1, -1)
};

static const vector<Hex<int>> hex_diagonals = 
{
    Hex<int>( 2, -1, -1), 
    Hex<int>( 1, -2,  1), 
    Hex<int>(-1, -1,  2), 
    Hex<int>(-2,  1,  1), 
    Hex<int>(-1,  2, -1), 
    Hex<int>( 1,  1, -2)
};

#endif /* LIBHEX_HPP */
