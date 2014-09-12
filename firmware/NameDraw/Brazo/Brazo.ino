#include <Servo.h>
#include "Letras.h"

double l1=50; // First arm length (mm)
double l2=75; // Second arm length (mm)
double x=0,y=0,oldx,oldy;
double theta1,theta2;
double bl;
double minstep=0.1; //mm

//Variables for "handwriten" letters
//typedef struct{
//  float x;
//  float y;
//  char v;
//} coordenada;
//First parameter, number of letter
//Second parameter, list of points
//coordenada alfabeto[6][3];


// Recognized commands:
// * d T : delay T millis
//     T must be an integer
// * m X.X Y.Y V : move to (X.X,Y.Y)
//     X.X and Y.Y must be in decimal format,
//     even if they are integers
//     V must be an integer
//       V=0 Slow movement, interpolated, writing
//       V=1 Quik movement, not interpolated, moving from one letter to another
// * g : Return position of the arm

Servo servo1, servo2;

int i,j,k;
float myFloat;
int myInt;

void setup() {
  
  // Initialize serial
  Serial.begin(9600);
 
  // Attach servos
  servo1.attach(9);
  servo2.attach(10);
  
  // Say hello
  Serial.println("Online :)");
  
//  foo++;
//  Serial.println(foo);
  //alfabeto[0][0].y = 5.0;
  //initletra();
  //Serial.println(alfabeto[0][0].y);
  Serial.println(sizeof(int));
  Serial.print("El diccionario ocupa: ");
  Serial.println(sizeof(alfabeto));
//  for (i=0; i<54; i++) {
//    for (j=0; j<25; j++) {
//      for (k=0; k<3; k++) {
//        //myFloat = pgm_read_float_near(foo+k);
//        myFloat = pgm_read_float( &alfabeto[i][j][k] );
//        Serial.print(myFloat);
//        //myInt = pgm_read_word( &alfabeto[i][j][k] );
//        //Serial.print(myInt);
//        Serial.print(" ");
//      }Serial.println();
//    }Serial.println();
//  }
//  Serial.println();
}

void loop() {
  Serial.println("Empiezo!!!");  
  

  //Serial communication variables
  char input[40],buf,*chop,l;
  double x1,x2,nx2,y1,y2,ny2,v;
  int time, p;
  //Interpolation variables
  int i,segments;
  
  memset(input,0,40); // Set buffer to 0
  
  //Wait for data
  while (!Serial.available());
  
  for (i=0 ;Serial.available()>0 && i<39 ; i++) {
    // Read the serial buffer, or at least the first 39 chars
    // (input[39] must be NULL)
    delay(5); // Waits for the byte to be read by the ftdi
    input[i] += Serial.read();  // Gets one byte from serial buffer
  }
  
  switch (input[0]) {
    case 'g': // Read current position
      Serial.print("Position: (");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.println(")");
      break;
      
    case 'l': // Write letter
      strtok(input," ");
      chop=strtok(NULL," ");
      l = chop[0];
      Serial.print("Writting leter: ");
      Serial.println(l);
      printletter(l);
      break;
    
    case 'd': // Delay
      strtok(input," ");
      chop=strtok(NULL," ");
      time = atoi(chop);
      Serial.print("Waiting ");
      Serial.println(time);
      delay (time);
      break;
      
    case 'p': // Position the arm
    
      strtok(input," ");
      chop=strtok(NULL,".");
      x1 = atoi(chop);
      
      chop=strtok(NULL," ");
      x2 = atoi(chop);
      nx2=strlen(chop);
      
      chop=strtok(NULL,".");
      y1 = atoi(chop);
      
      chop=strtok(NULL," ");
      y2 = atoi(chop);
      ny2=strlen(chop);
      
      chop=strtok(NULL," ");
      p = atoi(chop);
      
      x=x1+x2/pow(10,nx2); //mm
      y=y1+y2/pow(10,ny2); //mm
      
      writeline(x, y, p);
      
      delay(1000);  
      break;
      
    default:
      Serial.println("Unknown command");
      break;
  }
  
  // Clear buffer to newline
  while (Serial.available()){
    buf=Serial.read();
    if (buf=='\n') break;
  }
}

