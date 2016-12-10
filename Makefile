CXX=g++
FLAGS=-O3 -funroll-loops -DNDEBUG -pipe -march=native -I. -Wa,-q -fpic

all: test libhll.a

libhll.a: hll.o
	ar cr $@ $<

hll.o: hll.cpp
	$(CXX) -c $(FLAGS)	$< -o $@

test: test.cpp hll.o
	$(CXX) $(FLAGS)	hll.o $< -o $@

clean:
	rm -f test.o test hll.o libhll.a