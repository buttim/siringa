#include <CheapStepper.h>
#include <InputDebounce.h>

#define GIRI  3

CheapStepper s(8,9,10,11);
InputDebounce btn1,btn2;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A1,LOW);
  digitalWrite(A5,LOW);

  btn1.setup(A0,20,InputDebounce::PIM_INT_PULL_UP_RES); 
  btn2.setup(A3,20,InputDebounce::PIM_INT_PULL_UP_RES); 
}

enum Status {IDLE, FORWARD, BACK};

Status status=IDLE;

void loop() {
  if (status==IDLE) {
    if (btn1.process(millis())) {
      s.setRpm(6);
      s.newMove(true,2048*GIRI*2);
      status=FORWARD;
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.println("avanti");
    }
    if (btn2.process(millis())) {
      while (btn2.process(millis())) ;
      s.setRpm(15);
      s.newMove(false,2048*GIRI*2);      
      status=BACK;
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.println("indietro");
    }
  }
  else {
    s.run();
    if (status==FORWARD) {
      if (!btn1.process(millis())) {
        s.stop();
        status=IDLE;
        digitalWrite(LED_BUILTIN,LOW);
        Serial.println("stop");
      }
    }
    if (status==BACK) {
      if (s.getStepsLeft()==0 || btn1.process(millis()) || btn2.process(millis())) {
        s.stop();
        status=IDLE;
        digitalWrite(LED_BUILTIN,LOW);
        delay(250);
        Serial.println("stop");
      }
    }
  }
  /*Serial.println("avanti");
  digitalWrite(LED_BUILTIN,HIGH);
  s.setRpm(6);
  s.newMove(true,2048*GIRI*2);
  do s.run(); while (s.getStepsLeft()!=0);
  s.stop();
  delay(100);

  Serial.println("indietro");
  digitalWrite(LED_BUILTIN,LOW);
  s.setRpm(15);
  s.newMove(false,2048*GIRI*2);
  do s.run(); while (s.getStepsLeft()!=0);
  s.stop();
  delay(100);*/
}
