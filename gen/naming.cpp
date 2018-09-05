

#include <naming.h>

std::vector<Naming> NamingIndex::all_Namings{{}};
const Naming *naming::operator->() const {
  return NamingIndex::all_Namings.data() + i;
}
const Naming &naming::operator*() const { return NamingIndex::all_Namings[i]; }
naming naming::null = 0;

std::unordered_map<mapping, naming> NamingIndex::lookup_by_map_index;

naming Naming::create(const mapping &map) {
  NamingIndex::all_Namings.push_back({map});
  naming n{(int)NamingIndex::all_Namings.size() - 1};

  return NamingIndex::index(n);
}

naming Naming::get_or_create(const mapping &x) {
  auto n = NamingIndex::lookup_by_map_index.find(x);
  if (n == NamingIndex::lookup_by_map_index.end())
    return create(x);
  return n->second;
}

naming Naming::get_if_exists(const mapping &x) {
  auto n = NamingIndex::lookup_by_map(x);
  if (!n)
    return naming::null;
  return n;
}

// {'getter': 'map', 'unique': True, 'type': 'mapping'}
naming NamingIndex::lookup_by_map(const mapping &x) {
  auto it = lookup_by_map_index.find(x);
  if (it == lookup_by_map_index.end())
    return naming();
  return it->second;
}

naming NamingIndex::index(const naming n) {
  const auto &obj = *n;
  const auto &obj_map = obj.map;
  lookup_by_map_index[obj_map] = n;
  return n;
}

std::ostream &operator<<(std::ostream &os, const Naming &n) {
  os << "Naming{";
  os << n.map;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const naming &n) {
  return os << *(n) << "@" << n.i;
}
