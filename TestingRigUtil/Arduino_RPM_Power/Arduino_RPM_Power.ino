int current = LOW;
unsigned long last;
unsigned long rpm;
uint16_t analog_voltage;
uint16_t analog_amps;
int rpm_pin = 12;
int voltage_pin = 0;
int amps_pin = 1;
int i = 0;
int loops = 500;
int debounce = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  last = micros();
  pinMode(rpm_pin, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(rpm_pin);
  if(val == HIGH && current == LOW) {
    //rising edge
    current = HIGH;
    unsigned long now = micros();
    unsigned long delta = now - last;
    last = now;
    rpm = 60000000 / delta;
    debounce = 0;
  } else if(val == LOW && current == HIGH){
    //falling edge
    current = LOW;
  }
  debounce++;

  //Sample power at same rate that data is sent
  //Just sample at a different time
  if(i == loops/2){
    analog_voltage = analogRead(voltage_pin);
    analog_amps = analogRead(amps_pin);
  }

  i++;
  if(i == loops){
    Serial.print(rpm);
    Serial.println("  ");
    i = 0;
  }
}
