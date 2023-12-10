# ESP32_Internals.py
# This sample program shows some information 
# and uses some on-board resources to illustrate their query
# (c) 2019-01-29 Claus Kuehnel (info@ckuehnel.ch
	
import machine, time, sys, uos, ubinascii, esp, esp32
from machine import Timer
from machine import Pin


led = Pin(48, Pin.OUT)   # yellow on-board LED on Arduino Nano ESP32

def blink():
	led.on()
	time.sleep_ms(20)      # LED on for 20 milliseconds
	led.off()

 
print('\nThis program will show some information')
print('and the usage of some on-board ressources.\n')
print('The yellow on-board led blinks ones per second\n')

print('This is a {}'.format(uos.uname().machine))
print('Installed firmware version is {}\n'.format(uos.uname().version))
print('Platform is {}'.format(sys.platform))
print('Micropython version is {}'.format(sys.version))
print('Flash size is {:4.2f} MByte'.format(esp.flash_size()/1000000))
print('CPU frequency is {:3.0f} MHz'.format(machine.freq()/1000000))

UID = ubinascii.hexlify(machine.unique_id()).decode('utf-8')
print('Length of UID is {} bytes'.format(int(len(UID)/2)))
print('UID is {}\n'.format(UID))

t = Timer(-1) # virtual (RTOS-based) timer 
t.init(period=1000, mode=Timer.PERIODIC, callback=lambda f: blink())


 
 









