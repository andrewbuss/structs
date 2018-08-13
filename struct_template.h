{%- set lname = name.lower() %}
{%- set plural = lname + 's' %}
{%- set abbr = lname[0] %}
{%- set all_members = ', '.join(members.keys()) %}

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include "hashutils.hpp"
#include "prettyprint.hpp"
#include "{{ lname }}_ptr.h"
{% for include in includes %}
#include <{{ include }}>
{% endfor %}

{% for name, val in (usings or {}).iteritems() %}
using {{ name }} = {{ val }};
{% endfor %}


{% for s in forward_declared_structs %}
struct {{ s }};
{% endfor %}

struct {{ name }} {
  {% for member, type in members.items() -%}
  const {{ type }} {{ member }};
  {% endfor -%}

  {% for name, type in (mutable or {}).items() -%}
  mutable {{ type }} {{ name }};
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
  static {{ lname }} get_if_exists(const {{ lookups[unique_index].type }}& x);
  {{ lname }} save() const {
    return get_or_create({{ lookups[unique_index].getter }});
  }
  {% else %}
  {{ lname }} save() const {
    return create({{ all_members }});
  }
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
  static std::vector<{{ name }}> all_{{ name }}s;
};

std::ostream& operator<<(std::ostream& os, const {{ name }}&);
std::ostream& operator<<(std::ostream& os, const {{ lname }}&);
