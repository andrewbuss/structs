

#include <naming.h>
#include <token.h>

#include <cstdint>

std::vector<Naming> NamingIndex::all_Namings{{}};
const Naming *naming::operator->() const {
  return NamingIndex::all_Namings.data() + i;
}
const Naming &naming::operator*() const { return NamingIndex::all_Namings[i]; }

std::unordered_map<std::vector<uint8_t>, naming>
    NamingIndex::lookup_by_is_index;

naming Naming::create(const std::vector<uint8_t> &is) {
  NamingIndex::all_Namings.push_back({is});
  naming n{(int)NamingIndex::all_Namings.size() - 1};

  return NamingIndex::index(n);
}

naming Naming::get_or_create(const std::vector<uint8_t> &x) {
  auto n = NamingIndex::lookup_by_is(x);
  if (!n)
    return create(x);
  return n;
}

naming Naming::get_if_exists(const std::vector<uint8_t> &x) {
  auto n = NamingIndex::lookup_by_is(x);
  if (!n)
    return naming{0};
  return n;
}

// {'getter': 'is', 'unique': True, 'type': 'std::vector<uint8_t>'}
naming NamingIndex::lookup_by_is(const std::vector<uint8_t> &x) {
  auto it = lookup_by_is_index.find(x);
  if (it == lookup_by_is_index.end())
    return naming();
  return it->second;
}

naming NamingIndex::index(const naming n) {
  const auto &obj = *n;
  const auto &obj_is = obj.is;
  lookup_by_is_index[obj_is] = n;
  return n;
}

/*
std::ostream& operator<<(std::ostream& os, const Naming& n) {
  os << "Naming{";
  os <<  n.is;return os << "}";
}
*/

/*
std::ostream& operator<<(std::ostream& os, const naming& n) {
  return os << n.i;
}
*/
