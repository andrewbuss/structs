#include <application.h>
#include <iostream>
#include <judgment.h>
#include <prettyprint.hpp>
#include <rule.h>
#include <token.h>
#include <vector>
#include <cassert>

using namespace std;

template <typename V, typename iter>
vector<V> range_to_vec(const pair<iter, iter> iters) {
  vector<V> v;
  auto &[start, end] = iters;
  for (iter it = start; it != end; ++it)
    v.push_back(it->second);
  return v;
}

int main() {
  token t = Token::get_or_create("foo");
  cout << t << endl;

  token t2 = Token::get_or_create("bar");
  cout << t2 << endl;

  token t3 = Token::get_or_create("foo");
  cout << t3 << endl;

  assert(t == t3);
  assert(t != t2);

  token wff = Token::get_or_create("wff");
  token turnstile = Token::get_or_create("|-");
  token x = Token::get_or_create("x");
  token y = Token::get_or_create("y");
  token equals = Token::get_or_create("=");
  //  cout << TokenIndex::lookup_by_s_index << endl;

  judgment j1 = Judgment::get_or_create(vector<string>{"wff", "x"});
  judgment j2 = Judgment::get_or_create(vector<token>{wff, y});
  judgment j3 = Judgment::get_or_create(vector<token>{wff, x});
  judgment j4 = Judgment::get_or_create(vector<token>{turnstile, x});
  judgment j5 = Judgment::get_or_create(vector<token>{turnstile, x, equals, y});

  cout << JudgmentIndex::lookup_by_type_index << endl;
  cout << "wff: " << range_to_vec<judgment>(JudgmentIndex::lookup_by_type(wff->typ))
       << endl;
  cout << "|-: " << range_to_vec<judgment>(JudgmentIndex::lookup_by_type(turnstile->typ))
       << endl;
  cout << "x: " << range_to_vec<shape>(ShapeIndex::lookup_by_token(x))
       << endl;

}
