#define AC_PIN 9	// Output to Opto Triac
#define LED 13		// builtin LED for testing
long counter = 0;
unsigned long currenttime = 0;
unsigned long previoustime = 0;
unsigned long elapsedtime = 0;

void setup()
{
    Serial.begin(115200);	//start the serial port at 115200 baud we want
    Serial.println("AC Motor Control v1");	//the max speed here so any
    pinMode(AC_PIN, OUTPUT);		// Set the Triac pin as output
    attachInterrupt(1, zero_cross_detect, RISING); 	// Attach an Interupt to Pin 3 (interupt 1) for Zero Cross Detection
} 


void loop()
{
//Get ADC Reading Calculate Active Up

}


void zero_cross_detect()	// function to be fired at the zero crossing.  This function
{	
    //set 50% as a test//
    unsigned long delaytime = 8333 * (1 - .99);
    setdimmer(delaytime);
}  
  
void setdimmer(unsigned long time)
{
  
  digitalWrite(AC_PIN, LOW);
  delayMicroseconds(time);
  digitalWrite(AC_PIN, HIGH);
}
		// End zero_cross_detect


