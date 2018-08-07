

#include <constant.h>
#include <token.h>

std::vector<Constant> ConstantIndex::all_Constants{{}};
const Constant *constant::operator->() const {
  return ConstantIndex::all_Constants.data() + i;
}
const Constant &constant::operator*() const {
  return ConstantIndex::all_Constants[i];
}

std::unordered_map<token, constant> ConstantIndex::lookup_by_tok_index;

constant Constant::create(const token &tok) {
  ConstantIndex::all_Constants.push_back({tok});
  constant c{(int)ConstantIndex::all_Constants.size() - 1};

  return ConstantIndex::index(c);
}

constant Constant::get_or_create(const token &x) {
  auto c = ConstantIndex::lookup_by_tok(x);
  if (!c)
    return create(x);
  return c;
}

constant Constant::get_if_exists(const token &x) {
  auto c = ConstantIndex::lookup_by_tok(x);
  if (!c)
    return constant{0};
  return c;
}

// {'getter': 'tok', 'unique': True, 'type': 'token'}
constant ConstantIndex::lookup_by_tok(const token &x) {
  auto it = lookup_by_tok_index.find(x);
  if (it == lookup_by_tok_index.end())
    return constant();
  return it->second;
}

constant ConstantIndex::index(const constant c) {
  const auto &obj = *c;
  const auto &obj_tok = obj.tok;
  lookup_by_tok_index[obj_tok] = c;
  return c;
}

/*
std::ostream& operator<<(std::ostream& os, const Constant& c) {
  os << "Constant{";
  os <<  c.tok;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const constant& c) {
  return os << c.i;
}
*/
