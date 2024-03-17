#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include <Ressources/CodePage437_8x19.h>
#include <MPU6050_tockn.h>
#include <MPU6050_tockn69.h>

void Ball_Bewegung();
void point_scored(int player);
void draw_player_paddle1();
void draw_player_paddle2();
void Spielfeld();
void Zufall();
void Knoepfe();

//Pin Konfiguration VGA
const int redPin = 14;
const int greenPin = 25;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//Pin Speaker
const int speaker_out = 23;

//Knöpfe
const int buttonPin[] = {19, 13, 4};
int buttonState[] = {0, 0, 0}; 
//Debounce
int lastButtonState[] = {0, 0, 0};
unsigned long lastDebounceTime[] = {0, 0, 0};
unsigned long debounceDelay = 50; 

//Konfiguration Spieler Schlaeger
const int B_Schlaeger = 5;
const int H_Schlaeger = 40;

const int p1_x = 0;
int p1_y = 100;
const int p2_x = 318 - B_Schlaeger; 
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
bool Pause = 0;
bool p = 1;
//true = KI, false = Spieler
bool Spielmodus1 = 1;
bool Spielmodus2 = 1;

//Spielfeldbegrenzung
bool Spielfeldbegrenzung = 1;

//KI Zustand
bool KI1_Zustand = 0;
double Variation_1 = 0;
bool KI2_Zustand = 0;
double Variation_2 = 0;

bool Zufall_Ball = 0;

VGA3Bit vga;

MPU6050 mpu6050(Wire);
MPU605069 mpu60502(Wire);

void setup(){
  Serial.begin(115200);
  vga.setFrameBufferCount(2);
	Mode myMode = vga.MODE320x240;
	//myMode.print<HardwareSerial>(Serial);
	vga.init(myMode, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  pinMode(speaker_out, OUTPUT);


//MPU6050
  Wire.begin();
  mpu60502.begin();
  mpu60502.calcGyroOffsets(true);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

//Knöpfe
  pinMode(buttonPin[0], INPUT);
  pinMode(buttonPin[1], INPUT);
  pinMode(buttonPin[2], INPUT);
}

void loop(){
  Knoepfe();
  if (Pause){
    if (p){
      vga.setFont(CodePage437_8x19);
	    vga.setCursor(vga.xres/2-20, vga.yres/3);
	    vga.setTextColor(vga.RGB(255,255,255));
	    vga.print("Pause");
      vga.show();
      p = 0;
    }
  } else {
    p = 1;
    vga.clear(0);
    Spielfeld();
    Ball_Bewegung();
    draw_player_paddle1();
    draw_player_paddle2();
    vga.show();
    delay(5);
  }
}

void Knoepfe(){
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
  if (mpu6050.getAngleX() > 4) p1_y--; 
  else if (mpu6050.getAngleX() < -5) p1_y++;
  mpu6050.update();
  
  if (mpu60502.getAngleX() > 3) p2_y--; 
  else if (mpu60502.getAngleX() < -7) p2_y++;
  mpu60502.update();

  Serial.print("X1: "); 
  Serial.print(mpu6050.getAngleX()); 
  Serial.print(";\t X2: "); 
  Serial.println(mpu60502.getAngleX()); 
}

void Ball_Bewegung(){
  //Links
  if (B_Schlaeger + radius > ball_x) {
    vx = vx * - 1;
    Zufall();
    //Punkt P2
    if (ball_y > p1_y + H_Schlaeger || ball_y + radius < p1_y) point_scored(2);
    else {
      tone(speaker_out, 440, 10);
      KI2_Zustand = 0;}
  }
  //Rechts
  if (ball_x + radius > p2_x) {
    vx = vx * - 1;
    Zufall();
    //Punkt P1
    if (ball_y > p2_y + H_Schlaeger || ball_y + radius < p2_y) point_scored(1);
    else {
      tone(speaker_out, 440, 10);
      KI1_Zustand = 0;}
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

void Zufall(){
  if (Zufall_Ball) {
	  if (vx < 0) vx = -1 - 0.001*random(1, 1000);
	  else vx = 1 + 0.001*random(1, 1000);

	  if (vy < 0) vy = -1 - 0.001*random(2, 1000);
	  else vy = 1 + 0.001*random(2, 1000);
  }
}

void point_scored(int player) {
  KI1_Zustand = 0;
  KI2_Zustand = 0;
  //Punkte-Soundeffekt
  if (Spielfeldbegrenzung){
    tone(speaker_out, 500, 150);
    delay(300);
    tone(speaker_out, 500, 150);
    delay(100);
    tone(speaker_out, 500, 150);
    delay(100);
    tone(speaker_out, 600, 250);
    delay(500);
  }

  if (player == 1) {
    p1_score++;
    if (Spielfeldbegrenzung) {
      vx = 1;
      vy = 1;                                                 
      ball_x = B_Schlaeger + radius;
      ball_y = random(15, 225);
    }
  } 
  if (player == 2) {
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
    Variation_1 = random(-3, 3)* H_Schlaeger;
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

void draw_player_paddle2() {
  if (KI2_Zustand = 0) {
    Variation_2 = random(-3, 3)* H_Schlaeger;
    KI2_Zustand = 1;
  }  
//KI2
  if (Spielmodus2 == true && vx > 0) { 
    if (ball_y > p2_y + Variation_2) p2_y = p2_y + 1;
    else if (ball_y < p2_y + Variation_2) p2_y = p2_y - 1; 
  } 

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
	vga.setCursor(vga.xres/2-35, vga.yres/2-10);
	vga.setTextColor(vga.RGB(255,0,0));
	vga.print("Pong Game!");

  //Spieleranzeige Links
  vga.setFont(Font6x8);
  vga.setCursor(2, 4);
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

  //Auflösung anzeigen
  vga.setTextColor(vga.RGB(0));
	vga.setCursor(2, vga.yres-8);
	vga.print(vga.xres);
	vga.print("x");
	vga.print(vga.yres);
}