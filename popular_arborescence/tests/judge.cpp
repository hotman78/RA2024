#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

using S = tuple< long long, int, int, long long >;
using E = long long;
S operator+(S s, E t) {
    return make_tuple(get< 0 >(s) + t, get< 1 >(s), get< 2 >(s),
                      get< 3 >(s));
}
S operator+=(S &s, E t) {
    s = s + t;
    return s;
}

S operator+(S s, S t) {
    return make_tuple(
        get< 0 >(s) + get< 0 >(t), get< 1 >(s) + get< 1 >(t),
        get< 2 >(s) + get< 2 >(t), get< 3 >(s) + get< 3 >(t));
}
S operator+=(S &s, S t) {
    s = s + t;
    return s;
}

class UF {
   public:
    vector< int > data;
    int sz;

   public:
    UF(int sz): sz(sz) {
        data.resize(sz, -1);
    }
    bool unite(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return 0;
        if (data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        sz--;
        return 1;
    }
    inline int root(int x) {
        return data[x] < 0 ? x : data[x] = root(data[x]);
    }
    inline bool same(int x, int y) {
        return root(x) == root(y);
    }
    inline int size() {
        return sz;
    }
    inline int size(int x) {
        return -data[root(x)];
    }
};

template < typename T, typename E, bool isMin = true >
struct skew_heap {
    struct Node {
        T key;
        E laz;
        Node *l, *r;
        int idx;
        Node() = default;
        Node(const T &k, int i = -1)
            : key(k),
              laz(0),
              l(nullptr),
              r(nullptr),
              idx(i) {}
    };
    using Ptr = Node *;

    static void propagate(Ptr x) {
        if (x->laz == 0) return;
        if (x->l) x->l->laz += x->laz;
        if (x->r) x->r->laz += x->laz;
        x->key += x->laz;
        x->laz = 0;
    }

    static Ptr meld(Ptr x, Ptr y) {
        if (!x || !y) return x ? x : y;
        if (!comp(x, y)) swap(x, y);
        propagate(x);
        x->r = meld(x->r, y);
        swap(x->l, x->r);
        return x;
    }

    static Ptr alloc(const T &key, int idx = -1) {
        return new Node(key, idx);
    }

    static Ptr pop(Ptr x) {
        propagate(x);
        return meld(x->l, x->r);
    }

    static Ptr push(Ptr x, const T &key, int idx = -1) {
        return meld(x, alloc(key, idx));
    }

    static void apply(Ptr x, const E &laz) {
        x->laz += laz;
        propagate(x);
    }

