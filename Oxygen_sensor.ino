/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

void setup() {
  //Set internal voltage reference to 1.1
  analogReference(INTERNAL);
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH); 
  Serial.begin(9600);
  int total = 0;
}

void loop() 
  
  {
  double total = 0;
  float result = 0;
  for (long count=0;count<10000;count++)
    {
      int sensorValue = analogRead(A0);
      total = total + sensorValue;
    }
    result = float(total/10000) * .001075;
   // result = ((total/10000) * .001075);
    Serial.print("\r		 \r");  
    Serial.println(total);
    Serial.print("\r		 \r");  
    Serial.println(result);
    }

