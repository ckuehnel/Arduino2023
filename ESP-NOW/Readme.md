# ESP-NOW Low-Power Sensornetzwerk

Die zwei Verzeichnisse enthalten ESP-Now Anwendungen mit unterschiedlichen Gateways.

## TP-GWWWiFi    <img src="https://github.com/ckuehnel/Arduino2023/blob/main/ESP-NOW/TP-GEWiFi/TP-GWWiFi.jpg"> 
Das im Blogbeitrag https://ckarduino.wordpress.com/2021/10/09/low-power-sensornetzwerk/ beschriebene Sensornetzwerk verwendet batterie- bzw. solarbetriebene Sensorknoten, 
die aus *DFRobot FireBeetle ESP32-E* oder *ThingPulse ePulse Low Power ESP32* Boards bestehen. Die erhobenen Messwerte werden über ESP-NOW an ein *ThingPulse ESP-Gateway* gesendet.
Hierbei kommt das ThingPulse-Gateway mit zwei WiFi-Kanälen zum Einsatz.

Das *ThingPulse ESP-Gateway* besteht aus zwei ESP32-Wrover-IB-Modulen. Detaillierte Informationen zum *ThingPulse ESP-Gateway* sind unter der URL https://thingpulse.com/new-product-the-espgateway-design/ zu finden.

Vom ersten ESP32 werden die über ESP-NOW gesendeten Messages empfangen (ESP-NOW Receiver), aufbereitet und seriell an den zweiten ESP32 im Gateway transferiert. 
Dieser übernimmt diese Daten und bereitet sie zum Versenden über MQTT auf.

Die MQTT Messages sende ich an den Public MQTT Broker von HiveMQ (https://www.hivemq.com/public-mqtt-broker/). Mit einem MQTT Client auf einem PC, Tablet oder Smartphone 
können einzelne oder alle MQTT Messages abonniert (subscribed) werden.

## TP-GWE   <img src="https://github.com/ckuehnel/Arduino2023/blob/main/ESP-NOW/TP-GWE/TP-GWE.jpg">
Ein *LilyGo_EPD-Knoten* mit ePaper Display SHTC3 Outdoor Sensor wird neu aufgenommen und das *ThingPulse ESP--Ethernet-Gateway* wird eingesetzt.

Das *ThingPulse ESP-Gateway* besteht aus einem ESP32-Wrover-IE-Modul und einem Microchip LAN8720A-Ethernet-Treiber. Detaillierte Informationen zum *ThingPulse ESP-Gateway* sind unter der URL https://thingpulse.com/product/espgateway-ethernet-esp32-wifi-ble-gateway-with-rj45-ethernet-connector/ zu finden.

<img src="https://github.com/ckuehnel/Arduino2023/blob/main/ESP-NOW/TP-GWE/LilyGo_EPD_Node.jpg">LiLyGo EPD102 Board

In der unteren Zeile wird die MAC Adresse (aus Platzgründen ohne :) angezeigt. Diese wird im Gateway benötigt.
