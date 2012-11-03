#include <FlexiTimer2.h>
//testing commits//
void setup()
{
  pinMode(A0, INPUT);
  pinMode(4, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(4,LOW);
  FlexiTimer2::set(1, 1.0/1000, checkPins); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
 Serial.begin(9600);
}

void loop()
{
}

void checkPins()
{
  
  int val1 = analogRead(A0); 
  int val2 = analogRead(A1);
  
 if ((val1 <= 100) && (val2 >= 1000))
  {
    digitalWrite(4,LOW);
    digitalWrite(13,LOW);
  }
  
  if ((val1 <= 100) && (val2 <= 100))
  {
    digitalWrite(4,HIGH);
    digitalWrite(13,HIGH);
  }

 




}





















