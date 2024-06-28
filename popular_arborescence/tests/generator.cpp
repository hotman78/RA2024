#include "constraints.hpp"
#include "xrand1.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>
using namespace std;

class Output {
 public:
  Output(string str, int mx = 10) {
    title    = str;
    num      = 0;
    this->mx = mx;
  }
  ~Output() {}
  bool next() {
    string str;
    int m = mx - 1, n = num;
    do {
      str = (char)(n % 10 + '0') + str;
      m /= 10;
      n /= 10;
    } while (m);
    str = title + str + ".in";
    ofstream ofs(str.c_str());
    ofs << ss.str();
    ss.str("");
    ++num;
    return num < mx;
  }
  stringstream ss;

 private:
  string title;
  int num, mx;
};

XRand rnd(551);

enum CaseKind {
  SMALL,
  SMALL_HACK,
  RANDOM,
  RANDOM_MAX,
};

void make_testcase(Output &output, int n, int m, int a, int b) {}
void make(Output &output, CaseKind case_kind) {
  do {
    if (case_kind == SMALL) {
      int n = rnd.NextInt(MIN_N, 30);
      int m = rnd.NextInt(n - 1, min(n * (n - 1) / 2, 100));
      output.ss << n << ' ' << m << '\n';
      // 最初に有向全域木を作成する
      vector< tuple< int, int, int > > edges;

      vector< int > tmp(n);
      for (int i = 0; i < n; ++i) tmp[i] = i;
      // この順に繋ぐ
      rnd.Shuffle(tmp.begin() + 1, tmp.end());
      set< pair< int, int > > used;
      for (int i = 0; i < n - 1; ++i) {
        // 繋がっている元となる頂点 j を選ぶ
        int j = rnd.NextInt(0, i);
        int w = rnd.NextInt(1, 10);
        used.emplace(tmp[j], tmp[i + 1]);
        edges.emplace_back(tmp[j], tmp[i + 1], w);
      }

      for (int i = n - 1; i < m; i++) {
        int s = rnd.NextInt(0, n - 1);
        int t = rnd.NextInt(0, n - 1);
        while (s == t || used.count({s, t})) {
          s = rnd.NextInt(0, n - 1);
          t = rnd.NextInt(0, n - 1);
        }
        int w = rnd.NextInt(1, 10);
        used.emplace(s, t);
        edges.emplace_back(s, t, w);
      }
      for (auto [s, t, w]: edges) {
        output.ss << s << ' ' << t << ' ' << w << '\n';
      }
    }
    if (case_kind == SMALL_HACK) {
      int n = rnd.NextInt(MIN_N, 10);
      int m = rnd.NextInt(n - 1, min(n * (n - 1) / 2, 50));
      output.ss << n << ' ' << m << '\n';
      // 最初に有向全域木を作成する
      vector< tuple< int, int, int > > edges;

      vector< int > tmp(n);
      for (int i = 0; i < n; ++i) tmp[i] = i;
      // この順に繋ぐ
      rnd.Shuffle(tmp.begin() + 1, tmp.end());
      set< pair< int, int > > used;
      for (int i = 0; i < n - 1; ++i) {
        // 繋がっている元となる頂点 j を選ぶ
        int j = rnd.NextInt(0, min(i,2));
        int w = rnd.NextInt(1, 2);
        used.emplace(tmp[j], tmp[i + 1]);
        edges.emplace_back(tmp[j], tmp[i + 1], w);
      }

      for (int i = n - 1; i < m; i++) {
        int s = rnd.NextInt(0, n - 1);
        int t = rnd.NextInt(0, n - 1);
        while (s == t || used.count({s, t})) {
          s = rnd.NextInt(0, n - 1);
          t = rnd.NextInt(0, n - 1);
        }
        int w = rnd.NextInt(1, 10);
        used.emplace(s, t);
        edges.emplace_back(s, t, w);
      }
      for (auto [s, t, w]: edges) {
        output.ss << s << ' ' << t << ' ' << w << '\n';
      }
    }
    else if (case_kind == RANDOM) {
      int n = rnd.NextInt(MIN_N, MAX_N);
      int m = rnd.NextInt(n - 1, min<int>(n * (n - 1) / 2, MAX_M));
      output.ss << n << ' ' << m << '\n';
      // 最初に有向全域木を作成する
      vector< tuple< int, int, int > > edges;

      vector< int > tmp(n);
      for (int i = 0; i < n; ++i) tmp[i] = i;
      // この順に繋ぐ
      rnd.Shuffle(tmp.begin() + 1, tmp.end());
      set< pair< int, int > > used;
      for (int i = 0; i < n - 1; ++i) {
        // 繋がっている元となる頂点 j を選ぶ
        int j = rnd.NextInt(0, i);
        int w = rnd.NextInt(MIN_W, MAX_W);
        used.emplace(tmp[j], tmp[i + 1]);
        edges.emplace_back(tmp[j], tmp[i + 1], w);
      }

      for (int i = n - 1; i < m; i++) {
        int s = rnd.NextInt(0, n - 1);
        int t = rnd.NextInt(0, n - 1);
        while (s == t || used.count({s, t})) {
          s = rnd.NextInt(0, n - 1);
          t = rnd.NextInt(0, n - 1);
        }
        int w = rnd.NextInt(MIN_W, MAX_W);
        used.emplace(s, t);
        edges.emplace_back(s, t, w);
      }
      for (auto [s, t, w]: edges) {
        output.ss << s << ' ' << t << ' ' << w << '\n';
      }
    }
    // else if(case_kind == RANDOM_MAX) {
    //     int n = MAX_N;
    //     output.ss << n << '\n';
    //     for(int i=0; i<n; i++){
    //         int a = rnd.NextInt(MIN_A, MAX_A);
    //         output.ss << a;
    //         if(i<n-1){
    //             output.ss << ' ';
    //         }
    //     }
    //     output.ss<<'\n';
    // }
  } while (output.next());
}

int main() {
  //   for (int t = 1; t <= 4; t++) {
  // string prefix = string(1, '0' + t) + "_";
  Output small("10_small_", 30);
  Output small_hack("15_small_hack_", 3);
  Output random("20_random_", 10);
  Output maxcase("30_max_", 5);
  make(small, SMALL);
  make(small_hack, SMALL_HACK);
  make(random, RANDOM);
  // make(maxcase, RANDOM_MAX);
  //   }
  return 0;
}