   private:
    static inline bool comp(Ptr x, Ptr y) {
        if constexpr (isMin) {
            return x->key + x->laz < y->key + y->laz;
        } else {
            return x->key + x->laz > y->key + y->laz;
        }
    }
};

int directedMST(int n, int m, int root,
                vector< tuple< int, int, int > > edges) {
    using heap = skew_heap< S, E >;
    using Ptr  = heap::Ptr;
    vector< Ptr > g(n, nullptr);
    UF uf(n);
    for (auto [s, t, c]: edges) {
        g[t] = heap::push(g[t], make_tuple(c, t, s, c));
    }
    set< int > not_used;
    for (int i = 0; i < n; ++i) {
        // 根以外の頂点を追加
        if (i != root) not_used.emplace(i);
    }
    // 各頂点について入ってくる辺
    vector< Ptr > ans(n, nullptr);
    while (!not_used.empty()) {
        // スタート地点
        int now = *not_used.begin();
        // cerr<<now<<endl;

        // 今考えているpath に含まれるかどうか
        set< int > connected;
        // path の履歴
        vector< Ptr > histry;
        vector< int > eraselist;
        while (not_used.count(now)) {
            connected.emplace(now);
            auto nxt = g[uf.root(now)];
            // cerr<<now<<" "<<get<2>(nxt->key)<<endl;

            // 使用した辺をpop
            g[uf.root(now)] = heap::pop(g[uf.root(now)]);

            // 自己ループは使わない（縮約によって自己ループが生まれうる）
            if (uf.same(now, get< 2 >(nxt->key))) {
                continue;
            }

            // 辺の追加
            {
                // ans の更新
                ans[get< 1 >(nxt->key)] = nxt;
                // 履歴の追加
                histry.emplace_back(nxt);
                // 現在の頂点を変更
                now = get< 2 >(nxt->key);
            }

            // サイクルを検出
            if (connected.count(now)) {
                Ptr newnode = nullptr;
                vector< pair< int, int > > edge_list;
                while (!histry.empty() &&
                       (histry.back() == nxt ||
                        get< 2 >(histry.back()->key) !=
                            get< 2 >(nxt->key))) {
                    const int from = get< 1 >(histry.back()->key);
                    const int to   = get< 2 >(histry.back()->key);
                    const long long cost =
                        get< 0 >(histry.back()->key);

                    if (g[uf.root(from)] != nullptr) {
                        // 縮約時の下駄
                        heap::apply(g[uf.root(from)], -cost);
                    }
                    // cerr<<g[from]<<" "<<from<<" "<<to<<endl;

                    newnode = heap::meld(newnode, g[uf.root(from)]);
                    edge_list.emplace_back(from, to);
                    // さらに縮約する場合を考え，縮約した辺は履歴に残さない
                    histry.pop_back();
                }
                // 縮約の状態は UF で管理
                for (auto [from, to]: edge_list) {
                    uf.unite(from, to);
                }

                g[uf.root(now)] = newnode;
            }
        }
        for (auto e: connected) {
            if (not_used.count(e)) {
                not_used.erase(e);
            }
        }
    }
    long long ans_w = 0;
    vector< int > ans_e(n, root);
    for (int i = 0; i < n; ++i) {
        if (i == root) continue;
        // cerr<<ans[i]<<endl;
        ans_w += get< 3 >(ans[i]->key);
    }
    return ans_w;
}

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);

    int n = inf.readInt();
    inf.readSpace();
    int m = inf.readInt();
    inf.readEoln();
    set< pair< int, int > > edges;
    map< pair< int, int >, int > weight;
    vector< vector< int > > g(n);
    vector< tuple< int, int, int > > edges2;

    for (int i = 0; i < m; ++i) {
        int s = inf.readInt();
        inf.readSpace();
        int t = inf.readInt();
        inf.readSpace();
        int w = inf.readInt();
        inf.readEoln();
        edges.emplace(s, t);
        g[t].emplace_back(s);
        weight[make_pair(s, t)] = w;
    }
    inf.readEof();
    auto comp = [&](pair< int, int > a, pair< int, int > b) {
        return weight[a] > weight[b];
    };
    vector< int > tree(n, -1);
    UF uf(n);
    for (int i = 0; i < n - 1; ++i) {
        // 得られた出力
        int s = ouf.readInt();
        ouf.readSpace();
        int t = ouf.readInt();
        ouf.readEoln();
        if (!weight.count(make_pair(s, t))) {
            quitf(_wa,
                  ("edge(" + to_string(s) + "," + to_string(s) +
                   ")is not exist.")
                      .c_str());
        }
        tree[t] = s;
        uf.unite(s, t);
    }
    if(uf.size()!=1){
        quitf(_wa, ("Output is not connected"));
    }
    if(count(tree.begin()+1, tree.end(), -1)){
        quitf(_wa, ("Output is not arborescence"));
    }
    for (int t = 0; t < n; ++t) {
        for (auto e: g[t]) {
            int s = tree[t];
            if (comp(make_pair(s, t), make_pair(e, t))) {
                edges2.emplace_back(e, t, -1);
            } else if (comp(make_pair(e, t), make_pair(s, t))) {
                edges2.emplace_back(e, t, 1);
            } else {
                edges2.emplace_back(e, t, 0);
            }
        }
    }

    ouf.readEof();
    auto res = directedMST(n, m, 0, edges2);
    if (res < 0) {
        quitf(_wa,
              ("res: " + to_string(res) + " is not popular").c_str());
    }
    // cerr<<res<<endl;
    quitf(_ok, ("res: " + to_string(res) + "OK").c_str());
}