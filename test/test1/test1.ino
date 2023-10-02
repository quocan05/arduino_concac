/*
 ***Các lệnh cơ bản
 *
  AT: Kiểm tra kết nối
  AT+ORGL: Reset lại cài đặt mặc định
  AT+RMAAD: Xóa mọi thiết bị đã ghép nối
  AT+UART?: Kiểm tra baudrate hiện tại của module
  AT+UART=9600,0,0: Thiết lặp baudrate mới là 9600 cho module
  AT+NAME?: Kiểm tra tên hiện tại của module
  AT+NAME=tên: Đặt tên khác cho module
  AT+PSWD?: Kiểm tra mật khẩu hiện tại của module
  AT+PSWD=3453: Đặt mật khẩu mới cho module
  
  
 ***Cài đặt cho SLAVE***
 *
  AT: Lệnh test, nó sẽ trả về OK nếu module đã hoạt động ở Command Mode
  AT+RMAAD: Xóa mọi thiết bị đã ghép nối
  AT+UART=9600,0,0 ( thiết lập baudrate Bluetooth 34800 ; baud của SLAVE và MASTER phải giống nhau)
  AT+PSWD=1234 (đặt mật khẩu cho Slave là 1234, MK của SLAVE và MASTER phải giống nhau)
  AT+ROLE=0: Đặt là chế độ SLAVE
  AT+ADDR?: Hiển thị địa chỉ của SLAVE. Ví dụ địa chỉ là 123:34:232421 thì đổi dấu ":" thành dấu "," để nhập vào MASTER

 ***Cài đặt cho MASTER***
 *
  AT: Lệnh test, nó sẽ trả về OK nếu module đã hoạt động ở Command Mode
  AT+RMAAD : ngắt kết nối với các thiết bị đã ghép
  ***AT+UART=9600,0,0 ( thiết lập baudrate Bluetooth 9600 ; baud của SLAVE và MASTER phải giống nhau)
  ***AT+PSWD=1234 (đặt mật khẩu cho Slave là 1234, MK của SLAVE và MASTER phải giống nhau)
  AT+ROLE=1 : đặt là module ở MASTER
  AT+CMODE=0: Cho phép kết nối với bất kì địa chỉ nào
  AT+INIT: khởi tạo cấu hình SPP. Nếu SPP đã hoạt động, bạn sẽ gặp lỗi mà bạn có thể bỏ qua.
  
  AT+INQ: Tìm các thiết bị bluetooth xung quanh có thể kết nối
    - Khi chạy INQ sẽ hiện ra các địa chỉ bluetooth đang hoạt động
  
  AT+RNAME?xxxx,xx,xxxxxx: Hiện tên của 1 địa chỉ bluetooth bất kỳ (Đổi dấy : thành ,)
Lệnh AT+RNAME chỉ chạy khi đã khởi tạo AT+INIT thành công
    - Ví dụ: AT+RNAME?0020,12,08BEAB sẽ hiện ra tên Slave 
  
  AT+BIND=address Ràng buộc Master với Slave
    - Ví dụ: AT+BIND=0020,12,08BEAB

  AT+LINK=address Kết nối Master với Slave
    - Ví dụ: AT+LINK=0020,12,08BEAB
  
 
 */
void setup() 
{
  // put your setup code here, to run once:

}

void loop() 
{
  // put your main code here, to run repeatedly:

}
