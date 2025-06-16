#set text(
  font: "Latin Modern Roman 12",
  size: 14pt,
)

#show raw: set text(
  font: "JetBrainsMono NF"
)

#set page(
  margin: 1.8cm
)

= Hướng dẫn chạy bài tập lập trình toán rời rạc 
\

Cách khuyến nghị để chạy bài tập này có ở trên
#underline()[#link("https://github.com/MinhThinLe/DiscreteMathAssignment")[github]].
Những cách khác là hoàn toàn khả thi. Tuy nhiên, chúng không được khuyến nghị.

Một cách ngắn gọn, cách chạy chương trình như sau:

- Clone repo với git
```bash
git clone https://github.com/MinhThinLe/DiscreteMathAssignment
```
- Chạy bài tập số n với
```bash
make exercise_n
```

Chỉ với bài tập 1, dữ liệu cần để vẽ đồ thị sẽ được viết vào file `result.txt`.
Dán nội dung file đó vào `ex1/graph.gv` và vẫn từ thư mục root của project, chạy
```bash
dot -Tpng ex1/graph.gv -o result.png
```


= Các test case đã chạy thành công
\
Trước khi bắt đầu, lưu ý rằng kết quả test sẽ không giống so với kết quả nhận
được khi chạy chương trình do những đoạn code không liên quan đến phần đó
sẽ bị comment để không gây nhiễu đầu ra chương trình. Cuối mỗi mục test sẽ
là kết quả khi chạy với tất cả code đã được bỏ chú thích.

== Bài tập 1.
\
Bài tập này không nhận dữ liệu đầu vào nên không có test case nào cả, kết
quả khi chạy chương trình như sau

```
programming-assignment  ➜  ./main 
_0_7_4_ -> _7_0_4_
_7_0_4_ -> _10_0_1_
_10_0_1_ -> _3_7_1_
_3_7_1_ -> _3_4_4_
_3_4_4_ -> _7_4_0_
_7_4_0_ -> _4_7_0_
_4_7_0_ -> _10_1_0_
_10_1_0_ -> _6_1_4_
_6_1_4_ -> _6_5_0_
_6_5_0_ -> _2_5_4_
_2_5_4_ -> _2_7_2_
```

Vì bài tập 1 chỉ có một yêu cầu nên đây là toàn bộ kết quả khi chạy chương
trình

== Bài tập 2.
\

a) Phần a không nhận đầu vào nên không có test case cho mục a, kết quả khi
chạy chương trình như sau
```
  programming-assignment  ➜  make exercise_2
  Có 182 thành phần liên thông trong đồ thị
```
b)
  - Bắt đầu với `rooks` và kết thúc với `bears`
  ```
  programming-assignment  ➜  make exercise_2
  Chọn điểm bắt đầu: rooks
  Chọn điểm kết thúc: bears
  Đường đi ngắn nhất từ rooks đến bears như sau
  bears <- boars <- boors <- books <- rooks 
  ```
  - Bắt đầu với `loads` và kết thúc với `zebra`
  ```
  programming-assignment  ➜  make exercise_2
  Chọn điểm bắt đầu: loads
  Chọn điểm kết thúc: zebra
  Không tồn tại đường đi từ loads đến zebra trong đồ thị này
  ```
  - Bắt đầu với `froat` và kết thúc với `nodes`
  ```
  programming-assignment  ➜  make exercise_2
  Chọn điểm bắt đầu: froat
  Chọn điểm kết thúc: nodes
  Đồ thị không chứa froat
  ```

Kết quả khi chạy toàn bộ chương trình
```
programming-assignment  ➜  make exercise_2
Có 182 thành phần liên thông trong đồ thị
Chọn điểm bắt đầu: loads
Chọn điểm kết thúc: brood
Đường đi ngắn nhất từ loads đến brood như sau
brood <- blood <- blond <- bland <- gland <- glans <- glads <- goads <- loads 
```

#pagebreak()

== Bài tập 3.
\

a) Phần a không nhận đầu vào nên không có test case cho mục a, kết quả khi 
chạy chương trình như sau
```
programming-assignment  ➜  make exercise_3
Số thành phần liên thông mạnh có trong đồ thị là 430
```

b) Vì trong đồ thị có một thành phần liên thông chứa trên 4000 phần tử nên để tránh
việc báo cáo này dài 10 trang, tất cả mọi test case dưới đây sẽ tránh thành
phần liên thông đó.

- Với đầu vào là `block`

```
  programming-assignment  ➜  make exercise_3
  Nhập từ cần tìm thành phần liên thông mạnh: block
  Các từ cùng thành phần liên thông mạnh với block là:
  clock
  flock
```
- Với đầu vào là `apple`
```
  programming-assignment  ➜  make exercise_3
  Nhập từ cần tìm thành phần liên thông mạnh: apple
  Từ apple không nằm trong thành phần liên thông mạnh nào cả
```

- Với đầu vào là `frott`

```
  programming-assignment  ➜  make exercise_3
  Nhập từ cần tìm thành phần liên thông mạnh: frott
  Đồ thị không chứa frott
```

c)
- Với từ bắt đầu là `human` và kết thúc là `looks`

```
  programming-assignment  ➜  make exercise_3
  Chọn điểm bắt đầu: human
  Chọn điểm kết thúc: looks
  Đường đi ngắn nhất từ human đến looks như sau
  looks <- rooks <- moors <- norms <- arson <- manor <- unarm <- human 
```

- Với từ bắt đầu là `mores` và kết thúc là `kneel`
```
  programming-assignment  ➜  make exercise_3
  Chọn điểm bắt đầu: mores
  Chọn điểm kết thúc: kneel
  Đường đi ngắn nhất từ mores đến kneel như sau
  kneel <- sleek <- glees <- ogles <- roles <- mores 
```

- Với từ bắt đầu là `alose` và kết thúc là `breed`

```
  programming-assignment  ➜  make exercise_3
  Chọn điểm bắt đầu: alose
  Chọn điểm kết thúc: breed
  Đồ thị không chứa alose
```

Kết quả khi chạy toàn bộ chương trình

```
programming-assignment  ➜  make exercise_3
Số thành phần liên thông mạnh có trong đồ thị là 430
Nhập từ cần tìm thành phần liên thông mạnh: would
Các từ cùng thành phần liên thông mạnh với would là:
could
cloud
aloud
mould
Chọn điểm bắt đầu: cloud
Chọn điểm kết thúc: arose 
Đường đi ngắn nhất từ cloud đến arose như sau
arose <- roses <- shoes <- echos <- hocus <- scudo <- could <- cloud 
```
#pagebreak()

Thư viện đã sử dụng:

Vector của #underline()[#link("https://github.com/goldsborough/vector")[Peter Goldsborough]]
