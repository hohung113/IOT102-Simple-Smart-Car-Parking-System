 // Lấy mã thẻ
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10
byte readCard[4];
byte a = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Khai báo một đối tượng LiquidCrystal_I2C với địa chỉ I2C 0x27 và màn hình 16x2.
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Khai báo một đối tượng MFRC522 với chân SS_PIN và RST_PIN.

void setup() {
  Serial.begin(9600);  // Bắt đầu giao tiếp serial với tốc độ 9600 baud.
  lcd.init();  // Khởi tạo màn hình LCD.
  lcd.backlight();  // Bật đèn nền của màn hình LCD.
  while (!Serial);  // Đợi cho serial được kết nối trước khi tiếp tục.
  SPI.begin();  // Khởi tạo giao tiếp SPI.
  mfrc522.PCD_Init();  // Khởi tạo mô-đun đọc thẻ RFID.
  delay(4);  // Đợi 4 ms.
  mfrc522.PCD_DumpVersionToSerial();  // In thông tin phiên bản MFRC522 ra serial monitor.
  lcd.setCursor(2, 0);
  lcd.print("Put your card");  // Hiển thị thông điệp trên màn hình LCD.
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {  // Kiểm tra xem có thẻ RFID mới nào được đặt lên đầu mô-đun hay không.
    return 0;  // Nếu không có thẻ mới, thoát khỏi hàm loop.
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {  // Đọc dữ liệu từ thẻ RFID.
    return 0;  // Nếu không đọc được thẻ, thoát khỏi hàm loop.
  }

  lcd.clear();  // Xóa nội dung trên màn hình LCD.
  lcd.setCursor(0, 0);
  lcd.print("Scanned UID");
  a = 0;
  Serial.println(F("Scanned PICC's UID:"));
  for ( uint8_t i = 0; i < 4; i++) {
    readCard[i] = mfrc522.uid.uidByte[i];  // Lưu dữ liệu UID của thẻ RFID vào mảng readCard.
    Serial.print(readCard[i], HEX);  // In UID ra serial monitor dưới dạng số hex.
    Serial.print(" ");
    lcd.setCursor(a, 1);
    lcd.print(readCard[i], HEX);  // Hiển thị UID lên màn hình LCD dưới dạng số hex.
    lcd.print(" ");
    delay(500);
    a += 3;
  }
  Serial.println("");
  mfrc522.PICC_HaltA();  // Tắt thẻ RFID.
  return 1;
}
