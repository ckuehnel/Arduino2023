# Approximation einer nichtlinearen Kennlinie
am Beispiel eines NTC-Widerstandes

Die NTC-Kennlinie wird durch die Steinhart-Hart-Gleichung beschrieben.
Siehe hierzu bspw. https://thermistor.sourceforge.net/.

Das Programm Arduino_NTC_Thermistor berechnet aus Werte des Spannungsteiles mit einem NTC dessen Widerstandswert und seine Temperatur.
Die Cubic Spline Approximation liefert eine sehr genaue Anpassung an die nichtlineare Kennlinie. Ich habe das auf einem RP2040 getestet.

Ausserdem werden eine Polynom-Approximation 4. Grades sowie ein Neuronales Netz für die Anpassung der Kennlinie verwendet. 
Das neuronale Netz arbeitet mit 9 Hidden Neurons, sowie einem Eingangs- und eines Ausgangs-Neuron. Die Gewichte werden mit dem Programm im Folder Neuronales Netz auf dem PC berechnet. 

Zum finalen Vergleichstest habe ich das Programm NTC_Runtime auf einem RP2040 verwendet. Es liefert die Daten, die dann vergleichend in einem Excel-Sheet bearbeitet diese folgenden Grafiken liefern. 

![image](https://github.com/ckuehnel/Arduino2023/assets/195788/c9e66b76-3b46-418f-b53b-b66351600ac9)

![image](https://github.com/ckuehnel/Arduino2023/assets/195788/d0f8a777-4173-493d-a95b-544169fce431)
