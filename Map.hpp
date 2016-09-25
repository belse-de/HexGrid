namespace std {
    template <> struct hash<Hex> {
        size_t operator()(const Hex& h) const {
            hash<int> int_hash;
            size_t hq = int_hash(h.q);
            size_t hr = int_hash(h.r);
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}

// parallelogram
unordered_set<Hex> map;
for (int q = q1; q <= q2; q++) {
    for (int r = r1; r <= r2; r++) {
        map.insert(Hex(q, r, -q-r)));
    }
}
// triangle
unordered_set<Hex> map;
for (int q = 0; q <= map_size; q++) {
    for (int r = 0; r <= map_size - q; r++) {
        map.insert(Hex(q, r, -q-r));
    }
}
// triangle
unordered_set<Hex> map;
for (int q = 0; q <= map_size; q++) {
    for (int r = map_size - q; r <= map_size; r++) {
        map.insert(Hex(q, r, -q-r));
    }
}

//hex
unordered_set<Hex> map;
for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = max(-map_radius, -q - map_radius);
    int r2 = min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
        map.insert(Hex(q, r, -q-r));
    }
}
