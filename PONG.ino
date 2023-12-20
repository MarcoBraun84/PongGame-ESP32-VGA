#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include <Ressources/CodePage437_8x14.h>
#include <Ressources/CodePage437_9x16.h>
#include <Ressources/CodePage437_8x19.h>
#include <MPU6050_tockn.h>
#include <MPU6050_tockn69.h>

void Ball_Bewegung();
void point_scored(int player);
void draw_player_paddle1();
void draw_player_paddle2();
void Spielfeld();
void Zufall();

//Pin Konfiguration VGA
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//Pin Speaker
const int speaker_out = 15;

//Pin Switch
const int Schalter = 18;
bool Schalter_Zustand;

//Kontroller Pin Zustände
bool p1_up_true;
bool p1_down_true;
bool p2_up_true;
bool p2_down_true;

//Konfiguration Spieler Schlaeger
const int B_Schlaeger = 5;
const int H_Schlaeger = 40;

const int p1_x = 0;
int p1_y = 50;
const int p2_x = 320 - B_Schlaeger; 
int p2_y = 100;

//Punktestand
int p1_score = 0;
int p2_score = 0;

// Ball Position und Physik
double vx = 1;
double vy = 1;
int ball_x = 50;
int ball_y = 150;

int radius = 3;

//Spielmodus
//true = KI, false = Spieler
bool Spielmodus1 = 0;
bool Spielmodus2 = 0;

//Spielfeldbegrenzung
bool Spielfeldbegrenzung = true;

//KI Zustand
bool KI1_Zustand = 0;
double Variation_1 = 0;

VGA3Bit vga;

MPU6050 mpu6050(Wire);
MPU605069 mpu60502(Wire);

void setup(){
  Serial.begin(115200);
  vga.setFrameBufferCount(2);
	Mode myMode = vga.MODE320x240;
	//print the parameters
	//myMode.print<HardwareSerial>(Serial);

	vga.init(myMode, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

  pinMode(Schalter, INPUT);
  pinMode(speaker_out, OUTPUT);

//MPU6050
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  mpu60502.begin();
  mpu60502.calcGyroOffsets(true);
}

void loop(){
  vga.clear(0);
  Schalter_Zustand = digitalRead(Schalter);

  if (Schalter_Zustand) Spielfeldbegrenzung = false; 
  else Spielfeldbegrenzung = true;
  
  Spielfeld();
  Ball_Bewegung();
  draw_player_paddle1();
  draw_player_paddle2();
  
  vga.show();
  delay(5);
}

void Ball_Bewegung(){
//Links
  if (ball_x - radius < B_Schlaeger) {
    vx = vx * - 1;
    Zufall();
  //Punkt P2
    if (ball_y > p1_y + H_Schlaeger) point_scored(2);
    else if (ball_y + radius < p1_y) point_scored(2);
    else tone(speaker_out, 440, 10);
  }
//Rechts
  if (ball_x + radius > p2_x) {
    vx = vx * - 1;
    Zufall();
  //Punkt P1
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

void Zufall(){
  if (vx < 0) vx = vx - 1 * random(0.5, 1);
  else vx = vx + 1 * random(0.5, 1);

  if (vy < 0) vy = vy - 1 * random(0.2, 1);
  else vy = vy + 1 * random(0.2, 1);
}

void point_scored(int player) {
  KI1_Zustand = 0;
//Punkte-Soundeffekt
  tone(speaker_out, 500, 150);
  delay(300);
  tone(speaker_out, 500, 150);
  delay(100);
  tone(speaker_out, 500, 150);
  delay(100);
  tone(speaker_out, 600, 250);
  delay(500);

  if (player == 1) {
    p1_score++;
    if (Spielfeldbegrenzung) {
      vx = 1;
      vy = 1;                                                 
      ball_x = B_Schlaeger + radius;
      ball_y = random(15, 225);
    }
  } 
  else if (player == 2) {
    p2_score++;
    if (Spielfeldbegrenzung) {
      vx = -1;
      vy = 1;
      ball_x = p2_x - radius;
      ball_y = random(15, 225);
    }
  }
}

void draw_player_paddle1() {
  if (KI1_Zustand = 0) {
    Variation_1 = random(-2, 3)* H_Schlaeger;
    KI1_Zustand = 1;
  }

//KI1
  if (Spielmodus1 == true && vx == -1) { 
    //if (ball_y > p1_y + H_Schlaeger/2) p1_y = p1_y + 1;
    //else if (ball_y < p1_y + H_Schlaeger/2) p1_y = p1_y - 1; 
    if (ball_y > p1_y + Variation_1) p1_y = p1_y + 1;
    else if (ball_y < p1_y + Variation_1) p1_y = p1_y - 1; 
  }
  
  if (mpu6050.getAngleX() > 3) p1_y--; 
  else if (mpu6050.getAngleX() < -3) p1_y++;
  Serial.print("angleX : ");
  Serial.println(mpu6050.getAngleX());
  mpu6050.update();

//Oben & Unten-Problematik  
  if (p1_y < 11) p1_y = 11;
  if (p1_y > 190) p1_y = 190;
  vga.fillRect(p1_x, p1_y, B_Schlaeger, H_Schlaeger, vga.RGB(255, 255, 255));
}

void draw_player_paddle2() {
//KI2
  if (Spielmodus2 == true && vx == 1) { 
    if (ball_y > p2_y + H_Schlaeger/2) p2_y = p2_y + 1;
    else if (ball_y < p2_y + H_Schlaeger/2) p2_y = p2_y - 1; 
  } 

  if (mpu60502.getAngleX() > 5) p2_y--; 
  else if (mpu60502.getAngleX() < -5) p2_y++;
  Serial.print("angleX2 : ");
  Serial.println(mpu60502.getAngleX());
  mpu60502.update();

//Oben & Unten-Problematik 
  if (p2_y < 11) p2_y = 11; 
  if (p2_y > 190) p2_y = 190;
  vga.fillRect(p2_x, p2_y, B_Schlaeger, H_Schlaeger, vga.RGB(255, 255, 255));
}

void Spielfeld(){
//Spielfeldbegrenzung oben&unten zeichnen
  vga.fillRect(0, 2, vga.xres, 10, vga.RGB(255, 255, 255));
  vga.fillRect(0, vga.yres - 8, vga.xres, 10, vga.RGB(255, 255, 255));
//"PONG" Schriftzug
  vga.setFont(CodePage437_8x19);
	vga.setCursor(vga.xres/2-50, vga.yres/2-10);
	vga.setTextColor(vga.RGB(255,0,0));
	vga.print("Pong Game!");
//Spieleranzeige Links
  vga.setFont(Font6x8);
  vga.setCursor(2, 4);
	vga.setTextColor(vga.RGB(0));
  if (Spielmodus1){
    vga.setTextColor(vga.RGB(0,0,255));
    vga.print("KI1:");
  } else {
    vga.setTextColor(vga.RGB(0));
    vga.print("P1:");
  }
  vga.print(p1_score);
//Spileranzeige Rechts
  vga.setCursor(vga.xres-50, 4);
  if (Spielmodus2){
    vga.setTextColor(vga.RGB(0,0,255));
    vga.print("KI2:");
  } else {
    vga.setTextColor(vga.RGB(0));
    vga.print("P2:");
  }
	vga.print(p2_score);
//reselution
  vga.setTextColor(vga.RGB(0));
	vga.setCursor(2, vga.yres-8);
	vga.print(vga.xres);
	vga.print("x");
	vga.print(vga.yres);
}