void moveTo(double x, double y){
    bl=sqrt(x*x+y*y); // (x,y) vector length
    
    Serial.print(" bl: ");Serial.print(bl,4);
    Serial.print(" x: ");Serial.print(x,4);
    Serial.print(" y: ");Serial.print(y,4);
      
    if (bl>=abs(l1-l2) && bl<=l1+l2) {
  
      // Angle between first arm and the vector
      theta1=acos((bl*bl+l1*l1-l2*l2)/(2*bl*l1))*180/PI; 
      theta1+=atan(y/x);
      
      // Angle between the first and the second arm 
      theta2=acos((bl*bl+l2*l2-l1*l1)/(2*bl*l2))*180/PI;
      
      // Set both servos
      servo1.write(theta1);
      servo1.write(theta2);
      Serial.print(" Theta: ");Serial.print(theta1,4);Serial.print(",");Serial.println(theta2,4);
    } else {
      Serial.println(" ERROR: Point unreachable");
    } 

}

// Traza una recta (v=1:dibujando o v=0:no) desde el punto actual
// al indicado como parametro 
void writeline(double x,double y, double v){
  //Calculate number of gegments
  int segments=sqrt((x-oldx)*(x-oldx)+(y-oldy)*(y-oldy))/minstep;
  //Fix number of segments for debug
  //segments=10;
  
  Serial.print("Moving to (");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(") in ");
  
  
  // Interpolate
  if (v==1) {
    
    Serial.print(segments);
    Serial.println(" segemnts.");
    for (i=1;i<=segments-1;i++){
      moveTo(oldx+(i*(x-oldx)/segments),oldy+(i*(y-oldy)/segments));
    }
  } else {
    Serial.println("1 quick segemnt.");
  }
  
  //To be sure of final position
  moveTo(x,y);
  
  
  // Send angles over serial, for debugging purposes
  Serial.print("Theta1: ");Serial.println(theta1,2);
  Serial.print("Theta2: ");Serial.println(theta2,2);
  
  oldx=x;
  oldy=y;
  
}

void printletter(char l) {
  int i;
  if ( l <= 'N' ) { //Mayusculas antes de la Ñ
    i=l-'A';
    Serial.println("Mayusculas antes de la ENHE");
  } else if (!strcmp((char*)l,"Ñ")) {//La Ñ
    i='N'-'A'+1;
    Serial.println("La ENHE");
  } else if (l <= 'Z') {//Mayusculas despues de la Ñ
    i=l-'A'+1;
    Serial.println("Mayusculas despues de la ENHE");
  } else if (l <= 'n') {//Minusculas antes de la ñ
    i=l-'a'+'Z'-'A'+1;
    Serial.println("Minusculas antes de la enhe");    
  } else if (l == 'ñ') {//La ñ
    i='n'-'a'+1+'Z'-'A'+1;
    Serial.println("La enhe");    
  } else if (l <= 'z') {//Minusculas despues de la ñ
    i=l-'a'+'Z'-'A'+2;
    Serial.println("Minusculas despues de la enhe");    
  }
  
  Serial.println(i);



////  for (i=0; i<54; i++) {
//    for (j=0; j<25; j++) {
//      for (k=0; k<3; k++) {
//        //myFloat = pgm_read_float_near(foo+k);
//        myFloat = pgm_read_float( &alfabeto[i][j][k] );
//        Serial.print(myFloat);
//        //myInt = pgm_read_word( &alfabeto[i][j][k] );
//        //Serial.print(myInt);
//        Serial.print(" ");
//      }Serial.println();
//    }Serial.println();
////  }
////  Serial.println();
  
  
}

