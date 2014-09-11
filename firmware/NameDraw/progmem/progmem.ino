#include <avr/pgmspace.h>

// save some unsigned ints
//PROGMEM  const uint16_t charSet[]  = { 65000, 32796, 16843, 10, 11234};

//// save some chars
//prog_uchar signMessage[] PROGMEM  = {"I AM PREDATOR,  UNSEEN COMBATANT. CREATED BY THE UNITED STATES DEPART"};
//
//unsigned int displayInt;
int k;    // counter variable
float myFloat;
float const PROGMEM foo[3][2]={{1.0,2.0},{3.0,4.0},{5.0,6.0}};
int i,j;
char myChar;  
char const MyString[] PROGMEM = "This should be in flash as well";  

void setup() {
//  // read back a 2-byte int
//   displayInt = pgm_read_word_near(charSet + k);
//  
//  // read back a char 
  myChar =  pgm_read_byte_near(MyString + k);

  Serial.begin(9600);
  Serial.println("Online :)");
  Serial.println(sizeof(float));
  Serial.println(sizeof(MyString));
  
  for (k=0; k<31; k++) {
    myChar =  pgm_read_byte_near(MyString + k);
    Serial.print(myChar);
  }
  Serial.println();

  Serial.println(sizeof(foo));
  for (k=0; k<2; k++) {
    for (j=0; j<3; j++) {
      //myFloat = pgm_read_float_near(foo+k);
      myFloat = pgm_read_float( &foo[j][k] );
      Serial.print(myFloat);
      Serial.print(" ");
    }
  }
  Serial.println();

  
//   Serial.println(MyString);
  //for (i=0; i<3; i++) {
   // for (j=0; j<2; j++) {
     // Serial.println(foo[i][j]);
   //   foo[i][j]=0;
   // }
//  }
  Serial.println("Funciono!");
}

void loop() {
  
}
