// Generated code -- http://www.redblobgames.com/grids/hexagons/

#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

using std::abs;
using std::max;
using std::vector;


struct Point
{
    const double x;
    const double y;
    Point(double x_, double y_): x(x_), y(y_) {}
};

template <typename T>
struct Hex
{
    const T q;
    const T r;
    const T s;
    Hex(T q_, T r_, T s_): q(q_), r(r_), s(s_) {}

    Hex operator+ (const Hex &rhs) const {
        return Hex(q + rhs.q, r + rhs.r, s + rhs.s);
    }
    Hex operator- (const Hex &rhs) const
    {
        return Hex(q - rhs.q, r - rhs.r, s - rhs.s);
    }
    Hex operator* (T rhs) const
    {
        return Hex(q * rhs, r * rhs, s * rhs);
    }
    bool operator == (const Hex &rhs) const
    {
        return q == rhs.q && r == rhs.r && s == rhs.s;
    }
    bool operator != (const Hex &rhs) const
    {
        return !(*this == rhs);
    }
    
    
    int length()
    {
        return int((abs(q) + abs(r) + abs(s)) / 2);
    }
    int distance(const Hex &rhs) const
    {
        return (*this - rhs).length();
    }
    
};




struct OffsetCoord
{
    const int col;
    const int row;
    OffsetCoord(int col_, int row_): col(col_), row(row_) {}
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
    Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double start_angle_): f0(f0_), f1(f1_), f2(f2_), f3(f3_), b0(b0_), b1(b1_), b2(b2_), b3(b3_), start_angle(start_angle_) {}
};


struct Layout
{
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_): orientation(orientation_), size(size_), origin(origin_) {}
};


// Forward declarations

const vector<Hex<int>> hex_directions = {Hex<int>(1, 0, -1), Hex<int>(1, -1, 0), Hex<int>(0, -1, 1), Hex<int>(-1, 0, 1), Hex<int>(-1, 1, 0), Hex<int>(0, 1, -1)};
Hex<int> hex_direction(int direction)
{
    return hex_directions[direction];
}


Hex<int> hex_neighbor(Hex<int> hex, int direction)
{
    return (hex + hex_direction(direction));
}


const vector<Hex<int>> hex_diagonals = {Hex<int>(2, -1, -1), Hex<int>(1, -2, 1), Hex<int>(-1, -1, 2), Hex<int>(-2, 1, 1), Hex<int>(-1, 2, -1), Hex<int>(1, 1, -2)};
Hex<int> hex_diagonal_neighbor(Hex<int> hex, int direction)
{
    return (hex + hex_diagonals[direction]);
}






Hex<int> hex_round(Hex<double> h)
{
    int q = int(round(h.q));
    int r = int(round(h.r));
    int s = int(round(h.s));
    double q_diff = abs(q - h.q);
    double r_diff = abs(r - h.r);
    double s_diff = abs(s - h.s);
    if (q_diff > r_diff and q_diff > s_diff)
    {
        q = -r - s;
    }
    else
        if (r_diff > s_diff)
        {
            r = -q - s;
        }
        else
        {
            s = -q - r;
        }
    return Hex<int>(q, r, s);
}


Hex<double> hex_lerp(Hex<double> a, Hex<double> b, double t)
{
    return Hex<double>(a.q * (1 - t) + b.q * t, a.r * (1 - t) + b.r * t, a.s * (1 - t) + b.s * t);
}


