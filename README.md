# pong-esp32-vga

## Aufgabenstellung
Es soll eine Spielekonsole basierend auf einem ESP32 entwickelt werden. Die Spielersteuerung erfolgt über zwei Lagesensoren (MPU6050). Die Ausgabe erfolgt über ein Monitor, der über VGA angesteurt wird.

## Umsetzung
### VGA Monitor Ausgabe
Hierfür wurde die Libary [bitluni ESP32lib](https://github.com/bitluni/ESP32Lib) genutzt.
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



### Pong Code
Den zu grundeliegenden *Game Loop* und die *draw_player_paddle*-Funktion des Spiels entnehmten wir aus [diesem Projekt](https://github.com/nickbild/pico_pong).
Hieraus ergab sich die **Ball_Bewegung**-Funktion:
```
void Ball_Bewegung(){
//Links
  if (ball_x - radius < B_Schlaeger) {
    vx = vx * - 1;
    Zufall();
//Punkt
    if (ball_y > p1_y + H_Schlaeger) point_scored(2);
    else if (ball_y + radius < p1_y) point_scored(2);
    else tone(speaker_out, 440, 10);
  }
//Rechts
  if (ball_x + radius > p2_x) {
    vx = vx * - 1;
    Zufall();
//Punkt
    if (ball_y > p2_y + H_Schlaeger) point_scored(1);
    else if (ball_y + radius < p2_y) point_scored(1);
    else tone(speaker_out, 440, 10);
  }
//Unten&Oben
  if (ball_y < 12 + radius || ball_y > 230 - radius) {
    vy = vy * -1;
    Zufall();
    tone(speaker_out, 500, 10);
  }
//Bewegung
  ball_x = ball_x + vx;
  ball_y = ball_y + vy;
  vga.fillEllipse(ball_x, ball_y, radius, radius, vga.RGB(255, 255, 255));
}
```
Erweitert wurde:
- Zufall-Funktion: verändert die Ballbewegung bei einer Berührung sehr unwahrscheinlich
- tone-Funktiion: erzeugt über einen Piper abprall Geräusche des Balls
- vga.fillEllipse-Funktion: erstellt das Bild des Balls

### MPU6050 Steuerung
Genutzte Libary: 
```
#include <MPU6050_tockn.h>
```
Dubliziert und die I2C-Adresse geändert zu x069. (AD0 mit 3,3V)

### Kontroller Gehäuse
Die Idee war ein schlichtes Grundkonzept, welches unteranderem für den einhändigen Gebrauch geeigent sein sollte.

