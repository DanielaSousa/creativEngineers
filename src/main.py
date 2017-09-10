import socket
import requests
import serial
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 5005

ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(3)


sock = socket.socket(socket.AF_INET, # Internet
socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while 1:
	#receive messages
	data, addr = sock.recvfrom(512) # buffer size is 1024 bytes
	print "received message:", data
	
	if('http' not in data):
		##arduino
		ser.write(data)
		pass
	
	else:
		while(1):
			r = requests.get(data);
			if ('NUMREG.VA' in data):
				s = r.text
				final = s.split("[11] = ")[1].split(" ")[0]
				number = int(final)
				if(number == 10):
					#ser.write("r")
					print "r\n"
				elif(number == 20):
					#ser.write("n")
					print "n\n"
					break
				
				time.sleep(1)
			else:
				break







