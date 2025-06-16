compiler = gcc -O3 -march=native -mtune=native -Wall -Werror -o main

exercise_1:
	@$(compiler) ex1/main.c lib/vector.c
	@./main > result.txt
	@echo Thuật toán đã chạy xong, kết quả đã được viết vào result.txt

exercise_2:
	@$(compiler) ex2/main.c lib/vector.c
	@./main

exercise_3:
	@$(compiler) ex3/main.c lib/vector.c
	@./main
