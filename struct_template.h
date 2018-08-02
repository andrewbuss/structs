#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include "hashutils.h"
#include "prettyprint.hpp"

{% for include in includes %}
#include <{{ include }}>
{% endfor %}

{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] %}
{%- set all_members = ', '.join(members.keys()) %}
using {{ lname }} = int;

struct {{ name }} {
  // {{ members }}
  {% for member, type in members.items() -%}
  const {{ type }} {{ member }};
  {% endfor -%}

  {% for property, prop_spec in (properties or {}).items() -%}
  // {{ prop_spec }}
  {{ prop_spec.type }} {{ property }}() const { return {{ prop_spec.body }}; }
  {%- endfor %}
  static const {{ name }} &get({{ lname }} {{ abbr }}) { return all_{{ lname }}s[{{ abbr }}]; }

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
  {%- endfor -%}):{% set comma = joiner(", ") -%}
  {%- for member, type in members.items() -%}
    {{ comma() }}{{ member }}({{ member }})
  {%- endfor -%}
  {}

  {% set comma = joiner(", ") -%}
  static {{ lname }} create(
    {%- for member, type in members.items() -%}
    {{ comma() }}const {{ type }}& {{ member }}
    {%- endfor -%}) {
    all_{{ lname }}s.push_back({{ name }}{ {{all_members}} });
    return index(all_{{ plural }}.size() - 1);
  }

  static std::vector<{{ name }}> all_{{ plural }};
  {% if unique_index %}
  static {{ lname }} get_or_create(const {{ lookups[unique_index].type }}& x) {
    auto {{ abbr }} = {{ unique_index }}(x);
    if (!{{ abbr }}) return create(x);
    return {{ abbr }};
  }
  {%- endif %}
  {% for lookup, lookup_spec in (lookups or {}).items() %}
  // {{ lookup_spec }}
  {%- if lookup_spec.unique %}
  static std::unordered_map<{{ lookup_spec.type }}, {{ lname }}> {{ lookup }}_index;
  static {{ lname }} {{ lookup }}(const {{ lookup_spec.type }}& x) {
    auto it = {{ lookup }}_index.find(x);
    if (it == {{ lookup }}_index.end())
      return 0;
    return it->second;
  }
  {% else %}
  using {{ lookup }}_index_type = std::unordered_multimap<{{ lookup_spec.type }}, {{ lname }}>;
  static {{ lookup }}_index_type {{ lookup }}_index;
  using {{ lookup }}_index_iterator = {{ lookup }}_index_type::const_iterator;

  static std::pair<{{ lookup }}_index_iterator, {{ lookup }}_index_iterator>
  {{ lookup }}(const {{ lookup_spec.type }}& x) {
    return {{ lookup }}_index.equal_range(x);
  }
  {% endif %}
  {% endfor %}
  static {{ lname }} index(const {{ lname }} {{ abbr }}) {
    const auto obj = all_{{ plural }}[{{ abbr }}];
    {%- for lookup, lookup_spec in (lookups or {}).items() -%}
    {%- set prop_var = 'obj_' + lookup_spec.getter.strip('()') -%}
    {% if lookup_spec.iterable %}
    for(const auto& {{ prop_var }} : obj.{{ lookup_spec.getter }})
    {% else %}
    const auto& {{ prop_var }} = obj.{{ lookup_spec.getter }};
    {% endif %}
    {%- if lookup_spec.unique -%}
    {{ lookup }}_index[{{ prop_var }}] = {{ abbr }};
    {%- else -%}
    {{ lookup }}_index.emplace({{prop_var }}, {{ abbr }});
    {%- endif %}
    {%- endfor %}
    return {{ abbr }};
  }
};

std::ostream& operator<<(std::ostream& os, const {{ name }}& {{ abbr }});
