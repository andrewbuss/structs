

#include <naming.h>
#include <token.h>

std::vector<Naming> NamingIndex::all_Namings{{}};
const Naming *naming::operator->() const {
  return NamingIndex::all_Namings.data() + i;
}
const Naming &naming::operator*() const { return NamingIndex::all_Namings[i]; }

NamingIndex::lookup_by_naming_index_type NamingIndex::lookup_by_naming_index;

std::unordered_map<std::vector<token>, naming> NamingIndex::lookup_by_cs_index;

NamingIndex::lookup_by_type_index_type NamingIndex::lookup_by_type_index;

NamingIndex::lookup_by_shape_index_type NamingIndex::lookup_by_shape_index;

NamingIndex::lookup_by_token_index_type NamingIndex::lookup_by_token_index;

// {'body': '{ if(cs.size() > 0) return cs[0]; else return token(); }', 'type':
// 'token'}
token Naming::type() const {
  {
    if (cs.size() > 0)
      return cs[0];
    else
      return token();
  }
}

naming Naming::create(const std::vector<token> &cs, const shape &s,
                      const naming &n) {
  NamingIndex::all_Namings.push_back({cs, s, n});
  naming n = {(int)NamingIndex::all_Namings.size() - 1};
  return NamingIndex::index(n);
}

naming Naming::get_or_create(const std::vector<token> &x) {
  auto n = NamingIndex::lookup_by_cs(x);
  if (!n)
    return create(x);
  return n;
}

// {'getter': 'n', 'type': 'naming'}
std::pair<NamingIndex::lookup_by_naming_index_iterator,
          NamingIndex::lookup_by_naming_index_iterator>
NamingIndex::lookup_by_naming(const naming &x) {
  return lookup_by_naming_index.equal_range(x);
}

// {'getter': 'cs', 'unique': True, 'type': 'std::vector<token>'}
naming NamingIndex::lookup_by_cs(const std::vector<token> &x) {
  auto it = lookup_by_cs_index.find(x);
  if (it == lookup_by_cs_index.end())
    return naming();
  return it->second;
}

// {'getter': 'type()', 'type': 'token'}
std::pair<NamingIndex::lookup_by_type_index_iterator,
          NamingIndex::lookup_by_type_index_iterator>
NamingIndex::lookup_by_type(const token &x) {
  return lookup_by_type_index.equal_range(x);
}

// {'getter': 's', 'type': 'shape'}
std::pair<NamingIndex::lookup_by_shape_index_iterator,
          NamingIndex::lookup_by_shape_index_iterator>
NamingIndex::lookup_by_shape(const shape &x) {
  return lookup_by_shape_index.equal_range(x);
}

// {'getter': 'cs', 'type': 'token', 'iterable': True}
std::pair<NamingIndex::lookup_by_token_index_iterator,
          NamingIndex::lookup_by_token_index_iterator>
NamingIndex::lookup_by_token(const token &x) {
  return lookup_by_token_index.equal_range(x);
}

naming NamingIndex::index(const naming n) {
  const auto &obj = *n;
  const auto &obj_n = obj.n;
  lookup_by_naming_index.emplace(obj_n, n);
  const auto &obj_cs = obj.cs;
  lookup_by_cs_index[obj_cs] = n;
  const auto &obj_type = obj.type();
  lookup_by_type_index.emplace(obj_type, n);
  const auto &obj_s = obj.s;
  lookup_by_shape_index.emplace(obj_s, n);
  for (const auto &obj_cs : obj.cs)
    lookup_by_token_index.emplace(obj_cs, n);
  return n;
}

/*
std::ostream& operator<<(std::ostream& os, const Naming& n) {
  os << "Naming{";
  os <<  n.cs;os <<  ", " <<  n.s;os <<  ", " <<  n.n;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const naming& n) {
  return os << n.i;
}
*/
