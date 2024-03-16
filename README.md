# pong-esp32-vga
Es soll eine PONG-Spielekonsole entwickelt werden die für zwei Spiler ausgelegt ist und auf dem ESP32 Mikrokontroller basiert. 
Ziel des Projektes ist, mithilfe von zwei Lagesensoren (MPU6050) das Spiel Pong auf einem über VGA angesteuerten Monitor gegeneinander spielen zu können.
Drei Funktionsknöpfe ermöglichen, das Spiel zu pausieren und die KI1 bzw. KI2 ein- oder auszuschalten. 
Über ein Piezo werden Spielgeräusche, wie Ballaufprallgeräusche und Punktemedlodie, erzeugt.


## Hardware
<img align="right" src="https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/d2e03790-8c00-4ce0-8b58-bf102e5905b7" width="300">

Benötigt werden folgende Komponenten:
- [ESP32-Entwicklerboard](https://www.reichelt.de/nodemcu-esp32-wifi-und-bluetooth-modul-debo-jt-esp32-p219897.html?&trstct=pos_0&nbc=1)
- 2x [MPU6050-Lagesensor](https://www.reichelt.de/entwicklerboards-beschleunigung-gyroskop-3-achsen-mpu-6050-debo-hmc5883l-2-p282539.html?&trstct=pos_0&nbc=1)
- [Jumper Kabel / Steckbrückenkabel](https://www.reichelt.de/entwicklerboards-steckbrueckenkabel-20cm-3x-20-kabel-debo-kabelset8-p280591.html?&trstct=pos_2&nbc=1)
- [VGA-Kabel](https://www.reichelt.de/vga-monitor-kabel-15-pol-vga-stecker-1-m-st-mxtmmhq1m-p274592.html?&trstct=vrt_pdn&nbc=1)
- 3x [Knöpfe](https://www.kaufland.de/product/419690543/?utm_source=shopping&utm_medium=non-paid&utm_campaign=pricecomparison&sid=36742452)
- 3x [10k Widerstände](https://www.reichelt.de/widerstand-kohleschicht-10-kohm-0207-250-mw-5--1-4w-10k-p1338.html)
- [Piezo](https://www.reichelt.de/de/de/piezo-schallwandler-85-db-4-khz-summer-epm-121-p35927.html?&trstct=pos_0&nbc=1)


#### VGA-Kabel vorbereiten
Die Bildübertragung benötigt 6 Ardern (R, G, B, V-Syc, H-Syc, GRD) des VGA-Kabels, um ein Bild mit 3-Bit-Farbraum zu übertragen. Diese können durch Jumper-Kabel direkt vom VGA-Kabelstecker abgegriffen werden (siehe Bild).

![Screenshot 2023-12-20 233102](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/7121787a-c713-4a21-82c4-fd71c8974b97)

Oder man kann das VGA-Kabel direkt verlötet werden, hierbei bietet es sich an, die einzelnen Ardern mit einer Durchgangsprüfung zu bestimmen.


### Schaltplan:
![image](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/f3d59562-bde2-493a-a981-77f30469e8db)

Die Lagesensoren benötigen mindestens 3,3V Versorgungsspannung und werden über einen I2C-Bus (SDA, SCL) verbunden. Der zweite Lagesensor erhält zusätzlich 3,3V auf I2C-Adresspin (AD0), damti dieser eine andere Busadresse erhält. Die Knöpfe schalten auf dem HIGH-Zustand. Die Stromversorgung wird über den intergrierten USB-C Eingang des ESP32 sichergestellt.

### 3D-Druck: Gehäuse, Kontroller
Ziel beim entwickeln der Konsolen- und Kontroller-Gehäuse war es, ein möglichst modulares Konzept zu verfolgen, um alle Bauteile bei anderer Nutzung wieder verwenden zu können. Zudem sollte die Gehäuse keine Schrauben für die Montage benötigen.

Die [Modelle](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/3D-Druck%20Modelle.zip) benötigen folgende Druckeinstellungen:

```
Rafts:No
Supports:No
Infill:10%
Filament: PLA
Nozzel: 0.4 mm
Layer height: 0.2
Wall Line Count: 3
```


#### Konsolengehäuse
![Gehäuse Konsole + Deckel](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/79d7c0b8-aae6-45aa-bf49-9ed77179f16b)

Im Konsolengehäuse sind Halterungen für den ESP32 und einen Piper vorgesehen. 
Auf der Vorderseite befindet sich mittig ein "PONG" Schriftzug und zwei kleinere Spielerbezeichnung, die für die passende Zuordnung der Kontroller und der darüber liegenden Funktionsknöpfe dienen.
An den beiden Seitenflächen befinden sich die Kabelausgänge der beiden Kontroller. 
Auf der rechten Seite befindet sich zudem eine größere Aussparung für den ESP32 USB-C Anschluss, der zugleich auch als Stromversogung dient.
Auf der Rückseite des Konsolengehäuses befindet sich der VGA-Kabelausgang.
Im Konsolendeckel wurden passende Aussparungen für die drei Knöpfe gelassen, die mit einem innenliegenden Steg verstärkt wurden. 


#### Kontrollergehäuse
![Kontroller 1   2 Konstruktion + Real](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/assets/117396763/218a894a-f1be-4176-b77e-7c4cac5f6a94)


Die Kontroller wurden so designt, das der MPU6050 in das Gehäuse reingesteckt werden kann und selbständig hälten. Auf der Oberseite befindet sich die Spielerbezeichnung P1 bzw P2.
Die Kontroller-Form ermöglichen dem Spieler, den Kontroller einhändig auf dem Tisch oder zweihändig zu bedienen. 


## Software Installation
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Library MPU6050](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/MPU6050_tockn.zip), [github Library MPU6050](https://github.com/Tockn/MPU6050_tockn)
- [Library MPU605069](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/MPU6050_tockn69.zip)
- [Library bitluni_ESP32Lib](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/bitluni_ESP32Lib.zip), [github bitluni ESP32lib](https://github.com/bitluni/ESP32Lib)

Um den Game-Code zu kompilieren und zu übertragen muss die Arduino IDE installiert werdem. Nach der Installation der IDE, müssen die Librarys installiert werden, um den Code ausführen zukönnen. Hierfür kann in der IDE unter ```Sketch -> Include Library -> Add .ZIP Library``` die verlinkten Zip-Librarys installiert werden. Alternativ könnte man über ```Sketch -> Include Library -> Library Manger``` die Librarys gesucht und installiert werden (nicht die modifizierte MPU605069-Library).

Weitherin muss das ESP32-Board hinzugefügt werden. Hierzu wird unter ```Tools -> Board -> Boards Manager...``` "esp32 by Espressif Systems" installiert. Anschließend kann unter ```Tools -> Board -> esp32``` "ESP32 Dev Module" ausgewählt werden. Zum Hochladen des [PONG.ino](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/PONG.ino)-Codes muss nur noch unter ```Tools -> Port``` der angeschlossene ESP32-Port eingestellt werden.


### Spielvariablenliste
```bool Spielfeldbegrenzung = 1```: Bei Deaktivierung wird der Ball bei einem Treffer nicht "neu eingeworfen", sondern prallt vom linken bzw. rechten Bildschirmrand ab.

```bool Zufall_Ball = 0```: Bei Aktivierung wird der Ball beim Abprallen zufällig seine Geschwindigkeit ändern.

```bool Spielmodus1 = 1``` bzw. ```bool Spielmodus2 = 1```: Schaltet den "KI"-Spieler ein bzw. aus und wird von den jeweiligen Funktionsknöpfen beeinflusst. 


### VGA Monitor Ausgabe
Um eine Bildausgabe über den mit VGA angesteuerten Monitor zu erreichen, wurde die Libary [bitluni ESP32lib](https://github.com/bitluni/ESP32Lib) verwendet. Mit unserem ESP32 ist eine maximale Auflösung von 320x240 Pixel möglich. 

Um beispielhafte ein Pong-Schriftzug über VGA auszugeben ist folgender Code nötig:  

```
#include <ESP32Lib.h>
#include <Ressources/CodePage437_8x19.h>

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
  vga.setFont(CodePage437_8x19);
	vga.setCursor(vga.xres/2-35, vga.yres/2-10);
	vga.setTextColor(vga.RGB(255,0,0));
	vga.print("Pong Game!");
  vga.show();
}
```

### Pong Code
Als Grundlage diente [dieses Projekt](https://github.com/nickbild/pico_pong). Hieraus wurde der zugrundeliegende *Game Loop* und die *draw_player_paddle*-Funktion entnommen.
Daraus entstand die **Ball_Bewegung**-Funktion, die das Bewegungsverhalten, die Abprallgeräusche und die Darstellung des Balls übernimmt.
```
void Ball_Bewegung(){
  //Links
  if (B_Schlaeger + radius > ball_x) {
    vx = vx * - 1;
    Zufall();
    //Punkt P2
    if (ball_y > p1_y + H_Schlaeger || ball_y + radius < p1_y) point_scored(2);
    else tone(speaker_out, 440, 10);
  }
  //Rechts
  if (ball_x + radius > p2_x) {
    vx = vx * - 1;
    Zufall();
    //Punkt P1
    if (ball_y > p2_y + H_Schlaeger || ball_y + radius < p2_y) point_scored(1);
    else tone(speaker_out, 440, 10);
  }
  //Unten&Oben
  if (ball_y < 12 + radius || ball_y > 230 - radius) {
    vy = vy * -1;
    Zufall();
    tone(speaker_out, 500, 10);
  }
  ball_x = ball_x + vx;
  ball_y = ball_y + vy;
  vga.fillEllipse(round(ball_x), round(ball_y), radius, radius, vga.RGB(255, 255, 255));
}
```
- Zufall-Funktion: verändert die Ballbewegung bei einem Abprall (```bool Zufall_Ball = 1```)
- tone-Funktiion: erzeugt über einen Piper unterschiedliche Abprallgeräusche des Balls
- vga.fillEllipse-Funktion: erstellt das Bild des Balls

Weiterhin entstand die **draw_player_paddle1**-Funktion, sowie die **draw_player_paddle2**-Funktion. Diese beinhalten die anschaltbaren "KIs", die Spielereingabe und die Darstellung.

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
Leider unterstützt die genutzte Libary [MPU6050_tockn](https://github.com/Tockn/MPU6050_tockn) keine Adressierung des MPU6050, wodurch mehrere MPUs nicht standardmäßig möglich sind. Lediglich durch Dublizieren dieser Library und Ändern der I2C-Adresse werden damit zwei MPUs ermöglicht (AD0 mit 3,3V ändert die Adresse zu x069 mit [MPU605069](https://github.com/tiimiiiiiiiiii/pong-esp32-vga/blob/main/MPU6050_tockn69.zip)).

Um die Funktion der beiden MPUs zu testen, kann der folgende Code verwendet werden. Hierbei werden über den Serial Monitor die Werte der X-Achsen-Auslenkung ausgegeben.

```
#include <MPU6050_tockn.h>
#include <MPU6050_tockn69.h>

void setup(){
  MPU6050 mpu6050(Wire);
  MPU605069 mpu60502(Wire);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  mpu60502.begin();
  mpu60502.calcGyroOffsets(true);

void loop(){
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.println(mpu6050.getAngleX());
  mpu60502.update();
  Serial.print("angleX2 : ");
  Serial.println(mpu60502.getAngleX());
}
```
Hierbei wird zum Start der Konsole eine automatische Kalibrierung der MPUs vorgenommen.


### Knopf-Funktion bzw. Debouncing
Für die drei Knöpfe wurde mit Hilfe [dieser Anleitung](https://docs.arduino.cc/built-in-examples/digital/Debounce) eine erweiterte Debounce-Funktion erstellt, die das Knopfsignal entprellt und damit einen klaren Knopfdruck sicher stellt.
```
const int buttonPin[] = {19, 13, 2};
int buttonState[] = {0, 0, 0}; 
int lastButtonState[] = {0, 0, 0};
unsigned long lastDebounceTime[] = {0, 0, 0};
unsigned long debounceDelay = 50;
---
int reading[] = {digitalRead(buttonPin[0]), digitalRead(buttonPin[1]), digitalRead(buttonPin[2])};
for (int i = 0; i < 3; i++) {
  if (reading[i] != lastButtonState[i]) lastDebounceTime[i] = millis();
  if ((millis() - lastDebounceTime[i]) > debounceDelay) {
    if (reading[i] != buttonState[i]) {
      buttonState[i] = reading[i];
      if (buttonState[0] == HIGH) Spielmodus1 = !Spielmodus1;
      if (buttonState[1] == HIGH) Spielmodus2 = !Spielmodus2;
      if (buttonState[2] == HIGH) Pause = !Pause;
    }
  }
  lastButtonState[i] = reading[i];
}
```
