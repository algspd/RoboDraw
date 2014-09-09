#include <Servo.h>

double l1=0.8; // First arm length (mm)
double l2=0.7; // Second arm length (mm)

// Recognized commands:
// * d T : delay T millis
//     T must be an integer
// * m X.X Y.Y : move to (X.X,Y.Y)
//     X.X and Y.Y must be in decimal format,
//     even if they are integers

Servo servo1, servo2;

void setup() {
  // Initialize serial
  Serial.begin(9600);
 
  // Attach servos
  servo1.attach(9);
  servo2.attach(10);
  
  // Say hello
  Serial.println("Online :)");
}

void loop() { 
  double x,y;
  char input[40],buf,*chop;
  int time,i;
  double x1,x2,nx2,y1,y2,ny2;
  
  memset(input,0,40); // Set buffer to 0
  while (!Serial.available());
  
  for (i=0 ;Serial.available()>0 && i<39 ; i++) {
    // Read the serial buffer, or at least the first 39 chars
    // (input[39] must be NULL)
    delay(5); // Waits for the byte to be read by the ftdi
    input[i] += Serial.read();  // Gets one byte from serial buffer
  }
  
  switch (input[0]) {
    case 'd': // Delay
      
      strtok(input," ");
      chop=strtok(NULL," ");
      time = atoi(chop);
      Serial.print("Waiting ");
      Serial.println(time);
      delay (time);
      break;
      
    case 'p': // Position  
    default:
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
      
      x=x1+x2/pow(10,nx2); //mm
      y=y1+y2/pow(10,ny2); //mm
      
      Serial.print("Moving to (");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.println(")");

      double bl=sqrt(x*x+y*y); // (x,y) vector length
  
      // Angle between first arm and the vector
      double theta1=acos((bl*bl+l1*l1-l2*l2)/(2*bl*l1))*180/PI; 
      double t=atan(y/x);
      theta1=theta1+t;
      
      // Angle que forma el segundo brazo con el primero
      double theta2=acos((bl*bl+l2*l2-l1*l1)/(2*bl*l2))*180/PI;
      servo1.write(theta1);
      servo1.write(theta2);
      
      Serial.print("Theta1: ");Serial.println(theta1,2);
      Serial.print("Theta2: ");Serial.println(theta2,2);
  
      delay(1000);
      break;
  }
  
  // Clear buffer to newline
  while (Serial.available()){
    buf=Serial.read();
    if (buf=='\n') break;
  }

} 
