#include "IRremote.h"
#include <Servo.h>

IRrecv irrecv(2); // указываем вывод, к которому подключен приемник
int servoPin = 9;
int echoPin = 10; 
int trigPin = 11; 

decode_results results;

Servo servo;
int angle = 0; // угол сервы в градусах
int angle_OK = 0;
int d_ang = 0; // шаг изменения угла сервы
int onr=0;
int average = 0;
int tr_max = 0, tr_min = 1000000;

int duration, cm, cm_i; // sonar
int n_track = 0; // количество измерений трэкинга
int i=0;


void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
  
  irrecv.enableIRIn(); // запускаем прием с пульта
  
  servo.attach(servoPin); // подключаем серво
  
  pinMode(trigPin, OUTPUT);  // подключаем контакты к сонару
  pinMode(echoPin, INPUT);
}

void loop() {
      d_ang=numbers(d_ang);
    if ( irrecv.decode( &results )) { // если данные пришли с пульта
      switch ( results.value ) {
      case 0xFF5AA5: // клавиша Right
      angle=angle-d_ang;
      if (angle < 0) { angle = 0; }
      servo.write(angle);
      Serial.println("-------------TURN---------------");
      Serial.print(  "angle = " );
      Serial.print(  angle );
      Serial.println( " gr" );  
      Serial.println("--------------------------------");
      break;
      case 0xFF10EF: // клавиша Left
      angle=angle+d_ang;
      if (angle > 180) { angle = 180; }
      servo.write(angle);
      Serial.println("-------------TURN---------------");
      Serial.print(  "angle = " );
      Serial.print(  angle );
      Serial.println( " gr" );
      Serial.println("--------------------------------");
      break;
      case 0xFF6897: // клавиша *
      d_ang=0;
      Serial.println("0");
      break;
      case 0xFF38C7: // клавиша OK
      Serial.println("-----------DISTANCE-------------");
      cm=servo_measured(0);
      angle_OK=angle;
      Serial.println("--------------------------------");
      break;
      case 0xFFB04F: // клавиша #
      scaner();
      break;
      case 0xFF18E7 :// клавиша UP
      Serial.println("------------TRACKING------------");
      n_track = d_ang *10;
      Serial.print(  "ntrack = " );
      Serial.println(  n_track );
      Serial.print(  "angle = " );
      Serial.print(  angle );
      Serial.println( " gr" );
      Serial.println( "" );
      for (i = 1; i < n_track + 1; i++) { 
        cm=servo_measured(1); 
        Serial.println( "" );
        average=average+cm;
        if (cm>tr_max) {tr_max=cm;}
        if (cm<tr_min) {tr_min=cm;}
        } 
        average = average / n_track;
        Serial.print( "average = " );
        Serial.print(average); 
        Serial.println(" cm");
        
        Serial.print( "max = " );
        Serial.print(tr_max); 
        Serial.println(" cm");
        
        Serial.print( "min = " );
        Serial.print(tr_min); 
        Serial.println(" cm");
        
      Serial.println("--------------------------------");
      tr_max = 0;
      tr_min = 1000000;
      average=0;
      break;
      case 0xFF4AB5 :// клавиша DOWN
      int cm2=0;
      angle_OK=fabs(angle_OK-angle);
      Serial.println("--------------ONR---------------");
      cm2=servo_measured(0);
      onr=sqrt(cm*cm+cm2*cm2-2*cm*cm2*cos(angle_OK*3.14159265/180));
      Serial.println( "" );
      Serial.print("ONR = ");
      Serial.print( onr );
      Serial.println( " cm" );
      Serial.print(" n_ang = ");
      Serial.print( angle_OK );
      Serial.println( " gr" );
      Serial.println("--------------------------------");
      break;
    } 
  irrecv.resume(); // принимаем следующую команду
  
  }

delay(100);
}





int numbers(int num0){
  int num=0;
  if ( irrecv.decode( &results )) { // если данные пришли с пульта
    switch ( results.value ) {
      case 0xFFA25D: // клавиша 1
      num = 1;
      break;
      case 0xFF629D: // клавиша 2
      num = 2; 
      break;
      case 0xFFE21D: // клавиша 3
      num = 3; 
      break;
      case 0xFF22DD: // клавиша 4
      num = 4; 
      break;
      case 0xFF02FD: // клавиша 5
      num = 5; 
      break;
      case 0xFFC23D: // клавиша 6
      num = 6; 
      break;
      case 0xFFE01F: // клавиша 7
      num = 7; 
      break;
      case 0xFFA857: // клавиша 8
      num = 8; 
      break;
      case 0xFF906F: // клавиша 9
      num = 9; 
      break;
      }
  }
if (num == 0) { num=num0; }
else {
  if (num0 < 10) { num = num0*10+num; }
  Serial.println(num);
  }
  
 if ( irrecv.decode( &results )) { // если данные пришли с пульта
    if (results.value == 0xFF9867) { // клавиша 0
      if (num0<10) {num=num0*10;}
      Serial.println(num);
      } 
 }  
  return num;
}



void scaner() {
  Serial.println("------------SCANER--------------");
  Serial.print( "Ot 180 gr do 0 gr kazhdye " );
  Serial.print( d_ang );
  Serial.println( " gr" );
  
  if (d_ang>0) {  
      for(angle = 180; angle > -1; angle-=d_ang) { 
        cm=servo_measured(1); 
        //Serial.println( "" );
        if (d_ang<15) {delay(d_ang*40);}
        else {
          if (d_ang<30) {delay(d_ang*20);}
          else {delay(d_ang*10);}
        }
      }
      angle=180;
      servo.write(angle);
      } 
  Serial.println("--------------------------------");
}

int servo_measured(int k ) {
  servo.write(angle);
        digitalWrite(trigPin, LOW); 
        delayMicroseconds(2); 
        digitalWrite(trigPin, HIGH); 
        delayMicroseconds(10); 
        digitalWrite(trigPin, LOW); 
        duration = pulseIn(echoPin, HIGH); 
        cm_i = duration / 58;
       if (k == 0) {
        Serial.print(  "angle = " );
        Serial.print(  angle );
        Serial.println( " gr" ); 
        }
        Serial.print( "distance = " );
        Serial.print(cm_i); 
        Serial.println(" cm"); 
        delay(20);
        return cm_i;
}
