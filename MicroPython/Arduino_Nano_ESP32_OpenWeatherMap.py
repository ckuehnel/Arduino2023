# Sensorless Weather Station
# https://microcontrollerslab.com/micropython-openweathermap-api-esp32-esp8266-sensorless-weather-station/
# 2023-12-10 Claus Kuehnel info@ckuehnel.ch

import time

try:
  import urequests as requests
except:
  import requests
  
try:
  import ujson as json
except:
  import json

import network

import esp
esp.osdebug(None)

import gc
gc.collect()

ssid = 'Sunrise_2.4GHz_8AC2A0'
password = 'u2u7fgzv31Ds'

city = 'Altendorf SZ'
country_code = 'CH'

open_weather_map_api_key = '68f41be621dc61000324339afa4873d1'

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('\nOpenWeatherMap.org -  Sensorless Weather Station')
print('------------------------------------------------')
print('Connection successful')
print(station.ifconfig())

#set your unique OpenWeatherMap.org URL
open_weather_map_url = 'http://api.openweathermap.org/data/2.5/weather?q=' + city + ',' + country_code + '&units=metric' + '&APPID=' + open_weather_map_api_key

weather_data = requests.get(open_weather_map_url)
print
print(weather_data.json())

# Location (City and Country code)
location = '\nLocation: ' + weather_data.json().get('name') + ' - ' + weather_data.json().get('sys').get('country')
print(location)

# Weather Description
description = 'Description: ' + weather_data.json().get('weather')[0].get('main')
print(description)

# Temperature
raw_temperature = weather_data.json().get('main').get('temp')

# Temperature in Celsius
temperature = 'Temperature:\t' + str(raw_temperature) + ' *C'
#uncomment for temperature in Fahrenheit
#temperature = 'Temperature: ' + str(raw_temperature*(9/5.0)+32) + '*F'
print(temperature)

# Pressure
pressure = 'Pressure:\t' + str(weather_data.json().get('main').get('pressure')) + ' hPa'
print(pressure)

# Humidity
humidity = 'Humidity:\t' + str(weather_data.json().get('main').get('humidity')) + ' %rH'
print(humidity)

# Wind
wind = 'Wind:\t\t' + str(weather_data.json().get('wind').get('speed')) + ' m/s; ' + str(weather_data.json().get('wind').get('deg')) + ' grd'
print(wind)