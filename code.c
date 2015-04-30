int IN3 = 5;    // Input3 conectada al pin 5
int IN4 = 3;    // Input4 conectada al pin 4
int ledPin = 13;
int inicio = 2;        
int velsensor = 4;
int proxsensor = 3;
 
boolean RUN = false;
boolean REVERSE = false;
boolean GO = false;
float thigh = 1/200;
float tlow = 1/200;
int old_proxsensor = 1024;
int actual_proxsensor = 1024;
 
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inicio, INPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (velsensor, INPUT);
  pinMode (proxsensor, INPUT);
  Serial.begin(9600);
}
 
void loop() {    
 
  if(analogRead(velsensor) < 975) {
     digitalWrite(ledPin, HIGH);
  } else {
     digitalWrite(ledPin, LOW);
  }
if (digitalRead(inicio) == HIGH) {
    GO = true;
  } else {
    GO = false;
  }
 
    actual_proxsensor = analogRead(proxsensor);
  if(actual_proxsensor > 15) {
    RUN = true;
    REVERSE = false;
  } else {
    RUN = false;
    REVERSE = true;
    delay(500);
  }
 
  old_proxsensor = actual_proxsensor;
  Serial.println(actual_proxsensor);
  Serial.println();
  if(RUN == true && REVERSE == false && GO == true){
    Serial.println("true");
    analogWrite(IN3, 0);
    analogWrite(IN4, 255);
    delay(5);
  } else {
    Serial.println("false");
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
  if(REVERSE == true && RUN == false && GO == true){
    analogWrite(IN3, 110);
    analogWrite(IN4, 0);
    delay(5000);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
  }
}