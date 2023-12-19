# pong-esp32-vga

## Aufgabenstellung
Es soll eine Spielekonsole basierend auf einem ESP32 entwickelt werden. Die Spielersteuerung erfolgt über zwei Lagesensoren (MPU6050). Die Ausgabe erfolgt über ein Monitor, der über VGA angesteurt wird.

## Umsetzung
### VGA Monitor Ausgabe
Hierfür wurde die Libary [bitluni ESP32lib]([https://pages.github.com/](https://github.com/bitluni/ESP32Lib)https://github.com/bitluni/ESP32Lib) genutzt.
```
#include <ESP32Lib.h>
```
Um diese zu nutzen wurden folgende Befehle genutzt:  

```
VGA3Bit vga;
vga.setFrameBufferCount(2);
Mode myMode = vga.MODE320x240;
myMode.print<HardwareSerial>(Serial);
vga.init(myMode, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

void loop(){
  vga.clear(0);
  vga.show();
}
```
