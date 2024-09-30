#define ledPin 7

void setup() {
  pinMode(ledPin, OUTPUT);  // LED 핀을 출력 모드로 설정
    Serial.begin(115200); // Initialize serial port
  while (!Serial) {
    ; // wait for serial port to connect.
  }
}

void loop() {
  // 처음 1초 동안 LED 켜기
  digitalWrite(ledPin, LOW);  // LED 켜기
  delay(1000);  // 1초 동안 대기
  
  // 다음 1초 동안 LED 5회 깜빡이기
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);   // LED 끄기
    delay(100);                  // 100ms 대기
    digitalWrite(ledPin, LOW);  // LED 켜기
    delay(100);                  // 100ms 대기
  }

  // LED 끄기
  digitalWrite(ledPin, HIGH);  // LED 끄기

  // 무한루프 상태에서 종료
  while (1) {
    // 무한루프는 아무 동작도 하지 않음
  }
}
