import time;
import serial;
import thread;

arduino = serial.Serial('/dev/ttyUSB0', 9600);
run = True;

def input_thread(L) :
	string = raw_input();
	L.append(string);

L = [];
thread.start_new_thread(input_thread, (L,));

while run:
	#arduino.write("output 550$");
	print("lei: " + arduino.readline());

	#manejo de thread para cambiar velocidad
	if L :
		arduino.write("0$");
		run = False;
		pass
