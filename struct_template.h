#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include "hashutils.hpp"
#include "prettyprint.hpp"

{% for include in includes %}
#include <{{ include }}>
{% endfor %}

{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] %}
{%- set all_members = ', '.join(members.keys()) %}

struct {{ name }};

struct {{ lname }} {
  int i;
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
    size_t operator()( const {{ lname }} k ) const {
      return hash<int>{}(k.i);
    }
  };
}

struct {{ name }} {
  // {{ members }}
  {% for member, type in members.items() -%}
  const {{ type }} {{ member }};
  {% endfor -%}

  {% for property, prop_spec in (properties or {}).items() -%}
  // {{ prop_spec }}
  {{ prop_spec.type }} {{ property }}() const;
  {%- endfor %}

  {% set comma = joiner(", ") -%}
  {{ name }}():
  {%- for member, type in members.items() -%}
    {{ comma() }}{{ member }}({{ type }} ())
  {%- endfor -%}
  {}

  {% set comma = joiner(", ") -%}
  {{ name }}(
    {%- for member, type in members.items() -%}
    {{ comma() }}const {{ type }}& {{ member }}
    {%- endfor -%}):
  {% set comma = joiner(", ") -%}
  {%- for member, type in members.items() -%}
    {{ comma() }}{{ member }}({{ member }})
  {%- endfor -%}
  {}
  static {{ lname }} create(
  {% set comma = joiner(", ") -%}
  {%- for member, type in members.items() -%}
    {{ comma() }}const {{ type }}& {{ member }}
  {%- endfor -%});

  {% if unique_index %}
  static {{ lname }} get_or_create(const {{ lookups[unique_index].type }}& x);
  {%- endif %}

  {{ extra }}
};

struct {{ name }}Index {
  {% for lookup, lookup_spec in (lookups or {}).items() %}
  // {{ lookup_spec }}
  {%- if lookup_spec.unique %}
  static std::unordered_map<{{ lookup_spec.type }}, {{ lname }}> {{ lookup }}_index;
  static {{ lname }} {{ lookup }}(const {{ lookup_spec.type }}& x);
  {% else %}
  using {{ lookup }}_index_type = std::unordered_multimap<{{ lookup_spec.type }}, {{ lname }}>;
  static {{ lookup }}_index_type {{ lookup }}_index;
  using {{ lookup }}_index_iterator = {{ lookup }}_index_type::const_iterator;
  static std::pair<{{ lookup }}_index_iterator, {{ lookup }}_index_iterator>
  {{ lookup }}(const {{ lookup_spec.type }}& x);
  {% endif %}
  {% endfor %}
  static {{ lname }} index({{ lname }});
};

std::ostream& operator<<(std::ostream& os, const {{ name }}&);
std::ostream& operator<<(std::ostream& os, const {{ lname }}&);
