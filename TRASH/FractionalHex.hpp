struct FractionalHex {
    const double q, r, s;
    FractionalHex(double q_, double r_, double s_)
    : q(q_), r(r_), s(s_) {}
};

Hex hex_round(FractionalHex h) {
    int q = int(round(h.q));
    int r = int(round(h.r));
    int s = int(round(h.s));
    double q_diff = abs(q - h.q);
    double r_diff = abs(r - h.r);
    double s_diff = abs(s - h.s);
    if (q_diff > r_diff and q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    } else {
        s = -q - r;
    }
    return Hex(q, r, s);
}

float lerp(double a, double b, double t) {
    return a * (1-t) + b * t;
    /* better for floating point precision than
       a + (b - a) * t, which is what I usually write */
}

FractionalHex hex_lerp(Hex a, Hex b, double t) {
    return FractionalHex(lerp(a.q, b.q, t),
                         lerp(a.r, b.r, t),
                         lerp(a.s, b.s, t));
}

vector<Hex> hex_linedraw(Hex a, Hex b) {
    int N = hex_distance(a, b);
    vector<Hex> results = {};
    double step = 1.0 / max(N, 1);
    for (int i = 0; i <= N; i++) {
        results.push_back(hex_round(hex_lerp(a, b, step * i)));
    }
    return results;
}

vector<Hex> hex_linedraw(Hex a, Hex b) {
    int N = hex_distance(a, b);
    FractionalHex a_nudge(a.q + 1e-6, a.r + 1e-6, a.s - 2e-6);
    FractionalHex b_nudge(b.q + 1e-6, b.r + 1e-6, b.s - 2e-6);
    vector<Hex> results = {};
    double step = 1.0 / max(N, 1);
    for (int i = 0; i <= N; i++) {
        results.push_back(
            hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
    }
    return results;
}
