#include <bits/stdc++.h>
#define rep(i,n) for (int i = 0; i < n; ++i)
using namespace std;

using ll = long long;

template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; }

using P = pair<ll, ll>;

///////////////////////////////////////////////////////////////////////////////
/**
 * Lazy Segment Tree
 */
template <typename X, typename M>
class SegmentTree {
  using Fx = function<X(X, X)>;
  using Fa = function<X(X, M)>;
  using Fm = function<M(M, M)>;
  Fx fx_;
  Fa fa_;
  Fm fm_;
  const X ex_;
  const M em_;
  vector<X> node_;
  vector<M> lazy_;
  int n_;             // 最下段のノード数

 public:
  SegmentTree(const vector<X>& v, Fx fx, Fa fa, Fm fm, X ex, M em)
   : fx_(fx), fa_(fa), fm_(fm), ex_(ex), em_(em) {
    n_ = 1;
    while (n_ < v.size()) n_ <<= 1;  // n: 最下段の横幅
    node_.resize(2 * n_ - 1, ex_);   // ex で初期化
    lazy_.resize(2 * n_ - 1, em_);   // em で初期化

    // 最下段に値を挿入
    rep(i, v.size()) set(i, v[i]);
    // 最下段以外を更新していく
    build();
  }

  void set(int i, X x) { node_[(n_ - 1) + i] = x; }

  void build() {
    for (int i = n_ - 2; i >= 0; i--) {
      node_[i] = fx_(node_[i * 2 + 1], node_[i * 2 + 2]);
    }
  }

  // lazy eval
  void eval(int k) {
    if (lazy_[k] == em_) return;  // 更新するものが無ければ終了
    if (k < n_ - 1) {              // 葉でなければ子に伝搬
      lazy_[k * 2 + 1] = fm_(lazy_[k * 2 + 1], lazy_[k]);
      lazy_[k * 2 + 2] = fm_(lazy_[k * 2 + 2], lazy_[k]);
    }
    // 自身を更新
    node_[k] = fa_(node_[k], lazy_[k]);
    lazy_[k] = em_;
  }

  // [a, b) の区間の要素を x に更新 O(log(n))
  void update(int a, int b, M x, int k = 0, int l = 0, int r = -1) {
    if (r < 0) r = n_;
    eval(k);
    if (a <= l && r <= b) {  // 完全に内側のとき
      lazy_[k] = fm_(lazy_[k], x);
      eval(k);
    } else if (a < r && l < b) {   // 一部区間が被るとき
      update(a, b, x, k * 2 + 1, l, (l + r) / 2);   // 左の子
      update(a, b, x, k * 2 + 2, (l + r) / 2, r);   // 右の子
      node_[k] = fx_(node_[k * 2 + 1], node_[k * 2 + 2]);
    }
  }

  // [a, b) の区間に対するクエリについてノード k (区間 [l, r) 相当) が答える O(log(n))
  // 注意 [a, b) は半区間なので 区間 [m, n] の最小値を求めたい場合は find(m, n+1) とする
  X find(int a, int b, int k = 0, int l = 0, int r = -1) {
    if (r < 0) r = n_;
    eval(k);
    if (r <= a || b <= l) return ex_;  // 区間がかぶらない場合は init を返す

    if (a <= l && r <= b) {  // ノード k の担当範囲がクエリ区間 [a, b) に完全に含まれる
      return node_[k];
    } else {                        // 一部区間が被るとき
      X c1 = find(a, b, k * 2 + 1, l, (l + r) / 2);  // 左の子に値を聞く
      X c2 = find(a, b, k * 2 + 2, (l + r) / 2, r);  // 右の子に値を聞く
      return fx_(c1, c2);
    }
  }

  X operator[](int i) { return find(i, i+1); }
};
///////////////////////////////////////////////////////////////////////////////


// 区間更新・一点取得 https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_D
void aoj_dsl_2_d() {
  int n, q; cin >> n >> q;
  ll init = (1LL<<31) - 1;
  vector<ll> a(n, init);
  auto fx = [&](ll a, ll b) { return a == init ? b : a; };
  auto fa = [](ll a, ll b) { return b; };
  auto fm = [](ll a, ll b) { return b; };
  SegmentTree<ll, ll> seg(a, fx, fa, fm, init, init);
  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      int s, t, x; cin >> s >> t >> x;
      seg.update(s, t+1, x);
    } else {
      int s; cin >> s;
      cout << seg.find(s, s+1) << endl;
    }
  }
}

// 区間加算・一点取得 https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_E
void aoj_dsl_2_e() {
  int n, q; cin >> n >> q;
  ll init = 0;
  vector<ll> a(n, init);
  auto fx = [&](ll a, ll b) { return a == init ? b : a; };
  auto fa = [](ll a, ll b) { return a + b; };
  auto fm = [](ll a, ll b) { return a + b; };
  SegmentTree<ll, ll> seg(a, fx, fa, fm, init, init);
  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      int s, t, x; cin >> s >> t >> x; s--; t--;
      seg.update(s, t+1, x);
    } else {
      int s; cin >> s; s--;
      cout << seg.find(s, s+1) << endl;
    }
  }
}

// 区間更新・区間最小  https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_F
void aoj_dsl_2_f() {
  int n, q;
  cin >> n >> q;
  ll init = (1LL<<31)-1;
  vector<ll> a(n, init);
  auto fx = [](ll a, ll b) { return min(a, b); };
  auto fa = [](ll a, ll b) { return b; };
  auto fm = [](ll a, ll b) { return b; };
  SegmentTree<ll, ll> seg(a, fx, fa, fm, init, init);
  rep(i, q) {
    int t; cin >> t;
    if (t == 0) {
      int s, t, x;
      cin >> s >> t >> x;
      seg.update(s, t + 1, x);
    } else {
      int s, t;
      cin >> s >> t;
      cout << seg.find(s, t + 1) << endl;
    }
  }
}


int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // aoj_dsl_2_d();
  // aoj_dsl_2_e();
  aoj_dsl_2_f();

  return 0;
}
