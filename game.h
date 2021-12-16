#ifndef GAME_H
#define GAME_H
#include "special_chars.h"
#include "display.h"
#include <Arduino.h>
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {25, 35, 33, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27, 23, 31}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void createChars() {
  lcd.createChar(0, firstbar);
  lcd.createChar(1, midbar);
  lcd.createChar(2, endbar);
  lcd.createChar(3, firstempty);
  lcd.createChar(4, midempty);
  lcd.createChar(5, endempty);
}

void holdbar(int pos, int ch){
  lcd.setCursor(pos,3);
  lcd.write((byte)ch);
}    
class Game{
  public:
    void startcount(unsigned long counter, unsigned long a, unsigned long d, bool&stcountdown){
      lcd.clear();
      lcd.setCursor(9,1);
      if (counter == 0){
        disarm = d;
        arm = a;
        stcountdown = false;
      }
      lcd.print(counter < 10 ? "0"+String(counter): String(counter));
    }
    void plant(int buttontime, unsigned long secondTime, int code, bool& armordisarm, int&winorlose){
      displaytime(secondTime, armordisarm, winorlose);
      displaybar(buttontime);
      displaycode(code, buttontime);
      displaylock(armordisarm, winorlose);
    }
    void displaytime(unsigned long secondTime, bool armordisarm, int&winorlose){
      if(armordisarm){
        gameptr = &arm;
      }
      else {
        gameptr = &disarm;
      }
      if(secondTime >= 1000){
        (*gameptr)--;
      }
      if(arm == 0){
        winorlose = 2;
      }else if(disarm == 0){
        winorlose = 1;
      }
      unsigned long mytime = *gameptr;
      int hours = mytime/3600;
      mytime = mytime % 3600;
      int minutes = mytime/60;
      mytime = mytime % 60;
      int seconds = mytime;
      lcd.setCursor(6, 0);
      String h, m, s;
      h = hours < 10 ? "0" + String(hours): String(hours);
      m = minutes < 10 ? "0" + String(minutes): String(minutes);
      s = seconds < 10 ? "0" + String(seconds): String(seconds);
      lcd.print(h+":" +m+":"+s);
    }
    
    void displaybar(int buttonTime){
      
      if(buttonTime == 0){
        holdbar(5,3);
        holdbar(6,4);
        holdbar(7,4);
        holdbar(8,4);
        holdbar(9,4);
        holdbar(10,4);
        holdbar(11,4);
        holdbar(12,4);
        holdbar(13,4);
        holdbar(14,5);
      }
      else
      {
        for(int i = 0; i < buttonTime; i++){
            int a;
            if(i == 0)
              a = 0;
            else if(i == 9)
              a = 2;
            else 
              a = 1;
            holdbar(i+5, a);
        }
      }
    }
    void displaylock(bool&armordisarm, int&winorlose){
      
       char c = keypad.getKey();
       if(c){
         if(c == '#'){
           Serial.println("inputted:" + inputted + ":"+mycode);
           if(inputted == mycode){
            if(armordisarm)
              armordisarm = false;
            else{
              winorlose = 2;
            }
           }
           inputted = "";
         }
         else if(c != '*' && inputted.length() < 4){
           inputted+=c;
         }
       }
       if(inputted == ""){
          lcd.setCursor(7,2);
          if(armordisarm)
             lcd.print("LOCKED");
             
          else
            lcd.print("ARMED!");
        }
       else{
        lcd.setCursor(7,2);
        String temp = inputted;
        for(int i = inputted.length(); i < 4; i++){
          temp = temp + '-';
        }
        lcd.print(" "+temp+" ");
       }
    }
    void displaycode(int code, int buttontime){     
      mycode = String(code);
      for(int i = mycode.length(); i < 4; i++){
        mycode = "0"+mycode;
      }
      lcd.setCursor(8,1);
      if(buttontime == 10){
        lcd.print(String(mycode));
      }
      else{
        lcd.print("****");
      }
    }

    void team1wins(){
      lcd.setCursor(5,1);
      lcd.print("TERRORISTS");
      lcd.setCursor(8,2);
      lcd.print("WIN!");
    }
    void team2wins(){
      lcd.setCursor(1,1);
      lcd.print("COUNTER-TERRORISTS");
      lcd.setCursor(8,2);
      lcd.print("WIN!");
    }
  private:
  unsigned long* gameptr;
  unsigned long disarm;
  unsigned long arm;
  String mycode;
  String inputted;
  
};

#endif
