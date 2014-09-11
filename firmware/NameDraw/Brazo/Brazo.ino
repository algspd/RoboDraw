#include <Servo.h>
#include "Letras.h"
//#include <iostream>
//#include <vector>

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
//  alfabeto[0][0].y = 5.0;
  initletra();
  Serial.println(alfabeto[0][0].y);
  Serial.print("El diccionario ocupa: ");
  Serial.println(sizeof(alfabeto));
  
}

void loop() {
  //Serial communication variables
  char input[40],buf,*chop;
  double x1,x2,nx2,y1,y2,ny2;
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
      
      //Calculate number of gegments
      segments=sqrt((x-oldx)*(x-oldx)+(y-oldy)*(y-oldy))/minstep;
      //Fix number of segments for debug
      //segments=10;
      
      Serial.print("Moving to (");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(") in ");

      
      // Interpolate
      if (p==1) {
        
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



//void initletras(coordenada alfabeto){
//
/* Letras (unidades en mm)
  Mayusculas


B
alfabeto[0][0]{0.0, 5.0, 0},
alfabeto[0][0]{0.0, 15.0, 1},
alfabeto[0][0]{5.5, 15.0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{8.0, 12.5, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{5.5, 10.0, 1},
alfabeto[0][0]{0.0, 10.0, 1},
alfabeto[0][0]{5.5, 10.0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{8.0, 7.5, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{.0, .0, 1},
alfabeto[0][0]{5.5, 5.0, 1},
alfabeto[0][0]{0.0, 5.0, 1},
*/
//}
//
//void printletra(char l) {
//  
//}

