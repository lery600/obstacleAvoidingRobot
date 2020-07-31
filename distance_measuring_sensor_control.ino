#include <iarduino_HC_SR04.h>           // Подключаем библиотеку
iarduino_HC_SR04 sensor(2,3);           // Объявляем объект sensor, указывая номера arduino подключенные к выводам TRIG и ECHO датчика
                                        // Можно использовать любые выводы Arduino
void setup(){
  Serial.begin(9600);                   // Инициализация передачи данных в монитор последовательного порта
}
void loop(){
  delay(100);                           // Задержка 0,5 сек
  Serial.println(sensor.distance()   ); // Вывод расстояния (см) при температуре воздуха, около +23 °C
 if (sensor.distance() <= 4){
  digitalWrite(11, HIGH);
 }
 
 else {
  digitalWrite(11, LOW);
 }
}
