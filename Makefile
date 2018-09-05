CXXFLAGS=-Igen/ -Iinclude/
.PRECIOUS: gen/%.cpp gen/%.h
main: gen/token.o gen/judgment.o gen/rule.o gen/substitution.o gen/metavar.o gen/shape.o gen/naming.o gen/type.o gen/axiom.o gen/proof.o

gen/:
	mkdir gen

gen/%_ptr.h: structs/%.yaml gen/
	python2 jinjafy.py struct_ptr_template.h $< > $@
	clang-format -i $@

gen/%.h: structs/%.yaml gen/ gen/%_ptr.h
	python2 jinjafy.py struct_template.h $< > $@
	clang-format -i $@

gen/%.cpp: structs/%.yaml gen/%.h
	python2 jinjafy.py struct_template.cpp $< > $@
	clang-format -i $@
