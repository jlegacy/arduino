
#include <TimerOne.h>

/* Toggle HIGH or LOW digital write */
int toggle = 0; 
int inPin = 0;  
volatile int bitCount = 0;
volatile char processedBits[13] = {
  '0','0','0','0','0','0','0','0','0','0','0','0','0'};
volatile int threshold;
volatile int bitcheck = LOW;

void setup()
{
  pinMode(inPin, INPUT);
  
  Serial.begin(9600);
}  

void loop()
{
  threshold = analogRead(inPin); //Write the value of the photoresistor to the serial monitor.
  if (threshold > 1000)
  {
    start();
  }
}

void start()
{
  //detachInterrupt(0);
  Timer1.initialize(1778); // set a timer of length 889 microseconds
  Timer1.attachInterrupt(timerIsr); // attach the service routine here
}

void timerIsr()
{
  
  bitCount++;
  int x=0;
  do
  {
    x++;
    threshold = analogRead(inPin); 

    if (threshold >  1000)
    {
      break;
    }

  } 
  while (x < 100); 


  if (threshold >  1000)
  {
    processedBits[bitCount] = '1';
  }

  if (bitCount == 13)
  {
    Timer1.detachInterrupt(); // detach the service routine here
    bitCount = 0;
    Serial.println(processedBits[2] + ' ' + processedBits[3] + ' ' +  processedBits[4] + ' ' +  processedBits[5]  + ' ' + processedBits[6]);
    Serial.println(processedBits[7] + ' ' + processedBits[8] + ' ' + processedBits[9] + ' ' + processedBits[10] + ' ' + processedBits[11] + ' ' + processedBits[12]);
    processedBits[13] = '0','0','0','0','0','0','0','0','0','0','0','0','0';

   // attachInterrupt(0, start, RISING);
  }


}