vector<Hex<int>> hex_linedraw(Hex<int> a, Hex<int> b)
{
    int N = a.distance(b);
    Hex<double> a_nudge = Hex<double>(a.q + 0.000001, a.r + 0.000001, a.s - 0.000002);
    Hex<double> b_nudge = Hex<double>(b.q + 0.000001, b.r + 0.000001, b.s - 0.000002);
    vector<Hex<int>> results = {};
    double step = 1.0 / max(N, 1);
    for (int i = 0; i <= N; i++)
    {
        results.push_back(hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
    }
    return results;
}



const int EVEN = 1;
const int ODD = -1;
OffsetCoord qoffset_from_cube(int offset, Hex<int> h)
{
    int col = h.q;
    int row = h.r + int((h.q + offset * (h.q & 1)) / 2);
    return OffsetCoord(col, row);
}


Hex<int> qoffset_to_cube(int offset, OffsetCoord h)
{
    int q = h.col;
    int r = h.row - int((h.col + offset * (h.col & 1)) / 2);
    int s = -q - r;
    return Hex<int>(q, r, s);
}


OffsetCoord roffset_from_cube(int offset, Hex<int> h)
{
    int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
    int row = h.r;
    return OffsetCoord(col, row);
}


Hex<int> roffset_to_cube(int offset, OffsetCoord h)
{
    int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
    int r = h.row;
    int s = -q - r;
    return Hex<int>(q, r, s);
}




const Orientation layout_pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const Orientation layout_flat = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);
Point hex_to_pixel(Layout layout, Hex<int> h)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    double x = (M.f0 * h.q + M.f1 * h.r) * size.x;
    double y = (M.f2 * h.q + M.f3 * h.r) * size.y;
    return Point(x + origin.x, y + origin.y);
}


Hex<double> pixel_to_Hex(Layout layout, Point p)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    Point pt = Point((p.x - origin.x) / size.x, (p.y - origin.y) / size.y);
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;
    return Hex<double>(q, r, -q - r);
}


Point hex_corner_offset(Layout layout, int corner)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    double angle = 2.0 * M_PI * (M.start_angle - corner) / 6;
    return Point(size.x * cos(angle), size.y * sin(angle));
}


vector<Point> polygon_corners(Layout layout, Hex<int> h)
{
    vector<Point> corners = {};
    Point center = hex_to_pixel(layout, h);
    for (int i = 0; i < 6; i++)
    {
        Point offset = hex_corner_offset(layout, i);
        corners.push_back(Point(center.x + offset.x, center.y + offset.y));
    }
    return corners;
}




// Tests

#include <iostream>

void complain(const char* name) 
{
  std::cout << "FAIL " << name << std::endl;
}


void equal_Hex(const char* name, Hex<int> a, Hex<int> b)
{
    if (!(a.q == b.q and a.s == b.s and a.r == b.r))
    {
        complain(name);
    }
}


void equal_offsetcoord(const char* name, OffsetCoord a, OffsetCoord b)
{
    if (!(a.col == b.col and a.row == b.row))
    {
        complain(name);
    }
}


void equal_int(const char* name, int a, int b)
{
    if (!(a == b))
    {
        complain(name);
    }
}


void equal_hex_array(const char* name, vector<Hex<int>> a, vector<Hex<int>> b)
{
    equal_int(name, a.size(), b.size());
    for (int i = 0; i < a.size(); i++)
    {
        equal_Hex(name, a[i], b[i]);
    }
}


void test_hex_arithmetic()
{
    equal_Hex("hex_add", Hex<int>(4, -10, 6), (Hex<int>(1, -3, 2) + Hex<int>(3, -7, 4)));
    equal_Hex("hex_subtract", Hex<int>(-2, 4, -2), (Hex<int>(1, -3, 2) - Hex<int>(3, -7, 4)));
    equal_Hex("hex_subtract", Hex<int>(-3, 9, -6), (Hex<int>(1, -3, 2) * -3));
}


void test_hex_direction()
{
    equal_Hex("hex_direction", Hex<int>(0, -1, 1), hex_direction(2));
}


void test_hex_neighbor()
{
    equal_Hex("hex_neighbor", Hex<int>(1, -3, 2), hex_neighbor(Hex<int>(1, -2, 1), 2));
}


void test_hex_diagonal()
{
    equal_Hex("hex_diagonal", Hex<int>(-1, -1, 2), hex_diagonal_neighbor(Hex<int>(1, -2, 1), 3));
}


void test_hex_distance()
{
    equal_int("hex_distance", 7, Hex<int>(3, -7, 4).distance(Hex<int>(0, 0, 0)));
}


