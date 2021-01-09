# Bài tập lớn môn lập trình mạng 

## Đề tài: Trò chơi Bingo trực tuyến 

### Giới thiệu đề tài
“Trò chơi Bingo” là một đề tài được tạo ra nhằm nghiên cứu, tìm hiểu các vấn đề lập trình mạng, gồm 2 chương trình, một là chạy cho server, một là dành cho client. Người chơi dùng chương trình dành cho client kết nối với server đã bật trước đó để cùng với người khác chơi bingo với nhau thông qua cùng một mạng.

### Các chức năng
* Đăng nhập, đăng ký tài khoản
* Xem các thông tin về luật chơi, nhóm làm game
* Tạo phòng chơi, vào phòng bằng ID phòng, vào phòng nhanh
* Mời người chơi khác, kick người chơi khác 
* Sẵn sàng và chơi game

### Công cụ sử dụng
* Ngôn ngữ: C
* Thư viện: GTK
* RAD Tool: Glade

### Hướng dẫn cài đặt và sử dụng
#### Chuẩn bị
* Ubuntu với thư viện Glib phiên bản 2.23 trở lên
* Các người chơi phải chung một mạng LAN

#### Cài đặt
* Cài đặt Glade và các gói GTK
```
$ sudo apt update
$ sudo apt install snapd
$ sudo snap install glade
$ sudo apt-get install libgtk-3-dev
```

* Biên dịch 
```
$ make
```

#### Chơi game
##### Server
* Gõ lệnh sau trên terminal để kiểm tra địa chỉ IP  của  server
```
$ ifconfig
```
* Khởi động server
```
$ ./server [PORT] 
```
Ví dụ
```
$ ./server 8080
```

##### Client
* Kết nối tới server bằng ip của máy chạy server và port  của server
```
$ ./client [SERVER_IP] [PORT]
```
Ví dụ
```
$ ./client 192.168.1.102 8080
```