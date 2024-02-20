# Approximation der Kennlinie eine NTC 

Dieses Programmbeispiel die Appoximation einer nichtlienaren Sensor-Kennlinie 
am Beispiel eines NTC.

Im Programm Arduino_NTC_Resistor werden ausgehend von einer Spannungsmessung an einem Spannungsteiler mit NTC Widerstand und Temperatur mit Hilfe der Steinhart-Hart-Gleichung berechnet.

Das Programm RP2040_Spline berechnet eine Spline-Approximation der nichtliearen Kennlinie und stimmt dann über diesen Weg die jeweilige Temperatur sowie die Laufzeit für einen solchen Zugriff.

