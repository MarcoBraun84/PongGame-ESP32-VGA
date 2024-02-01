# pong-esp32-vga
## Aufgabenstellung
Es soll eine Spielekonsole, basierend auf einem ESP32, entwickelt werden. Die Spielersteuerung erfolgt über zwei Lagesensoren (MPU6050). Die Ausgabe erfolgt über ein Monitor, der über VGA angesteurt wird.

## Software Installation
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Library MPU6050](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/MPU6050_tockn.zip), [github Library MPU6050](https://github.com/Tockn/MPU6050_tockn)
- [Library MPU605069](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/MPU6050_tockn69.zip)
- [Library bitluni_ESP32Lib](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/bitluni_ESP32Lib.zip), [github bitluni ESP32lib](https://github.com/bitluni/ESP32Lib)

Nach der installation der Arduino IDE, müssen die Librarys installiert werden, um den Code auszuführen. Hierfür kann in der IDE unter ```Sketch -> Include Library -> Add .ZIP Library``` die verlinkten Zip-Librarys installiert werden.

Weitherin muss das ESP32-Board hinzugefügt werden. Hierzu wird unter ```Tools -> Board -> Boards Manager...``` "esp32 by Espressif Systems" installiert. Anschließend kann unter ```Tools -> Board -> esp32``` "ESP32 Dev Module" ausgewählt werden. Zum Hochladen des [PONG.ino](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/PONG.ino)-Codes muss nur noch unter ```Tools -> Port``` der angeschlossene ESP32-Port eingestellt werden.

## Hardware
Benötigt wird

- [ESP32-Entwicklerboard](https://www.reichelt.de/nodemcu-esp32-wifi-und-bluetooth-modul-debo-jt-esp32-p219897.html?&trstct=pos_0&nbc=1)
- 2x [MPU6050-Lagesensor](https://www.reichelt.de/entwicklerboards-beschleunigung-gyroskop-3-achsen-mpu-6050-debo-hmc5883l-2-p282539.html?&trstct=pos_0&nbc=1)
- [Jumper Kabel / Steckbrückenkabel](https://www.reichelt.de/entwicklerboards-steckbrueckenkabel-20cm-3x-20-kabel-debo-kabelset8-p280591.html?&trstct=pos_2&nbc=1)
- [VGA-Kabel](https://www.reichelt.de/vga-monitor-kabel-15-pol-vga-stecker-1-m-st-mxtmmhq1m-p274592.html?&trstct=vrt_pdn&nbc=1)
- 3x [Knöpfe](https://www.kaufland.de/product/419690543/?utm_source=shopping&utm_medium=non-paid&utm_campaign=pricecomparison&sid=36742452)

Die Steuerung der Spieler erfolgt über die Lagesensoren. Über zwei Knöpfe wird der Spielmodus von Spiler 1 und Spieler 2 eingestellt (KI on/off). Der dritte Knopf paussiert bzw. setzt das Spiel fort.

#### VGA-Kbael vorbereiten
Die Bildübertragung benötigt 6 Ardern (R, G, B, V-Syc, H-Syc, GRD). Diese können durch Jumper-Kabel direkt vom VGA-Kabelstecker abgegriffen werden (siehe Bild).

![Screenshot 2023-12-20 233102](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/7121787a-c713-4a21-82c4-fd71c8974b97)

Alternativ kann das VGA-Kabel direkt verlötet werden, hierbei bietet es sich an, die Ardern mit einer Durchgangsprüfung zu bestimmen.

### Schaltplan:
Alle Komponten müssen wie im dargestellten Schaltplan verbunden werden.


Die Lagesensoren werden über einen I2C-Bus verbunden. 

### 3D-Druck: Gehäuse, Kontroller

Grundidee für das designen der Gehäuse ist ein komplett modualres Konzept, um alle Bauteile bei anderer Verwendung wiederzunutzen. Zudem sollten keine Schrauben zur Montage nötig sein.

#### Gehäuse:
![Gehäuse Konsole](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/7bbf8f2c-fc84-4adc-a0d5-0c7b5efcb596)
![Gehäusedeckel Konsole](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/76756885-c001-4632-9bed-7917026a0189)

#### Kontroller Player 1: 
![Kontroller 1](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/f1b671e0-d6c7-474e-93c1-5dd18ab367cc)

#### Kontroller Player 2: 
![Kontroller 2](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/ef02c4bd-ee9f-4f61-a6d0-01b6baf0824e)


## Zusätzliche Codeerklärung

### VGA Monitor Ausgabe
Hierfür wurde die Libary [bitluni ESP32lib](https://github.com/bitluni/ESP32Lib) genutzt.
Um diese grundlegend nutzen zu können, sind folgende Befehle nötig:  

```
#include <ESP32Lib.h>

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

### Pong Code
Als grundlage deinte [dieses Projekt](https://github.com/nickbild/pico_pong). Hierruas wurde der grundeliegendene *Game Loop* und die *draw_player_paddle*-Funktion entnommen.
Daraus entstant die **Ball_Bewegung**-Funktion:
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
- Zufall-Funktion: verändert die Ballbewegung bei einer Berührung sehr unwahrscheinlich
- tone-Funktiion: erzeugt über einen Piper abprall Geräusche des Balls
- vga.fillEllipse-Funktion: erstellt das Bild des Balls

Zudem entstant die **draw_player_paddle1**-Funktion, sowie die **draw_player_paddle2**-Funktion.
```
void draw_player_paddle1() {
  if (KI1_Zustand = 0) {
    Variation_1 = random(-2, 3)* H_Schlaeger;
    KI1_Zustand = 1;
  }
  //KI1
  if (Spielmodus1 == true && vx < 0) { 
    if (ball_y > p1_y + Variation_1) p1_y = p1_y + 1;
    else if (ball_y < p1_y + Variation_1) p1_y = p1_y - 1; 
  }
  //Oben & Unten-Problematik  
  if (p1_y < 11) p1_y = 11;
  if (p1_y > 190) p1_y = 190;
  vga.fillRect(p1_x, p1_y, B_Schlaeger, H_Schlaeger, vga.RGB(255, 255, 255));
}
```

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
### Knopf-Funktion bzw. debouncing

https://docs.arduino.cc/built-in-examples/digital/Debounce
