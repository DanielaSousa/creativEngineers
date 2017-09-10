import requests


#requests.get('http://localhost:8080/SensorGateway', json={'data': 'calif'}) 
ip = '192.168.0.1'


# sValue = 30
# sIndx = 10
# sRealFlag = -1
# sFc = 2

# numericResgister = 'http://' + ip+  '/karel/ComSet?'
# numReg_payload = {'sFc':sFc, 'sIndx': sIndx, 'sRealFlag': sRealFlag, 'sValue': sValue}
# r = requests.get(numericResgister, params=numReg_payload)


import serial
import time
import sys


ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(3)
while 1:
	
	var = raw_input("enter robot")
	ser.write(var)
##
## 
##



# class numericResgister(object):
# 	"""docstring for numericResgister alteration"""
# 	def __init__(self, sV, sI, sR, sF):
# 		self.sValue = sV
# 		self.sIndx = sI
# 		self.sRealFlag = -1
# 		self.sFc = sF
	

