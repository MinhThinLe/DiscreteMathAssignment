excercise_1:
	gcc ex1/main.c ex1/lib.c ex1/vector.c -o main
	./main

test:
	echo $(STDLFLAGS)
