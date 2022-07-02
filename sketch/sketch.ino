#define DELAY_MS 500
int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
      incomingByte = Serial.read();
      Serial.write(incomingByte);  
  }

   Serial.print(":");	
   Serial.print(analogRead(A0));  // Вывод показателя температурного датчика А0
   Serial.print(":");
   Serial.print(analogRead(A1));  // Вывод показателя датчика давления А1
   Serial.flush();                // Зачищение буфера для отправки новых данных
   delay(DELAY_MS);
}
