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
	if(analogRead(velsensor) < 800) {
		digitalWrite(ledPin, HIGH);
		timeTotal = millis() - timeInicio;
		timeInicio = millis();
		if(timeTotal > 20) {
			Serial.println("::velup");
		}
	} else {
		digitalWrite(ledPin, LOW);
	}

	proxsensorVal = analogRead(proxsensor);

	//lectura de serial
	while (Serial.available() > 0) {
			// read the incoming byte:
			recieve = Serial.readString();
			if(recieve.toInt() < 500){
				velocidadPID = map(recieve.toInt(), 0, 100, 0, 255);
			} else {
				reversa(255);
				proxsensorVal = 1024;
			}
			Serial.println(recieve);
	}


	if (proxsensorVal > 50) {
		directa(velocidadPID);
	} else if(proxsensorVal < 50 && proxsensorVal > 18) {
		toPi = "::detener " + proxsensorVal;
		//Serial.println(toPi);
		detener(0);
		//script para manejar velocidad y reversa
	} else if (analogRead(proxsensor) < 18){
		reverseVel = map(proxsensorVal, 12, 18, 0, 255);
		reversa(reverseVel);
		Serial.println("::reversa");
	}


}
