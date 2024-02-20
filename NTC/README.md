# Approximation einer nichtlinearen Kennlinie
am Beispiel eines NTC-Widerstandes

Die NTC-Kennlinie wird durch die Steinhart-Hart-Gleichung beschrieben.
Siehe hierzu bspw. https://thermistor.sourceforge.net/.

Das Programm Arduino_NTC_Thermistor berechnet aus Werte des Spannungsteiles mit einem NTC dessen Widerstandswert und seine Temperatur.
Die Cubic Spline Approximation liefert eine sehr genaue Anpassung an die nichtlineare Kennlinie. Ich habe das auf einem RP2040 getestet.

Außerdem werden eine Polynom-Approximation 4. Grades sowie ein Neuronales Netz für die Anpassung der Kennlinie verwendet. 
Das neuronale Netz arbeitet mit 9 Hidden Neurons, sowie einem Eingangs- und eines Ausgangs-Neuron. Die Gewichte werden mit dem Programm im Folder Neuronales Netz auf dem PC berechnet. 

Zum finalen Vergleichstest habe ich das Programm NTC_Runtime auf einem RP2040 verwendet. Es liefert die Daten, die dann vergleichend in einem Excel-Sheet bearbeitet diese folgenden Grafiken liefern. 

Es zeigt sich, dass selbst die vereinfachte Steinhart-Hart-Gleichung die reale Kennlinie im Bereich mittlerer Temperaturen sehr gut wiedergibt. An den Bereichsenden nimmt der Fehler dann zu. 

Die Polynom-Approximation 4. Grades ist für genauere Messungen nur eingeschränkt verwendbar, genauso wie das neuronale Netz. Um mit diesen Methoden die Genauigkeit zu erhöhen, bedarf es weiteren Aufwands (Polynom höheren Grades, aufwendigeres Netzwerk). Die Cubic-Spline-Approximation liefert bereits sehr brauchbare Ergebnisse und wäre (aus meiner Sicht) das Mittel der Wahl. 

Bei den Zugriffszeiten verhält es sich ähnlich. Auf einem XIA0 RP2040 (Cortex-M0+ @ 125 MHz) belaufen sich diese für die Spline-Approximation auf ca. 20 us, für die Polynom-Approximation 4. Grades auf ca. 30 us und für das neuronale Netzwerk auf knapp 100 us. 

![image](https://github.com/ckuehnel/Arduino2023/assets/195788/c9e66b76-3b46-418f-b53b-b66351600ac9)

![image](https://github.com/ckuehnel/Arduino2023/assets/195788/1b8a7324-7af9-47d9-941a-5ca295094d7e)

2024-02-10/CK
