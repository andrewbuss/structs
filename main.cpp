#include <application.h>
#include <iostream>
#include <judgment.h>
#include <prettyprint.hpp>
#include <rule.h>
#include <token.h>
#include <vector>

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

  token wff = Token::get_or_create("wff");
  token turnstile = Token::get_or_create("|-");
  token x = Token::get_or_create("x");
  token y = Token::get_or_create("y");
  token equals = Token::get_or_create("=");
  cout << Token::all_tokens << endl;
  cout << Token::lookup_by_s_index << endl;

  judgment j1 = Judgment::get_or_create({wff, x});
  judgment j2 = Judgment::get_or_create({wff, y});
  judgment j3 = Judgment::get_or_create({wff, x});
  judgment j4 = Judgment::get_or_create({turnstile, x});
  judgment j5 = Judgment::get_or_create({turnstile, x, equals, y});

  cout << Judgment::all_judgments << endl;
  cout << Judgment::lookup_by_type_index << endl;
  cout << "wff: " << range_to_vec<judgment>(Judgment::lookup_by_type(wff))
       << endl;
  cout << "|-: " << range_to_vec<judgment>(Judgment::lookup_by_type(turnstile))
       << endl;
  cout << "x: " << range_to_vec<judgment>(Judgment::lookup_by_token(x))
       << endl;

}
