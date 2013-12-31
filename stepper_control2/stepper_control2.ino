#include <Stepper.h>
#include <EEPROM.h>
/* This Rugged Motor Shield application demonstrates keyboard control
 * of a stepper motor. Assumptions:
 *
 *   - stepper motor connected to the 2 phases
 *   - 8V-30V supply connected to Vin (optional, else Arduino Vin is used)
 *   - Vin jumper is cut (J21) (only required if Vin>15V)
 *   - FAULT1 and FAULT2 jumpers (J10/J14) installed (optional)
 *   - LED jumpers (J15, J16, J6, J7) installed (optional)
 *
 * The behavior is as follows:
 *
 *   - The 'f' key sets forward motion
 *   - The 'b' key sets backwards motion
 *   - The 'a' key accelerates rotational speed
 *   - The 'd' key decelerates rotation speed
 *   - The 'o' key increases motor power (PWM higher duty cycle)
 *   - The 'm' key decreases motor power (PWM lower duty cycle)
 * 
 * This software is licensed under the GNU General Public License (GPL) Version
 * 3 or later. This license is described at
 * http://www.gnu.org/licenses/gpl.html
 *
 * Application Version 1.0 -- October 2010 Rugged Circuits LLC
 * http://www.ruggedcircuits.com
 */

byte xIndex = 0;
byte yIndex = 0;

unsigned int lookupTable[64][4] = 
{
  {
    251,0,251,0  }
  ,
  {
    238,0,251,0  }
  ,
  {
    217,0,251,0  }
  ,
  {
    188,0,251,0  }
  ,
  {
    154,0,251,0  }
  ,
  {
    114,0,251,0  }
  ,
  {
    70,0,251,0  }
  ,
  {
    24,0,251,0  }
  ,
  {
    0,24,251,0  }
  ,
  {
    0,70,251,0  }
  ,
  {
    0,114,251,0  }
  ,
  {
    0,154,251,0  }
  ,
  {
    0,188,251,0  }
  ,
  {
    0,217,251,0  }
  ,
  {
    0,238,251,0  }
  ,
  {
    0,251,251,0  }
  ,
  {
    0,251,251,0  }
  ,
  {
    0,251,238,0  }
  ,
  {
    0,251,217,0  }
  ,
  {
    0,251,188,0  }
  ,
  {
    0,251,154,0  }
  ,
  {
    0,251,114,0  }
  ,
  {
    0,251,70,0  }
  ,
  {
    0,251,24,0  }
  ,
  {
    0,251,0,24  }
  ,
  {
    0,251,0,70  }
  ,
  {
    0,251,0,114  }
  ,
  {
    0,251,0,154  }
  ,
  {
    0,251,0,188  }
  ,
  {
    0,251,0,217  }
  ,
  {
    0,251,0,238  }
  ,
  {
    0,251,0,251  }
  ,
  {
    0,251,0,251  }
  ,
  {
    0,238,0,251  }
  ,
  {
    0,217,0,251  }
  ,
  {
    0,188,0,251  }
  ,
  {
    0,154,0,251  }
  ,
  {
    0,114,0,251  }
  ,
  {
    0,70,0,251  }
  ,
  {
    0,24,0,251  }
  ,
  {
    24,0,0,251  }
  ,
  {
    70,0,0,251  }
  ,
  {
    114,0,0,251  }
  ,
  {
    154,0,0,251  }
  ,
  {
    188,0,0,251  }
  ,
  {
    217,0,0,251  }
  ,
  {
    238,0,0,251  }
  ,
  {
    251,0,0,251  }
  ,
  {
    251,0,0,251  }
  ,
  {
    251,0,0,238  }
  ,
  {
    251,0,0,217  }
  ,
  {
    251,0,0,188  }
  ,
  {
    251,0,0,154  }
  ,
  {
    251,0,0,114  }
  ,
  {
    251,0,0,70  }
  ,
  {
    251,0,0,24  }
  ,
  {
    251,0,24,0  }
  ,
  {
    251,0,70,0  }
  ,
  {
    251,0,114,0  }
  ,
  {
    251,0,154,0  }
  ,
  {
    251,0,188,0  }
  ,
  {
    251,0,217,0  }
  ,
  {
    251,0,238,0  }
  ,
  {
    251,0,251,0  }
};

// Define how many steps there are in 1 revolution of your motor
#define STEPS_PER_REVOLUTION 200

/**********************************************************/
/* YOU SHOULDN'T HAVE TO CHANGE ANYTHING BELOW THIS POINT */
/**********************************************************/

// Enable (PWM) outputs
#define A_NEG_PIN 3
#define B_NEG_PIN 11
#define A_POS_PIN 13
#define B_POS_PIN 12

void fullsteps(unsigned steps);
void halfstepit(unsigned steps, unsigned dir);
void fullstepit(unsigned steps, unsigned dir);
void clearindata(char* indata);
void processdata(char* indata);

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

char start = '|';
char stop = '^';

boolean started = false;
boolean finished = false;
char buffer;


