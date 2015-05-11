import time;
import serial;
import thread;

arduino = serial.Serial('/dev/ttyUSB0', 9600);
rapidezActual = 0;
tiempoVariable = time.time();
tiempoInicio = time.time();

#variables de pid
velocidadReferencia = 0.8;
errorOld = 0;
PIDvarOld = 0;
K = 2740.02;
ti = 0.976837;
Ts = 0.033;

proxSensor = 1024;
velToArduino = 0;

def input_thread(L) :
	string = raw_input();
	L.append(string);

def PID(velocidad):
	global velocidadReferencia;
	global errorOld;
	global PIDvarOld;
	global K;
	global ti;
	global Ts;

	errorActual = velocidadReferencia - velocidad;
	respuesta = PIDvarOld + (K/ti)*(Ts+ti)*errorActual - (K*errorOld);

	print("ref: " + str(velocidadReferencia));
#	print("Error: " + str(errorActual));
	errorOld = errorActual;
	PIDvarOld = respuesta;

	if respuesta > 100 :
		respuesta = 100;
	elif respuesta < 0 :
		respuesta = 0;

	return respuesta;

L = [];
thread.start_new_thread(input_thread, (L,));

while True:
	#ver velocidad
	serialString = arduino.readline();
	print(serialString);
	if "::velup" in serialString:
		tiempoVariable = time.time() - tiempoInicio;
		tiempoInicio = time.time();
		#4 cm radio = 12.5664   --- 1mm/ms = 1m/s
		rapidezActual = 125.664 / (tiempoVariable*1000);
		velToArduino = PID(rapidezActual);
#		print("rapidez actual: " + str(rapidezActual));
		print("PID: " + str(velToArduino) + " \n ");
		arduino.write(str(velToArduino));
		pass

	if "::detener" in serialString:
		velToArduino = 0;
		proxSensor = serialString.split(' ')[1];

		print("Se encontro un obstaculo a:" + proxSensor);
		if proxSensor < 30:
			velToArduino = 5;
			pass
		if proxSensor < 25:
			velToArduino = 50;
			pass

		print("reversa en: " + str(velToArduino) + " \n ");
		pass

	#manejo de thread para cambiar velocidad
	if L :
		try :
			velocidadReferencia = float(L[0]);
			print("La nueva velocidad de referencia sera: " + str(L[0]));
		except :
			print("no fue un numero valido");

		del L
		L = [];
		thread.start_new_thread(input_thread, (L,));
		pass
