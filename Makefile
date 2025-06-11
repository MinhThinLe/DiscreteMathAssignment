excercise_1:
	gcc -Wall -Werror ex1/main.c  ex1/vector.c -o main
	./main

test:
	echo $(STDLFLAGS)
