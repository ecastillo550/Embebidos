#include <Arduino.h>

void setup();
void directa(int velocidad);
void reversa(int velocidad);
void detener();
double PID(float referenceVel, double velocidad);
void loop();
#line 1 "src/sketch.ino"
int directaPin = 5;
int reversaPin = 3;
int ledPin = 13;
int velsensor = 4;
int proxsensor = 3;

boolean RUN = false;
boolean REVERSE = false;
int reverseVel = 0;
int proxsensorVal = 1024;
unsigned long timeInicio = millis();
unsigned long timeTotal = 0;
double rapidezActual = 0;

//variables para pid
double velocidadPID = 0;
double errorOld = 0;
double PIDvarOld = 0;
float K = 1999.58;
float ti = 0.976837;
float Ts = 0.033;
double PIDvar = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode (reversaPin, OUTPUT);
  pinMode (directaPin, OUTPUT);
  pinMode (velsensor, INPUT);
  pinMode (proxsensor, INPUT);
  Serial.begin(9600);
}

void directa(int velocidad) {
  analogWrite(directaPin, velocidad);
  analogWrite(reversaPin, 0);
}

void reversa(int velocidad) {
  analogWrite(directaPin, 0);
  analogWrite(reversaPin, velocidad);
}

void detener() {
  analogWrite(directaPin, 0);
  analogWrite(reversaPin, 0);
}

double PID(float referenceVel, double velocidad) {
  double respuesta = 0;
  double errorActual = referenceVel - velocidad;

  respuesta = PIDvarOld + (K/ti)*(Ts+ti)*errorActual - (K*errorOld);

  //guardado de variables
  errorOld = errorActual;
  PIDvarOld = respuesta;

  if(respuesta > 100){
    respuesta = 100;
  } else if (respuesta < 0) {
    respuesta = 0;
  }

  Serial.println(errorActual);

  return respuesta;
}

void loop() {
  if(analogRead(velsensor) < 800) {
     digitalWrite(ledPin, HIGH);
     timeTotal = millis() - timeInicio;
     timeInicio = millis();
     if(timeTotal > 10) {
       //4 cm radio = 12.5664   --- 1mm/ms = 1m/s
       rapidezActual = 125.664 / timeTotal;
     }
  } else {
     digitalWrite(ledPin, LOW);
     if (millis() - timeInicio > 3800) {
       rapidezActual = 0;
     }
  }

  proxsensorVal = analogRead(proxsensor);
  if (proxsensorVal > 50) {
    //velocidadPID = PID(0.5, rapidezActual);
    velocidadPID = map(PID(0.7, rapidezActual), 0, 100, 0, 255);
    directa(velocidadPID);
  } else if(proxsensorVal < 50 && proxsensorVal > 18) {
    detener();
  } else if (analogRead(proxsensor) < 18){
    reverseVel = map(proxsensorVal, 12, 18, 0, 255);
    reversa(reverseVel);
  }


}
