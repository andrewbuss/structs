#include <axiom.h>
#include <cassert>
#include <iostream>
#include <judgment.h>
#include <prettyprint.hpp>
#include <rule.h>
#include <substitution.h>
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
  cout << naming::null << endl;
  NamingIndex::index(naming::null);
  token t = Token::get_or_create("foo");
  cout << t << endl;

  token t2 = Token::get_or_create("bar");
  cout << t2 << endl;

  token t3 = Token::get_or_create("foo");
  cout << t3 << endl;

  assert(t == t3);
  assert(t != t2);

  token wff = Token::get_or_create("wff");
  type wff_type = Type::get_or_create(wff);

  token set = Token::get_or_create("set");
  type set_type = Type::get_or_create(set);
  assert(set_type == Type::get_or_create(set));
  assert(set_type != Type::get_or_create(wff));

  token turnstile = Token::get_or_create("|-");
  type turnstile_type = Type::get_or_create(turnstile);
  token x = Token::get_or_create("x");
  token y = Token::get_or_create("y");
  token equals = Token::get_or_create("=");
  //  cout << TokenIndex::lookup_by_s_index << endl;

  judgment j6 = Judgment::get_or_create({"|-", "set$0", "=", "set$1"});
  judgment j7 =
      Judgment::get_or_create({"|-", "(", "wff$0", "->", "wff$1", ")"});

  judgment j8 = Judgment::get_or_create(vector<string>{"|-", "wff$0"});
  judgment j9 = Judgment::get_or_create(vector<string>{"|-", "wff$1"});
  judgment j10 =
      Judgment::get_or_create({"|-", "(", "wff$1", "->", "wff$0", ")"});
  substitution s1 = Substitution::get_or_create(
      j7,
      vector<substitution>{
          Substitution::get_or_create(
              Judgment::get_or_create({"set", "(", "1", "+", "1", ")"}), {}),
          Substitution::get_or_create(
              Judgment::get_or_create({"set", "(", "0", "+", "2", ")"}), {})});
  substitution s2 = Substitution::get_or_create(j8, {});
  substitution s3 = Substitution::get_or_create(j9, {});
  substitution s4 = Substitution::get_or_create(j7, {});
  axiom a1 = Axiom::get_or_create(s3, vector<substitution>{s2, s4});
  cout << a1 << endl;
  cout << a1->sub << endl;
  cout << a1->sub->as_judgment() << endl;
  axiom  a2 = Axiom::get_or_create(s3, vector<substitution>{s4, s2});
  cout << a2 << endl;
  cout << a2->sub << endl;


  ForwardRenaming r{{x, Metavar::get_or_create(wff_type, 0)},
                    {y, Metavar::get_or_create(wff_type, 1)}};

  judgment j1 = Judgment::get_or_create(vector<string>{"wff", "x"}, r);
  judgment j2 = Judgment::get_or_create(vector<token>{wff, y}, r);
  judgment j3 = Judgment::get_or_create(vector<token>{wff, x}, r);
  judgment j4 = Judgment::get_or_create(vector<token>{turnstile, x}, r);
  judgment j5 =
      Judgment::get_or_create(vector<token>{turnstile, x, equals, y}, r);

  //  cout << MetavarIndex::lookup_by_typ_index << endl;
  //  cout << ShapeIndex::lookup_by_shp_index << endl;

  for (const auto &it : TokenIndex::all_Tokens){
    cout << it << endl;
  }

  for (const auto &it : JudgmentIndex::lookup_by_shp_nam_index) {
    cout << it.second << endl << it.second->flattened() << endl;
  }
}
