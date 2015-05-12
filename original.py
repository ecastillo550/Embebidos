import time;
import serial;
import thread;
import sys;

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1.5);
rapidezActual = 0;
tiempoVariable = time.time();
tiempoInicio = time.time();

#variables de pid
if len(sys.argv) > 1 :
	print(sys.argv[1]);
	velocidadReferencia = float(sys.argv[1]);
else :
	velocidadReferencia = 0.8;

errorOld = 0;
PIDvarOld = 0;
K = 2740.02;
ti = 1.1386;
Ts = 0.033;
#errorActual = 0;

proxSensor = 1024;
velToArduino = 0;
first = True;
rapidezAnterior = 0;

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
#	global errorActual;

	errorActual = velocidadReferencia - velocidad;
	respuesta = PIDvarOld + (K/ti)*(Ts+ti)*errorActual - (K*errorOld);
	print("Error: " + str(errorActual));
	print("velocidad: " + str(velocidad));

	errorOld = errorActual;
	PIDvarOld = respuesta;

	if respuesta > 100 :
		respuesta = 100;
	elif respuesta < -100:
		respuesta = -10;

	return respuesta;

L = [];
thread.start_new_thread(input_thread, (L,));	

arduino.write("di50$");

while True:
	#ver velocidad
	serialString = '';
	serialString = arduino.readline();
	if serialString == '':
		rapidezActual = 0;
	
	if time.time() - tiempoInicio > 0.3 and rapidezAnterior == rapidezActual : 
		rapidezActual -= 0.05;
		#PIDvarOld = 0;
		#errorOld = 0;
		
	#print(serialString);
	if "::prox" in serialString:
		proxSensor = int(serialString.split(' ')[1]);
#		print(int(proxSensor));
	
	if "::velup" in serialString and first == False or rapidezActual == 0 or first:
		if "::velup" in serialString :
			tiempoVariable = time.time() - tiempoInicio;
			#tiempoInicio = time.time();
			#4 cm radio = 12.5664   --- 1mm/ms = 1m/s
			#print("PID: " + str(velToArduino) + " \n ");
			#print("rapidez actual: " + str(rapidezActual));		
			#print("referencia: " + str(velocidadReferencia));
			#print("Error: " + str(errorActual));
			print("tiempo var: " + str(tiempoVariable));
			rapidezAnterior = rapidezActual;
			rapidezActual = 125.664 / (tiempoVariable*1000);
			if rapidezActual > 5 :
				rapidezActual = rapidezAnterior;
			else :
				tiempoInicio = time.time();
	
		if proxSensor > 50:
			time.sleep(0.00001); 
			velToArduino = PID(rapidezActual);
			if velToArduino > 0:
				arduino.write("di" + str(velToArduino)+"$");
			elif velToArduino == 0:
				arduino.write("st$");
				rapidezActual = 0;
			else :
				arduino.write("re"+str(velToArduino)+"$");
				rapidezActual -= 0.05;
				print("bajo rapidez");
			print("PID: " + str(velToArduino));

	else :
		first = False;

	if proxSensor < 20:
		arduino.write("st$");
		#print("detener");
	
		#velToArduino = 0;
		#proxSensor = serialString.split(' ')[1];

		print("Se encontro un obstaculo a:" + str(proxSensor));
		#if proxSensor < 30:
		#	velToArduino = 5;
		#	pass
		#if proxSensor < 25:
		#	velToArduino = 50;
		#	pass

		#print("reversa en: " + str(velToArduino) + " \n ");
#	rapidezActual -= 0.01;
#	print("\n");

	#rapidexAnterior = rapidezActual;
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
