

#include <type.h>

std::vector<Type> TypeIndex::all_Types{{}};
const Type *type::operator->() const { return TypeIndex::all_Types.data() + i; }
const Type &type::operator*() const { return TypeIndex::all_Types[i]; }
type type::null = 0;

std::unordered_map<token, type> TypeIndex::lookup_by_tok_index;

type Type::create(const token &tok) {
  TypeIndex::all_Types.push_back({tok});
  type t{(int)TypeIndex::all_Types.size() - 1};

  return TypeIndex::index(t);
}

type Type::get_or_create(const token &x) {
  auto t = TypeIndex::lookup_by_tok_index.find(x);
  if (t == TypeIndex::lookup_by_tok_index.end())
    return create(x);
  return t->second;
}

type Type::get_if_exists(const token &x) {
  auto t = TypeIndex::lookup_by_tok(x);
  if (!t)
    return type::null;
  return t;
}

// {'getter': 'tok', 'unique': True, 'type': 'token'}
type TypeIndex::lookup_by_tok(const token &x) {
  auto it = lookup_by_tok_index.find(x);
  if (it == lookup_by_tok_index.end())
    return type();
  return it->second;
}

type TypeIndex::index(const type t) {
  const auto &obj = *t;
  const auto &obj_tok = obj.tok;
  lookup_by_tok_index[obj_tok] = t;
  return t;
}

std::ostream &operator<<(std::ostream &os, const Type &t) {
  os << "Type{";
  os << t.tok;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const type &t) {
  return os << *(t) << "@" << t.i;
}
