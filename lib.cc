// Generated code -- http://www.redblobgames.com/grids/hexagons/

// Forward declarations


Hex<int> hex_direction(int direction)
{
    return hex_directions[direction];
}


Hex<int> hex_neighbor(Hex<int> hex, int direction)
{
    return (hex + hex_direction(direction));
}



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






