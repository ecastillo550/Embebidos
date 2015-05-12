#include <Arduino.h>

void setup();
void directa(int velocidad);
void reversa(int velocidad);
void detener(int velocidad);
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
int incomingByte = 0;
int velocidadPID = 0;

String recieve = String();
String toPi = String();

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode (reversaPin, OUTPUT);
	pinMode (directaPin, OUTPUT);
	pinMode (velsensor, INPUT);
	pinMode (proxsensor, INPUT);
	Serial.begin(9600);
	Serial.println("herw we go");
	Serial.flush();
}

void directa(int velocidad) {
	analogWrite(directaPin, velocidad);
	analogWrite(reversaPin, 0);
}

void reversa(int velocidad) {
	analogWrite(directaPin, 0);
	analogWrite(reversaPin, velocidad);
}

void detener(int velocidad) {
	//casi lo mismo que reversa
	analogWrite(directaPin, 0);
	analogWrite(reversaPin, velocidad);
}


void loop() {
	//Serial.println("battlecruiser opperational");
	if(analogRead(velsensor) < 800) {
		digitalWrite(ledPin, HIGH);
		timeTotal = millis() - timeInicio;
		timeInicio = millis();
		if(timeTotal > 30) {
			Serial.println("::velup");
		}
	} else {
		digitalWrite(ledPin, LOW);
	}
	proxsensorVal = analogRead(proxsensor);
	toPi = "::prox ";
	toPi.concat(proxsensorVal);
	Serial.println(toPi);
	//lectura de serial
	while (Serial.available() > 0) {
		recieve = Serial.readStringUntil('$');
		//Serial.println(recieve.substring(0,2));
		//Serial.println(recieve.substring(2));
		if(recieve.substring(0,2) == "di"){
			//Serial.println(recieve);
			velocidadPID = map(recieve.substring(2).toInt(), 0, 100, 0, 255);
			directa(velocidadPID);
		} else if(recieve.substring(0,2) == "re") {
			reversa(255);
			proxsensorVal = 1024;
			Serial.println("patras");
		} else if(recieve.substring(0,2) == "st") {
			detener(0);
			Serial.println("detener");
		}
		//Serial.println(recieve);
	}
	Serial.flush();
}
