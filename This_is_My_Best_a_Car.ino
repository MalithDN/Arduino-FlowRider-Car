    
//=========================This is very simple a car auto control code======================

#include <Servo.h>
Servo servo; //define Servo object


//ultra socinc sensor
int Echo = 4 ;  // Echo(yellow jumper)
int Trig = 2; //  Trig (meteor orange jumper)


//moter drive moddule
int Left_motor_go = 6;   //(gray jumper)
int Left_motor_back = 9;   //(red jumper)

int Right_motor_go = 10;  // (black jumper)
int Right_motor_back = 11;  // (brown jumper)


//servo motor
int servo_pin = 5; //(chromite orange jumper)


//make the Distance varible
int Front_Distance = 0;
int Left_Distance = 0;
int Right_Distance = 0;

//======================================void setup function===============================

void setup() {
  // put your setup code here, to run once:

  servo.attach(servo_pin);
  servo.write(90);
  delay(300);
  Serial.begin(9600); //begin serial communication
  //IO
  pinMode(Left_motor_go, OUTPUT); // PIN 6 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 10 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 11 (PWM)
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);

}
//======================================= END setup function ========================================

//===============Motors control functions==============
void run() {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  //analogWrite(Right_motor_go, 165); //PWM0~255
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  //analogWrite(Left_motor_go, 165); //PWM0~255
  analogWrite(Left_motor_back, 0);
}

void brake(int time) {
  digitalWrite(Right_motor_go, LOW);
  analogWrite(Right_motor_go, 0);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  analogWrite(Left_motor_go, 0);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_back, 0);
  delay(time * 15);
}

void left(int time) {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  //analogWrite(Right_motor_go, 200);//PWM0~255
  //analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  //analogWrite(Left_motor_go, 0);
  //analogWrite(Left_motor_back, 0);
  delay(time * 500);
}

void spin_left(int time) {
  //digitalWrite(Right_motor_go, HIGH);
  //digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 200);//PWM0~255
  analogWrite(Right_motor_back, 0);
  //digitalWrite(Left_motor_go, LOW);
  //digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);//PWM0~255
  analogWrite(Left_motor_back, 200);
  delay(time * 500);
}

void right(int time) {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  //analogWrite(Right_motor_go, 0);
  //analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  //analogWrite(Left_motor_go, 200);//PWM0~255
  //analogWrite(Left_motor_back, 0);
  delay(time * 500);  //
}

void spin_right(int time) {
  //digitalWrite(Right_motor_go, LOW);
  //digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 200); //PWM0~255
  //digitalWrite(Left_motor_go, HIGH);
  //digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 200);
  analogWrite(Left_motor_back, 0); //PWM0~255
  delay(time * 500);
}

void back(int time) {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  //analogWrite(Right_motor_back, 150); //PWM0~255
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  //analogWrite(Left_motor_back, 150); //PWM0~255
  delay(time * 50);
}
//==========================END motor controal functions================================

//=================ultra sound Sensors program functions==============
float Distance_test() {
  digitalWrite(Trig, LOW);     // 2s to 2s
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);     // 10s to 10s
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = (Fdistance / 29) / 2;    //inches=(Fdistance/74)/2
  return Fdistance;                   //give distance valiue to main program
  //Serial.print("Distance(cm):");
  //Serial.println(Fdistance);
}

void front_detection() {
  servo.write (90); //PWM
  delay(1000);
  Front_Distance = Distance_test();
  //Serial.print("Front_Distance(cm):- ");
  // Serial.println(Front_Distance);
}

void left_detection() {
  servo.write (180); //PWM
  delay(1000);
  Left_Distance = Distance_test();
  servo.write(90);  // servo motor control
  delay(1000);
  //Serial.print("Left_Distance(cm):- ");
  //Serial.println(Left_Distance);
}

void right_detection() {
  servo.write (0); //PWM
  delay(1000);
  Right_Distance = Distance_test();
  servo.write(90);        //servo motor control
  delay(1000);
  //Serial.print("Right_Distance(cm):- ");
  //Serial.println(Right_Distance);
}

//==========================================viod loop function=======================================================

void loop() {
  // put your main code here, to run repeatedly:
  front_detection();
  if (Front_Distance <= 25) {         //Detection is small 25cm?
    back(2);                         //turn back word
    brake(1);                       //stop
    left_detection();              //take left decection
    right_detection();            //take right decection
    if (Left_Distance > Right_Distance) {
      spin_left(1);
      //left(1);
      brake(1);
    }
    else if (Left_Distance < Right_Distance) {
      spin_right(1);
      //right(1);
      brake(1);
    }
    else {
      back(2);
      brake(1);
    }
  }
  else {
    run();
  }
}
