#include <bits/stdc++.h>
using namespace std;
#include "./union_find.hpp"

#include <utility>
using lint = long long;
#define rep(i, n) for (lint i = 0; i < (lint)(n); i++)

constexpr lint SZ = 1000;
int rank(int n, set< pair< int, int > > v) {
    vector< char > used(n);
    UF uf(n);
    int res = 0;
    for (auto [s, t]: v) {
        // if (used[t]) continue;
        if (uf.unite(s, t)) {
            used[t] = 1;
            ++res;
        }
    }
    return res;
}

set< pair< int, int > > span(int n, set< pair< int, int > > v,
                             set< pair< int, int > > E) {
    vector< char > used(n);
    UF uf(n);
    set< pair< int, int > > res;
    auto intersect = [&](set< pair< int, int > > s,
                         set< pair< int, int > > t) {
        set< pair< int, int > > res;
        std::set_intersection(s.begin(), s.end(), t.begin(), t.end(),
                              std::inserter(res, res.end()));
        return res;
    };
    auto sz = ::rank(n, v);
    for (auto [s, t]: E) {
        if (v.count(make_pair(s, t))) {
            res.emplace(make_pair(s, t));
            continue;
        }
        v.emplace(make_pair(s, t));
        if (::rank(n, v) == sz) {
            res.emplace(make_pair(s, t));
        }
        v.erase(make_pair(s, t));
    }
    return res;
}

// comp: means <
set< pair< int, int > > popular_arborescence(
    int n, set< pair< int, int > > edges,
    function< bool(pair< int, int >, pair< int, int >) > comp) {
    vector< vector< int > > g(n);
    for (auto [s, t]: edges) {
        g[t].emplace_back(s);
    }
    auto intersect = [&](set< pair< int, int > > s,
                         set< pair< int, int > > t) {
        set< pair< int, int > > res;
        std::set_intersection(s.begin(), s.end(), t.begin(), t.end(),
                              std::inserter(res, res.end()));
        return res;
    };
    auto diff = [&](set< pair< int, int > > s,
                    set< pair< int, int > > t) {
        set< pair< int, int > > res;
        std::set_difference(s.begin(), s.end(), t.begin(), t.end(),
                            std::inserter(res, res.end()));
        return res;
    };
    int p = 1;
    vector< set< pair< int, int > > > C;
    C.emplace_back(edges);
    auto Compute_E_C = [&]() {
        vector< int > mx(n, -1);
        vector< set< pair< int, int > > > mx1(n);
        vector< set< pair< int, int > > > mx2(n);
        vector< vector< pair< int, int > > > ed(
            n, vector< pair< int, int > >(C.size()));
        rep(i, C.size()) {
            auto c = C[i];
            if (i) {
                c = diff(c, C[i - 1]);
            }
            for (auto [s, t]: c) {
                // 一番外側
                mx[t] = i;
            }
        }
        rep(i, C.size()) {
            auto c = C[i];
            if (i) {
                c = diff(c, C[i - 1]);
            }
            for (auto [s, t]: c) {
                if (mx[t] == i) {
                    mx1[t].emplace(make_pair(s, t));
                }
                if (mx[t] == i + 1) {
                    mx2[t].emplace(make_pair(s, t));
                }
            }
        }
        set< tuple< int, int, int > > E;
        rep(i, n) {
            // 一番外側
            for (auto e: mx1[i]) {
                bool ok = 1;
                for (auto e2: mx1[i]) {
                    if (comp(e, e2)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    E.emplace(mx[i], e.first, e.second);
                }
            }
            // 次に外側
            for (auto e: mx2[i]) {
                bool ok = 1;
                for (auto e2: mx2[i]) {
                    if (comp(e, e2)) {
                        ok = 0;
                        break;
                    }
                }
                for (auto f: mx1[i]) {
                    if (!comp(f, e)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    E.emplace(mx[i] - 1, e.first, e.second);
                }
            }
        }
        return E;
    };
    auto Compute_I = [&](set< tuple< int, int, int > > E) {
        set< pair< int, int > > I;
        vector< char > used(n);
        UF uf(n);
        for (auto [idx, s, t]: E) {
            if (used[t]) continue;
            if (uf.unite(s, t)) {
                used[t] = 1;
                // cerr<<idx<<" "<<s<<" "<<t<<endl;
                I.emplace(s, t);
            }
        }
        // cerr<<endl;
        return I;
    };
    while (p < n) {
        // Compute E(C)
        auto E = Compute_E_C();
        // Find branching I
        auto I = Compute_I(E);
        // for(auto [a,b]:I)cerr<<a<<b<<endl;
        // cerr << endl;
        int k = -1;
        rep(i, C.size()) {
            // cerr<<intersect(C[i],I).size()<<" "<<::rank(C[i])<<endl;
            if (intersect(C[i], I).size() < ::rank(n, C[i])) {
                k = i;
                break;
            }
        }
        if (k == -1) {
            return I;
        }
        C[k] = ::span(n, intersect(I, C[k]), edges);
        cerr << C[k].size() << endl;
        // cerr<<k<<endl;
        // for(auto e:I)cerr<<e.first<<" "<<e.second<<",";cerr<<endl;
        // cerr<<::rank(n, C[k])<<endl;
        if (k + 1 == p) {
            p++;
            C.emplace_back(edges);
        }
    }
    cerr << "Not Found" << endl;
    return set< pair< int, int > >();
}
int main() {
    int n, m;
    cin >> n >> m;
    set< pair< int, int > > edges;
    map< pair< int, int >, int > weight;
    rep(i, m) {
        int s, t, w;
        cin >> s >> t >> w;
        // 根に向かう辺は無視
        if(t==0)continue;
        edges.emplace(s, t);
        weight[make_pair(s, t)] = w;
    }
    auto comp = [&](pair< int, int > a, pair< int, int > b) {
        return weight[a] > weight[b];
    };
    auto res = popular_arborescence(n, edges, comp);
    for (auto [s, t]: res) {
        cout << s << " " << t << endl;
    }
}