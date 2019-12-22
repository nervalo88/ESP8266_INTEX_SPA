## ESP8266_INTEX_SPA

**!! THIS IS WORK IN PROGRESS !!**

This is a firmware for ESP8266 to expose it to a domotic system (Domoticz, jeedom...).
I'm personally using domoticz, which will be my test system. But this firmware shall be agnostic as the ESP will answser HTTP requests.
The original PCB of the SPA must be hacked : Add relays to control switches, external DS18B20 for temperature feedback, get status from original PCB outputs.

## Hacking the INTEX SPA
DISCLAIMER : of course, the warranty would void after you do this hack. 
I can not be responsible for any trouble of damage caused by those instruction.
WARNING : There is AC voltage on boards, don't do anything unless you know what you are doing !

### Switches board hack :
Add 4 wires to buttons PWR and HEATER.
TODO : Add picture

### Main board hack
Solder 3 wires to get Water pump, heater and jet pump states on R1, R2 & R3 (microcontroller side).
5V DC supply is taken directy from the board headers
TODO : Add picture

### Add your own electronics
* 1x ESP8266 Wemos D1 mini
* 1x Dual relay PCB
* 1x DS18B20 temperature sensor

All the above were attached with jumper wires and glued to the Main board cover.
TODO : Add picture


##References:
* @ripleyXLR8 reverse engineering of the main board (working principles and wiring), Thanks !
https://github.com/ripleyXLR8/intex-SSP-H-20-1-ESP32_spa_manager
* Found somebody whom reverse engineered the control board signal through 74HC595 shift registers, but unable to retrieve link...

Suitable to retrieve 7-segments display, but not to issue commands (display & buttons are muxed and some kind of scan sequence is issued by the main board after a single pin state change).

* [FR] general dicussion about attaching ESP to the intex SPA
https://www.jeedom.com/forum/viewtopic.php?t=19832
