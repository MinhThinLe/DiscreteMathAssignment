exercise_1:
	gcc -Wall -Werror ex1/main.c ex1/vector.c -o main
	./main > result.txt
	echo Thuật toán đã chạy xong, kết quả đã được viết vào result.txt

exercise_2:
	@gcc -Wall -Werror ex2/main.c ex2/vector.c -o main
	@./main
