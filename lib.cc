// Generated code -- http://www.redblobgames.com/grids/hexagons/

// Forward declarations


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

