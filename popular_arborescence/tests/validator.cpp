#include "constraints.hpp"
#include "testlib.h"
using namespace std;

void check(int n) {
    ensure(MIN_N <= n && n <= MAX_N);
    for (int i = 0; i < n; ++i) {
        inf.readInt(MIN_W, MAX_W);
        if (i < n - 1)
            inf.readSpace();
        else
            inf.readEoln();
    }
}

int main() {
    registerValidation();
    int n = inf.readInt(MIN_N, MAX_N);
    inf.readSpace();
    int m = inf.readInt(MIN_M, MAX_M);
    inf.readEoln();
    for (int i = 0; i < m; ++i) {
        int s = inf.readInt(0, n);
        inf.readSpace();
        int t = inf.readInt(0, n);
        inf.readSpace();
        int c = inf.readInt();
        ensure(s != t);
        inf.readEoln();
    }
    inf.readEof();
    return 0;
}