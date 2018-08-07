#pragma once

#include <functional>

{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] %}
{%- set all_members = ', '.join(members.keys()) %}

struct {{ name }};

struct {{ lname }} {
  int i = 0;
  const {{ name }}& operator*() const;
  operator bool() const { return i != 0; }
  {{ lname }} () : i(0) {}
  {{ lname }} (int i) : i(i) {}
  {{ lname }} operator=(const {{ lname }}& other) { return i = other.i; }
  bool operator==(const {{ lname }}& other) const { return i == other.i; }
  bool operator!=(const {{ lname }}& other) const { return i != other.i; }
  const {{ name }}* operator->() const;
};

namespace std {
  template <> struct hash<{{ lname }}> {
    size_t operator()(const {{ lname }}& k ) const {
      return k.i;
    }
  };
  template <> struct hash<const {{ lname }}> {
    size_t operator()(const {{ lname }}& k ) const {
      return k.i;
    }
  };
}
