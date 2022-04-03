#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 3

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const int TEAM_A = 8;
const int TEAM_B = 9;
const int TEAM_C = 10;

const int JURI_TRUE = 5;
const int JURI_FALSE = 6;
const int JURI_STANDBY = 7;

int scoreA = 8;
int scoreB = 0;
int scoreC = 0;

boolean standbyTime = false;

boolean lastStandby = false;

boolean onlyStandby = true;

int answeringTeam;

//meril -> membuat array yang akan diisi maksimal 3 data tim yang menjawab salah
int falseAnsweringTeam[3] = {0,0,0};
//meril -> membuat array yang akan diisi maksimal 3 data tim yang menjawab salah

void setup() {
  Serial.begin(4800);
  
  myDisplay.begin();

  myDisplay.setIntensity(0);

  myDisplay.displayClear();
  
  pinMode(TEAM_A, INPUT_PULLUP);
  pinMode(TEAM_B, INPUT_PULLUP);
  pinMode(TEAM_C, INPUT_PULLUP);

  pinMode(JURI_TRUE, INPUT_PULLUP);
  pinMode(JURI_FALSE, INPUT_PULLUP);
  pinMode(JURI_STANDBY, INPUT_PULLUP);
}

void loop() {
  if(standbyTime){
    myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print("STDBY");
    if(digitalRead(TEAM_A) == LOW){
        if(falseAnsweringTeam[0] == 1){
          return;
        }
      Serial.println("Tim A Menekan Tombol");
      answeringTeam = 1;
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("A" + String(scoreA));
      standbyTime = false;
      onlyStandby = false;
      delay(500);
    }
    
    if(digitalRead(TEAM_B) == LOW){
        if(falseAnsweringTeam[1] == 1){
          return;
        }
      Serial.println("Tim B Menekan Tombol");
      answeringTeam = 2;
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("B" + String(scoreB));
      standbyTime = false;
      onlyStandby = false;
      delay(500);
    }
    
    if(digitalRead(TEAM_C) == LOW){
        if(falseAnsweringTeam[2] == 1){
          return;
        }
      Serial.println("Tim C Menekan Tombol");
      answeringTeam = 3;
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("C" + String(scoreC));
      standbyTime = false;
      onlyStandby = false;
      delay(500);
    }
    if (digitalRead(JURI_STANDBY) == LOW && lastStandby == true){
      Serial.println("JURI MENAMPILKAN SEMUA NILAI, SEKALIGUS STANDBY");
      delay(500);
      lastStandby = false;
      standbyTime = true;
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("A" + String(scoreA));
      delay(2000);
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("B" + String(scoreB));
      delay(2000);
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("C" + String(scoreC));
      delay(2000);
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("STDBY");
    }
  }
  if (standbyTime == false || onlyStandby == true) {
    if(digitalRead(JURI_STANDBY) == LOW){
      lastStandby = true;
      Serial.println("SEKARANG STANDBY");
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("STDBY");
      delay(250);
      standbyTime = true;
      falseAnsweringTeam[0] = 0;
      falseAnsweringTeam[1] = 0;
      falseAnsweringTeam[2] = 0;
    }
  }
  if (answeringTeam == 1 || answeringTeam == 2 || answeringTeam == 3){
    if(onlyStandby == false){
      if(digitalRead(JURI_TRUE) == LOW){
        Serial.println("JURI NAMBAH NILAI");
        delay(500);
        scoringSystem(answeringTeam, true);
      }
      if(digitalRead(JURI_FALSE) == LOW){
        Serial.println("JURI NGURANGI NILAI");
        delay(500);
        scoringSystem(answeringTeam, false);
      }
    }
  }
}

void scoringSystem(int answeringTeam, boolean addScore){
  
  if(addScore == true){
    if(answeringTeam == 1){
      scoreA = scoreA + 2;
      onlyStandby = true;
      myDisplay.print("A" + String(scoreA));
    }
    if(answeringTeam == 2){
      scoreB = scoreB + 2;
      onlyStandby = true;
      myDisplay.print("B" + String(scoreB));
    }
    if(answeringTeam == 3){
      scoreC = scoreC + 2;
      onlyStandby = true;
      myDisplay.print("C" + String(scoreC));
    }
  }
  if(addScore == false){
    if(answeringTeam == 1){
      scoreA--;
      myDisplay.print("A" + String(scoreA));
      onlyStandby = true;
      standbyTime = true;
      lastStandby = false;
      falseAnsweringTeam[0] = 1;
    }
    if(answeringTeam == 2){
      scoreB--;
      onlyStandby = true;
      standbyTime = true;
      lastStandby = false;
      myDisplay.print("B" + String(scoreB));
      falseAnsweringTeam[1] = 1; 
    }
    if(answeringTeam == 3){
      scoreC--;
      onlyStandby = true;
      standbyTime = true;
      lastStandby = false;
      myDisplay.print("C" + String(scoreC));
      falseAnsweringTeam[2] = 1;
    }
  }
}
