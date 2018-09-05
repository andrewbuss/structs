

#include <metavar.h>
#include <type.h>

std::vector<Metavar> MetavarIndex::all_Metavars{{}};
const Metavar *metavar::operator->() const {
  return MetavarIndex::all_Metavars.data() + i;
}
const Metavar &metavar::operator*() const {
  return MetavarIndex::all_Metavars[i];
}
metavar metavar::null = 0;

MetavarIndex::lookup_by_typ_index_type MetavarIndex::lookup_by_typ_index;

std::unordered_map<std::pair<type, int>, metavar>
    MetavarIndex::lookup_by_typ_i_index;

// {'body': '{ return std::make_pair(typ, i); }', 'type': 'std::pair<type,
// int>'}
std::pair<type, int> Metavar::typ_i() const {
  { return std::make_pair(typ, i); }
}

metavar Metavar::create(const int &i, const type &typ) {
  MetavarIndex::all_Metavars.push_back({i, typ});
  metavar m{(int)MetavarIndex::all_Metavars.size() - 1};

  return MetavarIndex::index(m);
}

metavar Metavar::get_or_create(const std::pair<type, int> &x) {
  auto m = MetavarIndex::lookup_by_typ_i_index.find(x);
  if (m == MetavarIndex::lookup_by_typ_i_index.end())
    return create(x);
  return m->second;
}

metavar Metavar::get_if_exists(const std::pair<type, int> &x) {
  auto m = MetavarIndex::lookup_by_typ_i(x);
  if (!m)
    return metavar::null;
  return m;
}

// {'getter': 'typ', 'type': 'type'}
std::pair<MetavarIndex::lookup_by_typ_index_iterator,
          MetavarIndex::lookup_by_typ_index_iterator>
MetavarIndex::lookup_by_typ(const type &x) {
  return lookup_by_typ_index.equal_range(x);
}

// {'getter': 'typ_i()', 'unique': True, 'type': 'std::pair<type, int>'}
metavar MetavarIndex::lookup_by_typ_i(const std::pair<type, int> &x) {
  auto it = lookup_by_typ_i_index.find(x);
  if (it == lookup_by_typ_i_index.end())
    return metavar();
  return it->second;
}

metavar MetavarIndex::index(const metavar m) {
  const auto &obj = *m;
  const auto &obj_typ = obj.typ;
  lookup_by_typ_index.emplace(obj_typ, m);
  const auto &obj_typ_i = obj.typ_i();
  lookup_by_typ_i_index[obj_typ_i] = m;
  return m;
}

std::ostream &operator<<(std::ostream &os, const Metavar &m) {
  os << "Metavar{";
  os << m.i;
  os << ", " << m.typ;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const metavar &m) {
  return os << *(m) << "@" << m.i;
}
