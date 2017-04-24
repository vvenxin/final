//countdown timer library by Andrew Mascolo http://playground.arduino.cc/Main/CountUpDownTimer
#include <CountUpDownTimer.h>


#include <LiquidCrystal.h>
#include <Servo.h>


//FSR
int fsrPin = 0;
int fsrReading;

//Servo
Servo myServo;
int servoPin = 12;

//Ultrasonic
int trigPin = 6;
int echoPin = 7;
long duration;
int distance;

///Piezo
int piezoPin = 11;

///LCD
LiquidCrystal lcd(9,8,5,4,3,2);
int contrast=20;

//Timer
CountUpDownTimer t(DOWN);
CountUpDownTimer br(DOWN);
 
void setup(void) {
  Serial.begin(9600);
  //Servo
  myServo.attach(servoPin);
  //Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   //Piezo
  pinMode(piezoPin, OUTPUT);
  //LCD
  analogWrite(10,contrast);
  lcd.begin(16, 2);
  lcd.print("PUT YO PHONE DOWN");
  //Timers
  t.SetTimer(0,25,0);
  br.SetTimer(0,5,0);
  t.StartTimer();
  br.StartTimer();
}

//Piezo tune, code from here https://www.arduino.cc/en/Tutorial/PlayMelody
int c = 1915;
int d = 1700; 
int e = 1519;
int f = 1432;
int g = 1275;
int a = 1136;
int b = 1014;
int C = 956;
int R = 0;
int melody[] = {g,g,a,a,e,e,g};
int beats[]  = {16,16,16,16,16,16,32}; 
int MAX_COUNT = sizeof(melody) / 2;

long tempo = 10000;
int pause = 1000;
int rest_count = 100;

int tone_ = 0;
int beat = 0;
long mduration  = 0;


void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) {
    while (elapsed_time < mduration) {

      digitalWrite(piezoPin,HIGH);
      delayMicroseconds(tone_ / 2);
      digitalWrite(piezoPin, LOW);
      delayMicroseconds(tone_ / 2);
      elapsed_time += (tone_);
    } 
  }
  else {
    for (int j = 0; j < rest_count; j++) {
      delayMicroseconds(mduration);  
    }                                
  }        
}                        
 
void loop(void) {
  //read FSR
  fsrReading = analogRead(fsrPin);
  while (fsrReading>100){
    fsrReading = analogRead(fsrPin);
    //LCD
    t.Timer();
    if (t.TimeHasChanged() ){
      lcd.setCursor(0,2);
      lcd.print(t.ShowMinutes());
      lcd.setCursor(2,2);
      lcd.print(":");
      lcd.setCursor(3,2);
      lcd.print(t.ShowSeconds());
     }
    //Ultrasonic
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.034/2;
    digitalWrite(trigPin, LOW);
    //Servo
    if (distance<10){
      delay(200);
      myServo.write(50);
      delay(200);
      myServo.write(120);
    }
    
    //LCD
    if(t.ShowMinutes()==0 && t.ShowSeconds() ==0){
      lcd.setCursor(6,2);
      lcd.print("TAKE A BREAK");
      break;
     }
  }

  //break trigger
  if(t.ShowMinutes()==0 && t.ShowSeconds() ==0){
    br.Timer();
    if (br.TimeHasChanged() ){
      lcd.setCursor(0,2);
      lcd.print(br.ShowMinutes());
      lcd.setCursor(2,2);
      lcd.print(":");
      lcd.setCursor(3,2);
      lcd.print(br.ShowSeconds());
      }

   //Piezo trigger
   } else{
    fsrReading = analogRead(fsrPin);
    for (int i=0; i<MAX_COUNT; i++) {
     tone_ = melody[i];
     beat = beats[i];
    mduration = beat * tempo;
    playTone(); 
    delayMicroseconds(pause);
    }
  
  }
  
}
  
  
  


