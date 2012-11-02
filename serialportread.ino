/* ------------------------------------------------
* SERIAL COM - HANDELING MULTIPLE BYTES inside ARDUINO - 03_function development
* by beltran berrocal
*
* this prog establishes a connection with the pc and waits for it to send him
* a long string of characters like "hello Arduino!". 
* Then Arduino informs the pc that it heard the whole sentence
*
* the same as examlpe 02 but it deploys 2 reusable functions.
* for doing the same job. 
* readSerialString() and  printSerialString()
* the only problem is that they use global variables instead of getting them passed 
* as parameters. this means that in order to reuse this code you should also copy
* the 4 variables instantiated at the beginning of the code.
* Another problem is that if you expect more than one string at a time 
* you will have to duplicate and change names to all variables as well as the functions.
* Next version should have the possibility to pass the array as a parameter to the function.
*
* created 15 Decembre 2005;
* copyleft 2005 Progetto25zero1  <http://www.progetto25zero1.com>
*
* --------------------------------------------------- */

int  serIn;             // var that will hold the bytes-in read from the serialBuffer
char serInString[100];  // array that will hold the different bytes  100=100characters;
                        // -> you must state how long the array will be else it won't work.
int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
int  serOutIndx = 0;    // index of the outgoing serInString[] array;

  

//read a string from the serial and store it in an array
//this func uses globally set variable so it's not so reusable
//I need to find the right syntax to be able to pass to the function 2 parameters:
// the stringArray and (eventually) the index count
void readSerialString () {
    int sb;   
    if(Serial.available()) { 
       //Serial.print("reading Serial String: ");     //optional confirmation
       while (Serial.available()){ 
          sb = Serial.read();             
          serInString[serInIndx] = sb;
          serInIndx++;
       }
    
    }  
}

//print the string all in one time
//this func as well uses global variables
void printSerialString() {
  pinMode(13, OUTPUT); 
   if( serInIndx > 0) {
      Serial.print("Arduino memorized that you said: ");     
      //loop through all bytes in the array and print them out
      for(serOutIndx=0; serOutIndx < serInIndx; serOutIndx++) {
          Serial.print( serInString[serOutIndx] );    //print out the byte at the specified index
      }        
      //reset all the functions to be able to fill the string back with content
      if (strcmp(serInString, "test\n") == 0){
         digitalWrite(13, HIGH);
      }
      delay(1000);
     digitalWrite(13, LOW);

      serOutIndx = 0;
      serInIndx  = 0;
   }
}


void setup() {
  Serial.begin(9600);
}

void loop () {
  //simple feedback from Arduino
  
  //read the serial port and create a string out of what you read
  //readSerialString(serInString, serInIndx);
  readSerialString();
  
  //try to print out collected information. it will do it only if there actually is some info.
  printSerialString();
  
  //slows down the visualization in the terminal
  delay(2000);
}
