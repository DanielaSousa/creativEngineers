from pygame import mixer # Load the required library
import time
mixer.init()



import socket


UDP_IP = "127.0.0.1"
UDP_PORT = 5009

def sound(data):
	sec = 1

	if(data == 's'):
		url = 'Sounds/start.mp3'
		sec = 2
	elif(data == 'v'):
		url = 'Sounds/victory.mp3'
		sec = 3
	elif(data == 'l'):
		url = 'Sounds/lose.mp3'
		sec = 4

	mixer.music.load(url)
	mixer.music.play()
	time.sleep( sec )




sock = socket.socket(socket.AF_INET, # Internet
socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while 1:
	#receive messages
	data, addr = sock.recvfrom(1) # buffer size is 1024 bytes
	print "received message:", data
	sound(data)