// Set initial default values
void setup()
{
  // Configure all outputs off for now
  pinMode(A_POS_PIN, OUTPUT); 
  digitalWrite(A_POS_PIN, LOW);
  pinMode(A_NEG_PIN, OUTPUT); 
  digitalWrite(A_NEG_PIN, LOW);
  pinMode(B_POS_PIN, OUTPUT); 
  digitalWrite(B_POS_PIN, LOW);
  pinMode(B_NEG_PIN, OUTPUT); 
  digitalWrite(B_NEG_PIN, LOW);

  Serial.begin(9600);

  ClrEepromPos();

  // Change from divide-by-64 prescale on Timer 2 to divide by 8 to get
  // 8-times faster PWM frequency (976 Hz --> 7.8 kHz). This should prevent
  // overcurrent conditions for steppers with high voltages and low inductance.
  TCCR2B = _BV(CS21);



}

void printHelp()
{
  Serial.println(
  "f - forward motion\n"
    "b - backwards motion\n"
    "a - faster\n"
    "d - slower\n"
    "o - more power\n"
    "m - less power\n"
    );
}

void loop()
{

  analogWrite(A_POS_PIN, LOW);
  analogWrite(A_NEG_PIN, LOW);

  analogWrite(B_POS_PIN, LOW);
  analogWrite(B_NEG_PIN, LOW);

  delay(50);

  int i = 0;
  while (Serial.available() > 0) {
    buffer = Serial.read();
    //start bit//
    if (buffer == '|')
    {
      inData[0] = buffer;
      continue;
    }

    i = i + 1;
    inData[i] = buffer;
    inData[i+1] = '\0';

    //Stop bit// 
    if (buffer == '^')
    {
      process(inData);
      break;
    }

  }
}

void process(char* inData)
{
  Serial.println("processing");
  int j=0; 
  char inData2[20];
  //Strip of start and stop bits//
  for (int i=0;i<20;i++)
  {
    if ((inData[i] != '|') && (inData[i] != '^'))
    {
      inData2[j] = inData[i];
      inData2[j + 1] = '\0';
      j = j + 1;
    }
  }

  int i = strcmp( inData2, "Full" );
  if (i == 0)
  {
    steps(800,'full');
  }

  i = strcmp( inData2, "Quarter" );  
  if (i == 0)   {
    steps(800,'quarter');
  }

  i = strcmp( inData2, "Half" );
  if (i == 0)
  {
    steps(400,'half');
  }

  i = strcmp( inData2, "Eight" );
  if (i == 0)
  {
    steps(1600,'eight');
  }


  i = strcmp( inData2, "Sixteen" );
  if (i == 0)
  {
    steps(3200,'sixteen');
  }

  clearindata(inData);
  clearindata(inData2);
}

void clearindata(char* inData)
{
  for(int i = 0;i<20;i++)  
  {  
    inData[i] = 0; // copying data from buffer  
  }  
  memset(inData,0,sizeof(inData));
}


void steps(signed steps, char mode)
{
  unsigned stepmode = 0; 
  switch (mode) {

  case 'full':
    //full step
    stepmode = 16;
    break;

  case 'half':
    //half step
    stepmode = 8;
    break;

  case 'quarter':
    //quarter step
    stepmode = 4;
    break;

  case 'eight':
    //eight step
    stepmode = 2;
    break;

  case 'sixteen':
    //eight step
    stepmode = 1;
    break;

  default: 
    break;
  }

  signed cnt = 0;

  if (steps > 0)
  {
    //forward//
    signed i = GetEepromPos();
    do
    {

      i = i + stepmode;
      signed remainder = i - 63;
      if (i > 63)
      {
        i = remainder - 1;
      }
      unsigned j = i + 1;
      analogWrite(A_POS_PIN, lookupTable[i][0]);
      analogWrite(A_NEG_PIN, lookupTable[i][1]);
      analogWrite(B_POS_PIN, lookupTable[i][2]);
      analogWrite(B_NEG_PIN, lookupTable[i][3]);
      
      Serial.println(i);
      
      cnt = cnt + 1;
     delay(3);        


    } 
    while (cnt < steps);
    SetEepromPos(i);
  }

  if (steps < 0)
  {

    //reverse//
    signed i = GetEepromPos();
    do
    {
      i = (i  - stepmode);
      signed remainder = 63 + i;
      if (i < 0)
      {
        i = remainder + 1;
      }
      unsigned j = i + 1;

      analogWrite(A_POS_PIN, lookupTable[i][0]);
      analogWrite(A_NEG_PIN, lookupTable[i][1]);
      analogWrite(B_POS_PIN, lookupTable[i][2]);
      analogWrite(B_NEG_PIN, lookupTable[i][3]);
      cnt = cnt - 1;
     delay(3);  


    } 
    while (cnt > steps);
    SetEepromPos(i);
  }
}

//Stores last position of step//
int GetEepromPos(void)
{
  int value = EEPROM.read(0);
  return value;
}

void ClrEepromPos(void)
{
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
}

void SetEepromPos(unsigned i)
{
  EEPROM.write(0, i);
}























