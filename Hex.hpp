
struct Hex 
{
     // Cube storage
    const int q, r, s;
    const int v[3];
    
    // cube constructor
    Hex(int q_, int r_, int s_): q(q_), r(r_), s(s_), v{q_, r_, s_} 
    {
        assert (q + r + s == 0);
    }
    
    //  axial constructor
    Hex(int q_, int r_): q(q_), r(r_), s(-q_ - r_), v{q_, r_, s_} 
    {
        assert (q + r + s == 0);
    }
    
    
    static const vector<Hex> directions = {
        Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), 
        Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)
    };

    static Hex direction(int direction /* 0 to 5 */) {
        direction = (6 + (direction % 6)) % 6;
        assert (0 <= direction && direction < 6);
        return directions[direction];
    }

    
    
    bool operator == (Hex a, Hex b) 
    {
        return a.q == b.q && a.r == b.r && a.s == b.s;
    }

    bool operator != (Hex a, Hex b) 
    {
        return !(a == b);
    }
    
    Hex operator+ (Hex &rhs) 
    {
        return Hex(q + rhs.q, r + rhs.r, s + rhs.s);
    }

    Hex  operator- (Hex &rhs) 
    {
        return Hex(q - rhs.q, r - rhs.r, s - rhs.s);
    }

    Hex  operator* (int rhs) 
    {
        return Hex(q * rhs, r * rhs, s * rhs);
    }
    
    int length() {
        return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
    }

    int distance(Hex &rhs)  {
        return (*this - rhs).length();
    }
    
    Hex neighbor(int direction) {
        return (*this + hex_direction(direction));
    }
    
    Point to_pixel(Layout layout) {
        const Orientation& M = layout.orientation;
        double x = (M.f0 * q + M.f1 * r) * layout.size.x;
        double y = (M.f2 * q + M.f3 * r) * layout.size.y;
        return Point(x + layout.origin.x, y + layout.origin.y);
    }
    
    vector<Point> polygon_corners(Layout layout) {
        vector<Point> corners = {};
        Point center = to_pixel(layout);
        for (int i = 0; i < 6; i++) {
            Point offset = hex_corner_offset(layout, i);
            corners.push_back(Point(center.x + offset.x, 
                                    center.y + offset.y));
        }
        return corners;
    }
};

    
FractionalHex pixel_to_hex(Layout layout, Point p) {
{
    const Orientation& M = layout.orientation;
    Point pt = Point((p.x - layout.origin.x) / layout.size.x, 
                     (p.y - layout.origin.y) / layout.size.y);
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;
    return FractionalHex(q, r, -q - r);
}

Point hex_corner_offset(Layout layout, int corner) {
    Point size = layout.size;
    double angle = 2.0 * M_PI * 
             (layout.orientation.start_angle + corner) / 6;
    return Point(size.x * cos(angle), size.y * sin(angle));
}


