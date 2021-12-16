
#include "display.h"
#include "game.h"
int pin12 = 12, pin13 = 13;
Display mylcd;
Game mygame;

bool st;
bool stcountdown;
bool finished;
bool armordisarm;
unsigned long startTime;
unsigned long secondTime = 0;
unsigned long codeTime = 0;
int countdown = 10;
int buttontime =0;
int code;
int winorlose;
char c;
int clearcounter = 0;
void setup() {
  Serial.begin(9600);
  pinMode(pin12, INPUT_PULLUP);
  pinMode(pin13, INPUT_PULLUP);
  st = false;
  mylcd.start();
  mylcd.menu();
  randomSeed(analogRead(A0));
  code = random(0, 10000);
  stcountdown= true;
  finished = false;
  armordisarm = true;
  createChars();
  startTime = millis();
}

void loop() {
  
  if(!st){
    char key = keypad.getKey();
    if(key){
      mylcd.menuInput(key, st);
    }
  }
  else if(stcountdown){
    unsigned long currentTime = millis()- startTime;
    if(currentTime >= 1000){
      startTime += currentTime;
      mygame.startcount(countdown, mylcd.get_arm(),mylcd.get_disarm(),stcountdown);
      armordisarm = true;
      winorlose = 0;
      finished = false;
      countdown--;
    }
  }
  else{
    unsigned long currentTime = millis()- startTime;
    if(winorlose == 0){
      
      if(currentTime >=500){
        startTime += currentTime;
        secondTime += currentTime;
        codeTime += currentTime;
        if(codeTime >=6000){
          code = random(0, 10000);
          codeTime = 0;
          Serial.println(code);
        }
        
      }
      if(digitalRead(pin12)== HIGH && digitalRead(pin13) == HIGH){
          if(currentTime >=500){
            if(buttontime < 10){
              buttontime++;
            }
            if(buttontime == 9){
              code = random(0, 10000);
              codeTime = 0;
              Serial.println(code);
            }
          }
        }
        else{
          buttontime = 0;
        }
      mygame.plant(buttontime, secondTime, code, armordisarm, winorlose);
      if(secondTime >= 1000)
        secondTime = 0;
    }
    else if(winorlose ==1){
      if(clearcounter == 0){
        lcd.clear();
        clearcounter++;
      }
      mygame.team1wins();
      if(currentTime >= 5000)
        finished = true;
    }
    else if(winorlose == 2){
      if(clearcounter == 0){
        lcd.clear();
        clearcounter++;
      }
      mygame.team2wins();
      if(currentTime >= 5000){
        finished = true;
      }
    }
    
    if(finished){    
      st = false;
      stcountdown = true;
      startTime = millis();
      countdown = 10;
      secondTime = 0;
      clearcounter = 0;
      finished = false;
      mylcd.menu();
      
    }
  }
}
