#include <bits/stdc++.h>
#define rep(i,n) for (int i = 0; i < n; ++i)
using namespace std;

// S と S[i:|S|-1] の最長共通接頭辞の長さ を返す  O(|S|)
vector<int> z_algorighm(string s) {
  vector<int> z(s.size());
  z[0] = s.size();
  int i = 1, j = 0;
  while (i < s.size()) {
    while (i + j < s.size() && s[j] == s[i + j]) j++;
    z[i] = j;
    if (j == 0) {
      i++;
      continue;
    }
    int k = 1;
    while (k < j && k + z[k] < j) {
      z[i + k] = z[k];
      k++;
    }
    i += k;
    j -= k;
  }
  return z;
}

// https://judge.yosupo.jp/problem/zalgorithm
void zalgorithm() {
  string s;
  cin >> s;
  auto z = z_algorighm(s);
  for (int i = 0; i < z.size(); i++) {
    printf("%d%c", z[i], i == z.size()-1 ? '\n' : ' ');
  }
}


int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  zalgorithm();

  return 0;
}
