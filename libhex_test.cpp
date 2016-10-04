// Tests

#include <iostream>

#include "libhex.hpp"

void equal_Hex(const char* name, Hex a, Hex b)
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


void equal_hex_array(const char* name, vector<Hex> a, vector<Hex> b)
{
    equal_int(name, a.size(), b.size());
    for (unsigned int i = 0; i < a.size(); i++)
    {
        equal_Hex(name, a[i], b[i]);
    }
}


void test_hex_arithmetic()
{
    equal_Hex("hex_add", Hex(4, -10, 6), (Hex(1, -3, 2) + Hex(3, -7, 4)));
    equal_Hex("hex_sub", Hex(-2, 4, -2), (Hex(1, -3, 2) - Hex(3, -7, 4)));
    equal_Hex("hex_mul", Hex(-3, 9, -6), (Hex(1, -3, 2) * -3));
}


void test_hex_direction()
{
    equal_Hex("hex_direction", Hex(0, -1, 1), Hex::direction(2));
}


void test_hex_neighbor()
{
    equal_Hex("hex_neighbor", Hex(1, -3, 2), Hex(1, -2, 1).neighbor(2));
}


void test_hex_diagonal()
{
    equal_Hex("hex_diagonal", Hex(-1, -1, 2), Hex(1, -2, 1).diagonal_neighbor(3));
}


void test_hex_distance()
{
    equal_int("hex_distance", 7, Hex(3, -7, 4).distance(Hex(0, 0, 0)));
}


void test_hex_round()
{
    FractionalHex a = FractionalHex(0, 0, 0);
    FractionalHex b = FractionalHex(1, -1, 0);
    FractionalHex c = FractionalHex(0, -1, 1);
    equal_Hex("hex_round 1", Hex(5, -10, 5), 
        FractionalHex(0, 0, 0).lerp(FractionalHex(10, -20, 10), 0.5).round2hex() );
    equal_Hex("hex_round 2", a.round2hex(), (a.lerp(b, 0.499)).round2hex());
    equal_Hex("hex_round 3", b.round2hex(), (a.lerp(b, 0.501)).round2hex());
    equal_Hex("hex_round 4", a.round2hex(), (FractionalHex(a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3)).round2hex());
    equal_Hex("hex_round 5", c.round2hex(), (FractionalHex(a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4)).round2hex());
}


void test_hex_linedraw()
{
    equal_hex_array("hex_linedraw", {Hex(0, 0, 0), Hex(0, -1, 1), Hex(0, -2, 2), Hex(1, -3, 2), Hex(1, -4, 3), Hex(1, -5, 4)}, Hex(0, 0, 0).drawLine(Hex(1, -5, 4)));
}


void test_layout()
{
    Hex h = Hex(3, 4, -7);
    Layout flat = Layout(Layout::flat, Point(10, 15), Point(35, 71));
    equal_Hex("layout", h, h.point(flat).hex(flat).round2hex());
    Layout pointy = Layout(Layout::pointy, Point(10, 15), Point(35, 71));
    equal_Hex("layout", h, h.point(pointy).hex(pointy).round2hex() );
}


void test_conversion_roundtrip()
{
    Hex a = Hex(3, 4, -7);
    OffsetCoord b = OffsetCoord(1, -3);
    equal_Hex("conversion_roundtrip even-q",         a, a.hex2qoffset(EVEN).qoffset2hex(EVEN) );
    equal_offsetcoord("conversion_roundtrip even-q", b, b.qoffset2hex(EVEN).hex2qoffset(EVEN) );
    equal_Hex("conversion_roundtrip odd-q",          a, a.hex2qoffset(ODD).qoffset2hex(ODD) );
    equal_offsetcoord("conversion_roundtrip odd-q",  b, b.qoffset2hex(ODD).hex2qoffset(ODD) );
    equal_Hex("conversion_roundtrip even-r",         a, a.hex2roffset(EVEN).roffset2hex(EVEN) );
    equal_offsetcoord("conversion_roundtrip even-r", b, b.roffset2hex(EVEN).hex2roffset(EVEN) );
    equal_Hex("conversion_roundtrip odd-r",          a, a.hex2roffset(ODD).roffset2hex(ODD) );
    equal_offsetcoord("conversion_roundtrip odd-r",  b, b.roffset2hex(ODD).hex2roffset(ODD) );
}


void test_offset_from_cube()
{
    equal_offsetcoord("offset_from_cube even-q", OffsetCoord(1, 3), Hex(1, 2, -3).hex2qoffset(EVEN) );
    equal_offsetcoord("offset_from_cube odd-q",  OffsetCoord(1, 2), Hex(1, 2, -3).hex2qoffset(ODD)  );
}


void test_offset_to_cube()
{
    equal_Hex("offset_to_cube even-", Hex(1, 2, -3), OffsetCoord(1, 3).qoffset2hex(EVEN) );
    equal_Hex("offset_to_cube odd-q", Hex(1, 2, -3), OffsetCoord(1, 2).qoffset2hex(ODD)  );
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
