// Tests

#include <iostream>

#include "libhex.hpp"

void equal_Hex(const char* name, Hex<int> a, Hex<int> b)
{
    if ( not (a.q == b.q and a.s == b.s and a.r == b.r))
    {
        perror(name);
    }
}


void equal_offsetcoord(const char* name, OffsetCoord a, OffsetCoord b)
{
    if ( not (a.col == b.col and a.row == b.row))
    {
        perror(name);
    }
}


void equal_int(const char* name, int a, int b)
{
    if ( not (a == b))
    {
        perror(name);
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
