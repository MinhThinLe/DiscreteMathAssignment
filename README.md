# Bài Tập Lập Trình Toán Rời Rạc

Repo này chứa lời giải cho bài tập lập trình của học phần Toán rời rạc IT3020.

Phụ thuộc:  
+ make
+ gcc

Hai chương trình này nên được cài sẵn trên phần lớn hệ thống chạy Linux, nếu vì
lý do gì đó, hai chương trình này không được cài sẵn trên hệ thống của bạn, hãy
làm theo hướng dẫn của hệ điều hành của bạn và cài đặt chúng.

Để chạy chương trình, hãy clone repo này với git
```
git clone https://github.com/MinhThinLe/DiscreteMathAssignment.git 
```

Sau đó, cd vào thư mục chứa mã nguồn với
``` 
cd DiscreteMathAssignment 
```

Để thực thi chương trình của bài tập 1, hãy chạy lệnh sau
```
make exercise_1
```

Lưu ý rằng sau khi chạy bài tập 1, kết quả sẽ được ghi vào file `result.txt`.
Dán nội dung file đó vào `ex1/graph.gv` và biên dịch nó với graphviz để có được
đồ thị mô tả cây DFS.

Với bài tập 2, chạy
``` 
make exercise_2
```

Để thực thi chương trình của bài tập 2.  
Và cuối cùng.
```
make exercise_3
```

Để chạy chương trình của bài tập 3.

Chương trình có sử dụng thư viện vector của [Peter
Goldsborough](https://github.com/goldsborough/vector)