void test_hex_round()
{
    Hex<double> a = Hex<double>(0, 0, 0);
    Hex<double> b = Hex<double>(1, -1, 0);
    Hex<double> c = Hex<double>(0, -1, 1);
    equal_Hex("hex_round 1", Hex<int>(5, -10, 5), hex_round(hex_lerp(Hex<double>(0, 0, 0), Hex<double>(10, -20, 10), 0.5)));
    equal_Hex("hex_round 2", hex_round(a), hex_round(hex_lerp(a, b, 0.499)));
    equal_Hex("hex_round 3", hex_round(b), hex_round(hex_lerp(a, b, 0.501)));
    equal_Hex("hex_round 4", hex_round(a), hex_round(Hex<double>(a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3)));
    equal_Hex("hex_round 5", hex_round(c), hex_round(Hex<double>(a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4)));
}


void test_hex_linedraw()
{
    equal_hex_array("hex_linedraw", {Hex<int>(0, 0, 0), Hex<int>(0, -1, 1), Hex<int>(0, -2, 2), Hex<int>(1, -3, 2), Hex<int>(1, -4, 3), Hex<int>(1, -5, 4)}, hex_linedraw(Hex<int>(0, 0, 0), Hex<int>(1, -5, 4)));
}


void test_layout()
{
    Hex<int> h = Hex<int>(3, 4, -7);
    Layout flat = Layout(layout_flat, Point(10, 15), Point(35, 71));
    equal_Hex("layout", h, hex_round(pixel_to_Hex(flat, hex_to_pixel(flat, h))));
    Layout pointy = Layout(layout_pointy, Point(10, 15), Point(35, 71));
    equal_Hex("layout", h, hex_round(pixel_to_Hex(pointy, hex_to_pixel(pointy, h))));
}


void test_conversion_roundtrip()
{
    Hex<int> a = Hex<int>(3, 4, -7);
    OffsetCoord b = OffsetCoord(1, -3);
    equal_Hex("conversion_roundtrip even-q", a, qoffset_to_cube(EVEN, qoffset_from_cube(EVEN, a)));
    equal_offsetcoord("conversion_roundtrip even-q", b, qoffset_from_cube(EVEN, qoffset_to_cube(EVEN, b)));
    equal_Hex("conversion_roundtrip odd-q", a, qoffset_to_cube(ODD, qoffset_from_cube(ODD, a)));
    equal_offsetcoord("conversion_roundtrip odd-q", b, qoffset_from_cube(ODD, qoffset_to_cube(ODD, b)));
    equal_Hex("conversion_roundtrip even-r", a, roffset_to_cube(EVEN, roffset_from_cube(EVEN, a)));
    equal_offsetcoord("conversion_roundtrip even-r", b, roffset_from_cube(EVEN, roffset_to_cube(EVEN, b)));
    equal_Hex("conversion_roundtrip odd-r", a, roffset_to_cube(ODD, roffset_from_cube(ODD, a)));
    equal_offsetcoord("conversion_roundtrip odd-r", b, roffset_from_cube(ODD, roffset_to_cube(ODD, b)));
}


void test_offset_from_cube()
{
    equal_offsetcoord("offset_from_cube even-q", OffsetCoord(1, 3), qoffset_from_cube(EVEN, Hex<int>(1, 2, -3)));
    equal_offsetcoord("offset_from_cube odd-q", OffsetCoord(1, 2), qoffset_from_cube(ODD, Hex<int>(1, 2, -3)));
}


void test_offset_to_cube()
{
    equal_Hex("offset_to_cube even-", Hex<int>(1, 2, -3), qoffset_to_cube(EVEN, OffsetCoord(1, 3)));
    equal_Hex("offset_to_cube odd-q", Hex<int>(1, 2, -3), qoffset_to_cube(ODD, OffsetCoord(1, 2)));
}


void test_all()
{
    test_hex_arithmetic();
    test_hex_direction();
    test_hex_neighbor();
    test_hex_diagonal();
    test_hex_distance();
    test_hex_round();
    test_hex_linedraw();
    test_layout();
    test_conversion_roundtrip();
    test_offset_from_cube();
    test_offset_to_cube();
}





int main() {
  test_all();
}

