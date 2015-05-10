import time;
import serial;

arduino = serial.Serial('/dev/ttyUSB0', 9600);
rapidezActual = 0;
tiempoVariable = time.time();
tiempoInicio = time.time();

#variables de pid
velocidadReferencia = 0.8;
errorOld = 0;
PIDvarOld = 0;
K = 1999.58;
ti = 0.976837;
Ts = 0.033;

def PID(velocidad):
	global velocidadReferencia;
	global errorOld;
	global PIDvarOld;
	global K;
	global ti;
	global Ts;

	errorActual = velocidadReferencia - velocidad;
	respuesta = PIDvarOld + (K/ti)*(Ts+ti)*errorActual - (K*errorOld);

	errorOld = errorActual;
	PIDvarOld = respuesta;

	if respuesta > 100 :
		respuesta = 100;
	elif respuesta < 0 :
		respuesta = 0;

	return respuesta;


while True:
	#ver velocidad
	serialString = arduino.readline();
	#print(serialString);
	if "::velup" in serialString:
		tiempoVariable = time.time() - tiempoInicio;
		tiempoInicio = time.time();
		#4 cm radio = 12.5664   --- 1mm/ms = 1m/s
		rapidezActual = 125.664 / tiempoVariable;
		print("rapidez actual: " + str(rapidezActual));
		pass

