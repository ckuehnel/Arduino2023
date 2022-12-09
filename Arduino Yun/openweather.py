#!/usr/bin/env python3
# Reading weather data from openweathermap network
# created Claus Kühnel 2022-11-11 info@ckuehnel.ch

import urllib.request
import json, time

f = urllib.request.urlopen("http://api.openweathermap.org/data/2.5/weather?q=Altendorf,CH&APPID=68f41be621dc61000324339afa4873d1")
json_string = f.read().decode('utf-8')
parsed_json = json.loads(json_string)

#location = parsed_json['location']['city']
location = parsed_json['name']
temp_c  = parsed_json['main']['temp'] - 273.15
temp  = round(10*temp_c)/10
rel_hum = parsed_json['main']['humidity']
weather = parsed_json['weather'][0]['description']
station = parsed_json['id']
updated = parsed_json['dt']
print("Current temperature in {0:s} is {1:.1f} *C".format(location, temp))
print("Current relative humidity is {:d} %".format(rel_hum))
print("Weather is %s " % (weather))
print("Weather station is %s" % (station))
print("Last updated: ")
local_time = time.localtime(updated)
print(time.strftime("%Y-%m-%d %H:%M:%S", local_time)) 
f.close()

f = open("/root/TEMP","w")
f.write("{:.1f}".format(temp) + '\r\n')
f.close()

f = open("/root/HUMI","w")
f.write(str(rel_hum) + '\r\n')
f.close() 
