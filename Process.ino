#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID1 = "83 F8 2F AD";  // thay đổi mã code của bạn
String UID2 = "AD D8 D2 CF";  // thay đổi mã code của bạn
int totalSlots = 5;
int availableSlots = totalSlots;

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
int servoPosition = 70;
bool welcomeDisplayed = false;
bool parkingEmptyDisplayed = false;

void setup() {
  Serial.begin(9600);
  servo.write(servoPosition);
  lcd.init();
  lcd.backlight();
  servo.attach(3);
  SPI.begin();
  rfid.PCD_Init();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Parking ");
  lcd.setCursor(0, 1);
  lcd.print("Slots left ");
  lcd.print(availableSlots);
  lcd.print("/");
  lcd.print(totalSlots);

}
void loop() {
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;

  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
  }
  ID.toUpperCase();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning...");
  delay(1000);

 if (ID.substring(1) == UID1) { // Thẻ vào
    if (availableSlots > 0) {
      availableSlots--;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome Parking ");
      lcd.setCursor(0, 1);
       lcd.print("Slots left ");
      lcd.print(availableSlots);
      lcd.print("/");
      lcd.print(totalSlots);
      delay(1000);

      // Quay servo 180 độ
      servo.write(180);
      delay(3000); // Đợi 3 giây

      // Quay servo trở về vị trí cũ
      servo.write(servoPosition);
    } else {
      lcd.clear();
       lcd.setCursor(0, 0);
      lcd.print("Welcome Parking ");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full");
      delay(1000);
    }
}

  } else if (ID.substring(1) == UID2) { // Thẻ ra
    if (availableSlots < totalSlots) {
      availableSlots++;
      lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("Welcome Parking ");
      lcd.setCursor(0, 1);
       lcd.print("Slots left ");
      lcd.print(availableSlots);
      lcd.print("/");
      lcd.print(totalSlots);
      delay(1000);

      // Quay servo 180 độ
      servo.write(180);
      delay(3000); // Đợi 3 giây

      // Quay servo trở về vị trí cũ
      servo.write(servoPosition);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome Parking ");
      lcd.setCursor(0, 1);
      lcd.print("Parking Empty");
 
      delay(1000);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid Card");
    lcd.setCursor(0, 1);
    lcd.print("Scanning Again");
    delay(1000);
  }

  if (availableSlots == totalSlots) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome Parking lot");
    welcomeDisplayed = true;
    parkingEmptyDisplayed = false;
  } else if (availableSlots > 0 && parkingEmptyDisplayed) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Welcome Parking ");
    lcd.setCursor(0, 1);
    welcomeDisplayed = true;
    parkingEmptyDisplayed = false;
  }
