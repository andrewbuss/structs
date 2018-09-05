

#include <proof.h>

std::vector<Proof> ProofIndex::all_Proofs{{}};
const Proof *proof::operator->() const {
  return ProofIndex::all_Proofs.data() + i;
}
const Proof &proof::operator*() const { return ProofIndex::all_Proofs[i]; }
proof proof::null = 0;

std::unordered_map<axiom_axioms, proof> ProofIndex::lookup_by_via_args_index;

ProofIndex::lookup_by_via_index_type ProofIndex::lookup_by_via_index;

ProofIndex::lookup_by_arg_index_type ProofIndex::lookup_by_arg_index;

// {'body': '{ return std::make_pair(via, args); }', 'type': 'axiom_axioms'}
axiom_axioms Proof::via_args() const {
  { return std::make_pair(via, args); }
}

proof Proof::create(const axiom &via, const axioms &args) {
  ProofIndex::all_Proofs.push_back({via, args});
  proof p{(int)ProofIndex::all_Proofs.size() - 1};

  return ProofIndex::index(p);
}

// {'getter': 'via_args()', 'unique': True, 'type': 'axiom_axioms'}
proof ProofIndex::lookup_by_via_args(const axiom_axioms &x) {
  auto it = lookup_by_via_args_index.find(x);
  if (it == lookup_by_via_args_index.end())
    return proof();
  return it->second;
}

// {'getter': 'via', 'type': 'axiom'}
std::pair<ProofIndex::lookup_by_via_index_iterator,
          ProofIndex::lookup_by_via_index_iterator>
ProofIndex::lookup_by_via(const axiom &x) {
  return lookup_by_via_index.equal_range(x);
}

// {'getter': 'args', 'type': 'axiom', 'iterable': True}
std::pair<ProofIndex::lookup_by_arg_index_iterator,
          ProofIndex::lookup_by_arg_index_iterator>
ProofIndex::lookup_by_arg(const axiom &x) {
  return lookup_by_arg_index.equal_range(x);
}

proof ProofIndex::index(const proof p) {
  const auto &obj = *p;
  const auto &obj_via_args = obj.via_args();
  lookup_by_via_args_index[obj_via_args] = p;
  const auto &obj_via = obj.via;
  lookup_by_via_index.emplace(obj_via, p);
  for (const auto &obj_args : obj.args)
    lookup_by_arg_index.emplace(obj_args, p);
  return p;
}

std::ostream &operator<<(std::ostream &os, const Proof &p) {
  os << "Proof{";
  os << p.via;
  os << ", " << p.args;
  return os << "}";
}

std::ostream &operator<<(std::ostream &os, const proof &p) {
  return os << *(p) << "@" << p.i;
}
