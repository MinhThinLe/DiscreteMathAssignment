excercise_1:
	@gcc -Wall -Werror ex1/main.c  ex1/vector.c -o main
	@./main > result.txt
	@echo Thuật toán đã chạy xong, kết quả đã được viết vào result.txt

test:
	echo $(STDLFLAGS)
