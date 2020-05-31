#include <bits/stdc++.h>
#define rep(i,n) for (int i = 0; i < n; ++i)
using namespace std;

using ll = long long;
using P = pair<ll, ll>;
constexpr int INF = 1e9;
constexpr int mod = 998244353;

template <typename T>
class SegmentTree {
  using Fx = function<T(T, T)>;
  Fx fx;
  const T init;
  int n;             // 最下段のノード数
  vector<T> node;

 public:
  SegmentTree(const vector<T>& v, Fx f, T ini) : fx(f), init(ini) {
    n = 1;
    while (n < v.size()) n <<= 1;  // N: 最下段の横幅
    node.resize(2 * n - 1, init);  // init で初期化

    // 最下段に値を挿入
    rep(i, v.size()) node[(n - 1) + i] = v[i];

    // 最下段以外を更新していく
    for (int i = n - 2; i >= 0; i--) {
      node[i] = fx(node[i * 2 + 1], node[i * 2 + 2]);
    }
  }

  void update(int i, T x) {
    i += n - 1;  // i 番目の葉のノード番号
    node[i] = x;
    while (i > 0) {
      i = (i - 1) / 2;  // ノード i の親ノードの番号に変える
      node[i] = fx(node[i * 2 + 1], node[i * 2 + 2]);  // 左右の子の min を計算し直す
    }
  }

  void add(int i, T x) {
    i += n - 1;  // i 番目の葉のノード番号
    node[i] += x;
    while (i > 0) {
      i = (i - 1) / 2;  // ノード i の親ノードの番号に変える
      node[i] = fx(node[i * 2 + 1], node[i * 2 + 2]);
    }
  }

  // [a, b) の区間に対するクエリについてノード k (区間 [l, r) 相当) が答える
  // 注意 [a, b) は半区間なので 区間 [m, n] の最小値を求めたい場合は find(m, n+1) とする
  T find(int a, int b, int k = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;

    if (r <= a || b <= l) return init;  // 区間がかぶらない場合は init を返す
    if (a <= l && r <= b) {
      return node[k];  // ノード k の担当範囲がクエリ区間 [a, b) に完全に含まれる
    } else {
      T c1 = find(a, b, 2 * k + 1, l, (l + r) / 2);  // 左の子に値を聞く
      T c2 = find(a, b, 2 * k + 2, (l + r) / 2, r);  // 右の子に値を聞く
      return fx(c1, c2);
    }
  }

  T operator[](const int& i) const {
    return node[i + n - 1];
  }
};


// Static RMQ
// https://judge.yosupo.jp/problem/staticrmq
void static_rmq() {
  int n, q; cin >> n >> q;
  vector<ll> a(n);
  rep(i, n) cin >> a[i];

  auto f = [](ll x1, ll x2) { return min(x1, x2); };
  SegmentTree<ll> st(a, f, INF);

  rep(i, q) {
    int l, r; cin >> l >> r;
    cout << st.find(l, r) << endl;
  }
}

// https://judge.yosupo.jp/problem/point_set_range_composite
void point_set_range_composite() {
  int n, q;
  cin >> n >> q;
  vector<P> ab(n);
  rep(i, n) {
    ll a, b; cin >> a >> b;
    ab[i] = {a, b};
  }

  auto f = [&](P a, P b) -> P { return {(a.first * b.first) % mod, (a.second * b.first + b.second) % mod}; };
  P ini = {1, 0};
  SegmentTree<P> seg(ab, f, ini);

  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      ll p, c, d;
      cin >> p >> c >> d;
      seg.update(p, {c, d});
    } else {
      ll l, r, x;
      cin >> l >> r >> x;
      P res = seg.find(l, r);
      cout << (res.first * x + res.second) % mod << endl;
    }
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // static_rmq();
  point_set_range_composite();

  return 0;
}
