# Arduino_Nano_ESP32_I2CScan.py
# ScanS I2C-Bus for connected devices
# I2C: SDA = GPIO11,  SCL = GPIO12
# 2023-12-10 Claus Kuehnel info@ckuehnel.ch

from machine import Pin, I2C

i2c = I2C(0, scl=Pin(12), sda=Pin(11), freq=400000)

def printResult():
  if len(devices) == 0:
    print("No I2C device found!")
  else:
    print('{:2d} I2C device(s) found.'.format(len(devices)))
    for device in devices:  
      print('Device address: {0:3d} dec and {1:2s} hex'.format(device, hex(device)))
    
print('\nScan I2C-Bus...')
devices = i2c.scan()
printResult()

