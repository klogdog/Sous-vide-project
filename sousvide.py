#First generation working thermostat code 
# using a d1 mini pro and a songle relay triggered on pin 5 (d1)
#gikfun DS18B20 Output lead: red (VCC), black (GND), yellow (DATA)


from wifi import wifi_connect
from umqtt.simple import MQTTClient
import machine
import time
from machine import Pin
import onewire

#DS18B20temperatureProbe code
"""
#Change device pin to match the GPIO pin that you are using
#A 4.7kohm resisitor is required however according to https://wp.josh.com/2014/06/23/no-external-pull-up-needed-for-ds18b20-temp-sensor/ you can get away with not using it 
dataPin = machine.Pin(12)

# create the onewire object
DS18B20temperatureProbe = onewire.DS18B20(onewire.OneWire(dataPin))

# scan for devices on the bus
roms = DS18B20temperatureProbe.scan()
print('found devices:', roms)
DS18B20temperatureProbe.convert_temp()
time.sleep_ms(750)
for rom in roms:
  print(DS18B20temperatureProbe.read_temp(rom), end=' ')
  print()

"""

mqttServer = "mqttServer"
mqttPort = "1883"
mqttKeepAlive = 60
mqttUsername = "mqttUsername"
mqttPassword = "mqttPassword"
mqttTopic = "mqttTopic"
mqttClientID = "123456"
wifissid = "wifissid"
wifiPassword = "wifiPassword"
relayPin = Pin(5,Pin.OUT)
mqttTopic1 = "sensors/environmental/new/temperature,humidity"
mqttTopic2 = "heater"
mqttTopic3 = 'tempset'



wifi_connect(wifissid,wifiPassword)
relayFlag = 0
heaterFlag = 0
temperature = b'90'
setTemperature = b'0'



def subscribeCallBack(topic, message):
  global heaterFlag
  global temperature
  global setTemperature
  if topic == b'heater':
    if message == b'0':
      heaterFlag = 0
    if message == b'1':
      heaterFlag = 1
  if topic == b'sensors/environmental/new/temperature,humidity':
    measurement = msg.split(b'/')
    temperature = measurement[0]
  if topic == b'tempset':
    setTemperature = message






try:
 client = MQTTClient(mqttClientID,mqttServer, port=1883, user= mqttUsername, password= mqttPassword)
 client.set_callback(subscribeCallBack)
 client.connect()
 client.subscribe(mqttTopic1)
 client.subscribe(mqttTopic2)
 client.subscribe(mqttTopic3)
except:
  print("connection error")
  time.sleep(5)
  machine.reset()
while True:
  try:
    client.check_msg()
  except:
    print("unhandled exception")
    time.sleep(5)
    machine.reset()
  if heaterFlag == 1:
    if temperature < setTemperature:
      relayPin.value(1)
    else:
      relayPin.value(0)
  if heaterFlag == 0:
    relayPin.value(0)
  time.sleep(1)
