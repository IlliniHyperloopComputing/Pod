#include <Wire.h>
#define loops 500

int current1 = LOW;
unsigned long last1;
unsigned long rpm1;
int rpm_pin1 = 12;

int current2 = LOW;
unsigned long last2;
unsigned long rpm2;
int rpm_pin2 = 8;

int i = 0;

void setup() {
  Serial.begin(115200);  
  
  last1 = micros();
  last2 = micros();
  
  pinMode(rpm_pin1, INPUT);
  pinMode(rpm_pin2, INPUT);
 
}


void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(rpm_pin1);
  if(val == HIGH && current1 == LOW) {
    //rising edge
    current1 = HIGH;
    unsigned long now = micros();
    unsigned long delta = now - last1;
    last1 = now;
    rpm1 = 60000000 / delta;
  } else if(val == LOW && current1 == HIGH){
    //falling edge
    current1 = LOW;     
  }
  
  val = digitalRead(rpm_pin2);
  if(val == HIGH && current2 == LOW) {
    //rising edge
    current2 = HIGH;
    unsigned long now = micros();
    unsigned long delta = now - last2;
    last2 = now;
    rpm2 = 60000000 / delta;
  } else if(val == LOW && current2 == HIGH){
    //falling edge
    current2 = LOW;     
  }
  

  i++;
  if(i == loops){
    /*Serial.print(rpm1);
    Serial.print(" ");
    Serial.println(rpm2);
    */
    
    Serial.println(rpm1);
    i = 0;
  }
}
