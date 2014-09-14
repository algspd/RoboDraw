#include <Servo.h>
#include "Letras.h"

double const l1=74; // First arm length (mm)
double const l2=129; // Second arm length (mm)
double x=0,y=0,oldx,oldy;
double theta1,theta2;
double bl;
double const minstep=1.0; //mm
double maxx=36;
double const kerning=1;

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

Servo servo1, servo2, boli;

int i,j,k,a1,a2;
float myFloat;
int myInt;

void setup() {
  
  // Initialize serial
  Serial.begin(115200);
 
  // Attach pen servo
  boli.attach(11);
  boli.writeMicroseconds(1900);
  // Wait for the pen to be up
  delay(300);
  servo1.attach(9);
  servo2.attach(10);
  
  // Wait for the arm to be in start position
  delay(1000);
  //boli.writeMicroseconds(1400);
  
  // Say hello
  Serial.println("Online :)");
  
}

void loop() {
  Serial.println("Esperando instrucciones: ");
  boli.writeMicroseconds(1800);
  

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
      
    case 'c': // Write letter
      Serial.println("Cuadradico!");
      while (true){
        writeline(30.1, 140.0, 1);
        delay (500);
        writeline(50.1, 140.0, 1);
        delay (500);
        writeline(50.1, 160.0, 1);
        delay (500);
        writeline(30.1, 160.0, 1);
        delay (500);
      }
      
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
      
     case 'a': // Position the arm
    
      strtok(input," ");
      chop=strtok(NULL," ");
      a1 = atoi(chop);
      
      chop=strtok(NULL," ");
      a2 = atoi(chop);
      
      servo1.write(a1);
      servo2.write(a2);
      
      delay(1000);  
      break;
      
    case 'p': // Position the arm
    
      strtok(input," ");
      chop=strtok(NULL," ");
      x = atoi(chop);
      
      chop=strtok(NULL," ");
      y = atoi(chop);
            
      chop=strtok(NULL," ");
      p = atoi(chop);
      
      writeline(x, y, p);
      
      delay(100);
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
    maxx=max(x,maxx);
    bl=sqrt(x*x+y*y); // (x,y) vector length
    
    Serial.print(" bl: ");Serial.print(bl,4);
    Serial.print(" x: ");Serial.print(x,4);
    Serial.print(" y: ");Serial.println(y,4);
      
    if (bl>=abs(l1-l2) && bl<=l1+l2) {

      double o2=acos((-(bl*bl)+(l1*l1)+(l2*l2))/(2*l1*l2));
      
      // Angle between first arm and the vector
      theta1=PI/2-atan2(y,x)-asin((l2*sin(o2))/bl);
      //theta1=acos((bl*bl+l1*l1-l2*l2)/(2*bl*l1))*180/PI;
      //theta1+=atan(y/x);
      
      // Angle between the first and the second arm 
      //theta2=acos((bl*bl+l2*l2-l1*l1)/(2*bl*l2))*180/PI;
      theta2=PI-o2;
      
      // Convert both angles to degrees
      theta1=(theta1*180/PI);
      theta2=(theta2*180/PI);
      
      if(theta1>90 || theta2>90 || theta1<-90 || theta2<-90){
        Serial.println("------------> NO HAY SOLUCION EN EL RECORRIDO DE LOS SERVOS");
      }
      else {
        // *1600/180+700 Hace la conversion del rango 0-180 a 700-2300
        servo1.writeMicroseconds((theta1+90)*1600/180+700);
        servo2.writeMicroseconds((theta2+90)*1600/180+700);
      }

      // FIXME: Calcular el tiempo de desplazamiento dinamicamente
      delay(50);
      
      Serial.print(" Theta: ");Serial.print(theta1,2);Serial.print(",");Serial.println(theta2,2);
    } else {
      Serial.println(" ERROR: Point unreachable");
    }
}

// Traza una recta (v=1:dibujando o v=0:no) desde el punto actual
// al indicado como parametro 
void writeline(double x,double y, double v){
  double newx,newy;
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
    delay(100);
    boli.writeMicroseconds(1400);
    
    Serial.print(segments);
    Serial.println(" segments.");
    for (i=1;i<=segments-1;i++){
      newx = oldx+(i*(x-oldx)/segments);
      newy = oldy+(i*(y-oldy)/segments);
      moveTo(newx,newy);
    }
  }
  else {
    // if v==0
    boli.writeMicroseconds(1800);
    delay(100);
    Serial.println("1 quick segemnt.");
  }
  
  //To be sure of final position
  moveTo(x,y);

  // Send angles over serial, for debugging purposes
  Serial.print("Theta1: ");Serial.println(theta1,2);
  Serial.print("Theta2: ");Serial.println(theta2,2);
  
  // FIXME: actualizar solo si el brazo realmente se ha movido
  oldx=x;
  oldy=y;
  
}

void printletter(char l) {
  int i;
  double x, y, v, xoffset;
  xoffset=maxx+kerning;
  
  Serial.write(l);
  Serial.println();
  if ( l <= 'N' ) { //Mayusculas antes de la Ñ
    i=l-'A';
    Serial.println("Mayusculas antes de la ENHE");
  } else if (l == 'Ñ') {//La Ñ
    i='N'-'A'+1;
    Serial.println("La ENHE");
  } else if (l <= 'Z') {//Mayusculas despues de la Ñ
    i=l-'A'+1;
    Serial.println("Mayusculas despues de la ENHE");
  } else if (l <= 'n') {//Minusculas antes de la ñ
    i=l-'a'+'Z'-'A'+2;
    Serial.println("Minusculas antes de la enhe");    
  } else if (l == 'ñ' ) {//La ñ
    i='n'-'a'+1+'Z'-'A'+2;
    Serial.println("La enhe");    
  } else if (l <= 'z') {//Minusculas despues de la ñ
    i=l-'a'+'Z'-'A'+3;
    Serial.println("Minusculas despues de la enhe");    
  }
  
  //Serial.println(i);
  Serial.println();

    x = pgm_read_float( &alfabeto[i][0][0] );
    y = pgm_read_float( &alfabeto[i][0][1] );
    writeline(x+xoffset, y+146, 0);
    
    for (j=0; j<25; j++) {
        //Read x
        x = pgm_read_float( &alfabeto[i][j][0] );
        //Read y
        y = pgm_read_float( &alfabeto[i][j][1] );
        //Read v
        v = pgm_read_float( &alfabeto[i][j][2] );
        //v=0;
      xoffset=0;
      writeline(x+xoffset, y+146, v);
    }
    Serial.println();
}

