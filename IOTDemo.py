import requests
import json
#from pyfirmata import Arduino
firebase_url = 'https://iotdemo-75936.firebaseio.com/'
import serial
import time
import threading
#pp = Arduino('/dev/ttyACM0')

ser = serial.Serial('/dev/ttyACM0',9600)
time.sleep(2)
print('start')

class MyReadArduino (threading.Thread):
    def __init__(self): #override
        threading.Thread.__init__(self)
    
                
    def run(self): #override
        while True:
            if(ser.inWaiting()>0):
                response = ser.readline()
                if(response.startswith('open')):
                    requests.patch(firebase_url+'.json',data=json.dumps({'switch':True}))
                elif(response.startswith('close')):
                    requests.patch(firebase_url+'.json',data=json.dumps({'switch':False}))
                elif(response.startswith('legitimate')):
                    requests.patch(firebase_url+'/rfid'+'.json',data=json.dumps({'data':'Tom'}))
                    requests.patch(firebase_url+'/rfid'+'.json',data=json.dumps({'change':True}))
                elif(response.startswith('illegitimate')):
                    requests.patch(firebase_url+'/rfid'+'.json',data=json.dumps({'data':'illegitimate'}))
                    requests.patch(firebase_url+'/rfid'+'.json',data=json.dumps({'change':True}))

                elif(response.startswith('temp')):
                    requests.patch(firebase_url+'.json',data=json.dumps({'temperature':response[4:9]}))

MyReadArduino().start()

while True:
    Data = requests.get(firebase_url+'.json')
    if Data.json()['lcd']['change']:
        s = Data.json()['lcd']['data']
        ser.write(s)
        requests.patch(firebase_url+'/lcd'+'.json',data=json.dumps({'change':False}))
        time.sleep(1)
    
    if Data.json()['green']['change']:
        if Data.json()['green']['switch']:
            print('greenon')
            ser.write('greenon')
        else:
            print('greenoff')
            ser.write('greenoff')
        requests.patch(firebase_url+'/green'+'.json',data=json.dumps({'change':False}))
        time.sleep(1)
    
    if Data.json()['yellow']['change']:
        if Data.json()['yellow']['switch']:
            print('yellowon')
            ser.write('yellowon')
        else:
            print('yellowoff')
            ser.write('yellowoff')
        requests.patch(firebase_url+'/yellow'+'.json',data=json.dumps({'change':False}))
        time.sleep(1)
    
    if Data.json()['red']['change']:
        if Data.json()['red']['switch']:
            print('redon')
            ser.write('redon')
        else:
            print('redoff')
            ser.write('redoff')
        requests.patch(firebase_url+'/red'+'.json',data=json.dumps({'change':False}))
        time.sleep(1)


    
        

