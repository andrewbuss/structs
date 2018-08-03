#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace hashutils {
/* hash any pointer */
template <typename T> struct PointerHash {
  inline size_t operator()(const T *pointer) const {
    auto addr = reinterpret_cast<uintptr_t>(pointer);
#if SIZE_MAX < UINTPTR_MAX
    /* size_t is not large enough to hold the pointer’s memory address */
    addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a
                         size_t */
#endif
    return addr;
  }
  inline size_t operator()(T *pointer) const {
    auto addr = reinterpret_cast<uintptr_t>(pointer);
#if SIZE_MAX < UINTPTR_MAX
    /* size_t is not large enough to hold the pointer’s memory address */
    addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a
                         size_t */
#endif
    return addr;
  }
  inline size_t operator()(std::shared_ptr<T> sp) const {
    auto addr = reinterpret_cast<uintptr_t>(sp.get());
#if SIZE_MAX < UINTPTR_MAX
    /* size_t is not large enough to hold the pointer’s memory address */
    addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a
                         size_t */
#endif
    return addr;
  }
};
} // namespace hashutils

namespace hashutils {
template <typename T> inline size_t hash(const T &v) {
  return std::hash<T>()(v);
}

/* hash based off of a pointer dereference */
template <typename T> struct PointerDereferenceHash {
  inline size_t operator()(const T &pointer) const { return hash(*pointer); }
};

/* equality based off of pointer dereference */
template <typename T> struct PointerDereferenceEqualTo {
  inline bool operator()(const T &lhs, const T &rhs) const {
    return *lhs == *rhs;
  }
};

template <typename K, typename V>
using unordered_pointer_dereference_map =
    std::unordered_map<K, V, PointerDereferenceHash<K>,
                       PointerDereferenceEqualTo<K>>;
} // namespace hashutils

namespace hashutils {
class HashCombiner {
private:
  size_t h;

public:
  HashCombiner() : h(0) {}
  template <class T> inline HashCombiner &operator<<(const T &obj) {
    /* adapted from boost::hash_combine */
    h ^= hash(obj) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return *this;
  }
  operator size_t() const { return h; }
};

/* hash any container */
template <typename T> struct ContainerHash {
  size_t operator()(const T &v) const {
    HashCombiner h;
    for (const auto &e : v) {
      h << e;
    }
    return h;
  }
};
} // namespace hashutils

namespace std {
template <typename... T>
struct hash<set<T...>> : hashutils::ContainerHash<set<T...>> {};

template <typename... T>
struct hash<vector<T...>> : hashutils::ContainerHash<vector<T...>> {};

template <typename... T>
struct hash<unordered_map<T...>>
    : hashutils::ContainerHash<unordered_map<T...>> {};

template <typename... T>
struct hash<multiset<T...>>
    : hashutils::ContainerHash<multiset<T...>> {};

template <typename K, typename V> struct hash<pair<K, V>> {
  size_t operator()(pair<K, V> x) const {
    return hash<K>(x.first) ^ hash<V>(x.second);
  }
};

template <> struct hash<const string *&> {
  size_t operator()(const string *&x) const {
    return reinterpret_cast<uintptr_t>(x);
  }
};
} // namespace std
