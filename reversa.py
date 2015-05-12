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
	arduino.write("re$");
	time.sleep(1);
	print(arduino.readline());
	#manejo de thread para cambiar velocidad
	if L :
		arduino.write("di0$");
		run = False;
		pass
