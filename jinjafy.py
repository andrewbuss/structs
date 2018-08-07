
from jinja2 import Template
import yaml
import sys
import string

template = Template(open(sys.argv[1]).read())
sanitize = lambda s: s.translate(string.maketrans('->','__'),'()')
for definition in sys.argv[2:]:
    print template.render(sanitize=sanitize, **yaml.load(open(definition)))
