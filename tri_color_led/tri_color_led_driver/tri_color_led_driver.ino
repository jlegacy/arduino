//testing commits//

int greenPin = 9;
int redPin = 10;
int bluePin = 11;


int greenDir = 0;
int blueDir = 0;
int redDir = 0;
int potPin = 2;

int greenValue = 0;
int blueValue = 0;
float redValue = 0;

int randomNumber = 0;
int val = 0;
long count = 0;



void setup()
{
  randomizeColor();
  randomizeDirection();
}

void randomizeColor()
{
  //randomize start color//
  randomSeed(analogRead(0));
  greenValue = random(200);
  blueValue = random(200);
  redValue = random(160);
}

void randomizeDirection()
{
  //randomize directions//
  randomSeed(analogRead(0));
  greenDir = random(2);
  blueDir = random(2);
  redDir = random(2);
}

void loop()
{
  val = analogRead(potPin);
  checkTimer();
  delay(val/5);

  //after 1000 cycles randomize direction//
  count += 1;
  if (count > 1000)
  {
    count = 0;
    randomizeDirection();
  }
}

void checkTimer()
{

  //either count up or down
  //gives fading look//

  randomSeed(analogRead(0));
  greenValue = greenDir == 0 ? greenValue + random(3) : greenValue - random(3);
  randomSeed(analogRead(0));
  blueValue = blueDir == 0 ? blueValue + random(3) : blueValue - random(3);
  randomSeed(analogRead(0));
  redValue = redDir == 0 ? redValue + random(2) : redValue - random(2);

  if (greenValue > 204)
  {
    greenDir = 1;
    greenValue = 204;
  }

  if (greenValue < 0)
  {
    greenDir = 0;
    greenValue = 0;
  }


  if (blueValue > 204)
  {
    blueDir = 1;
    blueValue = 204;
  }
  if (blueValue < 0)
  {
    blueDir = 0;
    blueValue = 0;
  }



  if (redValue > 168)
  {
    redDir = 1;
    redValue = 168;
  }
  if (redValue < 0)
  {
    redDir = 0;
    redValue = 0;
  } 

  analogWrite(greenPin, greenValue);  
  analogWrite(bluePin, blueValue);  
  analogWrite(redPin, redValue);  

}





























