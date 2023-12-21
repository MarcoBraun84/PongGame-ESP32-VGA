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
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

VGA3Bit vga;
vga.setFrameBufferCount(2);
Mode myMode = vga.MODE320x240;
myMode.print<HardwareSerial>(Serial);
vga.init(myMode, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

void loop(){
  vga.clear(0);
  vga.fillRect(0, 2, vga.xres, 10, vga.RGB(255, 255, 255));
  vga.fillEllipse(ball_x, ball_y, radius, radius, vga.RGB(255, 255, 255));
  vga.show();
}
```

![Screenshot 2023-12-20 233102](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/7121787a-c713-4a21-82c4-fd71c8974b97)


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
Genutzte Libary: [MPU6050_tockn](https://github.com/Tockn/MPU6050_tockn)
Leider unterstützt diese keine Adressierung des MPU6050, wodurch mehrere MPUs nicht möglich sind. Lediglich durch dublizieren der Library und ändern der I2C-Adresse werden zwei MPUs ermöglicht. (AD0 mit 3,3V ändert die Adresse zu x069)

```
#include <MPU6050_tockn.h>
#include <MPU6050_tockn69.h>

MPU6050 mpu6050(Wire);
MPU605069 mpu60502(Wire);
Wire.begin();
mpu6050.begin();
mpu6050.calcGyroOffsets(true);
mpu60502.begin();
mpu60502.calcGyroOffsets(true);

mpu6050.update();
Serial.print("angleX : ");
Serial.println(mpu6050.getAngleX());

Serial.print("angleX2 : ");
Serial.println(mpu60502.getAngleX());
mpu60502.update();
```

### Kontroller Gehäuse
Die Idee war ein schlichtes Grundkonzept, welches unteranderem für den einhändigen Gebrauch geeigent sein sollte.

![Screenshot 2023-12-20 234227](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/419ace2a-d412-42a6-a761-d5a48181a605)
