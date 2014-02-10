all: optgenc

optgenc: xmlstuff.o rbtreestuff.o
	cc -O2 -o optgenc -Wall xmlstuff.o rbtreestuff.o -lproc

xmlstuff.o: optgenc.c 
	cc -O2 -o xmlstuff.o -c -Wall optgenc.c


rbtreestuff.o: rbtree.cpp redblack.hpp
	g++ -O2 -o rbtreestuff.o -c -Wall rbtree.cpp
