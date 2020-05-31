#include <bits/stdc++.h>
#define rep(i,n) for (int i = 0; i < n; ++i)
using namespace std;

using ll = long long;

///////////////////////////////////////////////////////////////////////////////
template<typename T>
struct BIT {
  int n;
  vector<T> d;
  int curr;
  BIT(int n=0):n(n),d(n+1) {
    curr = 1;
    while (curr < d.size()) curr <<= 1;
  }

  // i 番目の値に x を足す O(log D)
  void add(int i, T x=1) {
    for (i++; i <= n; i += i&-i) {
      d[i] += x;
    }
  }

  // ０番目からi番目までの和を求める O(log D)
  T sum(int i) {
    T x = 0;
    for (i++; i; i -= i&-i) {
      x += d[i];
    }
    return x;
  }

  // [l, r) の和を求める
  T sum(int l, int r) {
    return sum(r-1) - sum(l-1);
  }

  // BIT上の二分探索 bit.sum(v) >= w となる最小の v を求める O(log D)
  int lower_bound(T w) {
    int i = 0;
    for(int k = curr; k > 0; k >>= 1) {
      if(i + k < d.size() && d[i + k] < w) {
        w -= d[i + k];
        i += k;
      }
    }
    return i;
  }
};
///////////////////////////////////////////////////////////////////////////////

// Range Sum Query https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_B
void aoj_dsl_2_b() {
  int n, q;
  cin >> n >> q;
  BIT<ll> bit(n);

  rep(i, q) {
    int t, x, y; cin >> t >> x >> y;
    if (t == 0) {       // a[x-1] に y を加算する
      bit.add(x-1, y);
    } else {            // a[x-1] ... a[y-1] の和を求める
      cout << bit.sum(x-1, y) << endl;;
    }
  }
}


// https://judge.yosupo.jp/problem/static_range_sum
void static_range_sum() {
  int n, q; cin >> n >> q;
  BIT<ll> bit(n);
  rep(i, n) {
    int a; cin >> a;
    bit.add(i, a);
  }

  rep(i, q) {
    int l, r; cin >> l >> r;
    ll ans = bit.sum(r-1);
    if (l > 0) ans -= bit.sum(l-1);
    cout << ans << endl;
  }
}

// https://judge.yosupo.jp/problem/point_add_range_sum
void point_add_range_sum() {
  int n, q;
  cin >> n >> q;
  BIT<ll> bit(n);
  rep(i, n) {
    int a;
    cin >> a;
    bit.add(i, a);
  }

  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      int p, x; cin >> p >> x;
      bit.add(p, x);
    } else {
      int l, r;
      cin >> l >> r;
      cout << bit.sum(l, r) << endl;
    }
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // aoj_dsl_2_b();
  // static_range_sum();
  point_add_range_sum();

  return 0;
}
