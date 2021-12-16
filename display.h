#ifndef DISPLAY_H
#define DISPLAY_H
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

const int HOUR_LIMIT = 1;
const int TIME_LIMIT = 10000; //IN 00:00:00 FORMAT
class Display {
  public:
    Display() {
      arm = 120;
      disarm = 60;
      armordisarm = false;
    }
    void start() {
      lcd.init();
      lcd.backlight();
    }
    void menu() {
      validtime = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1.Start");
      lcd.setCursor(0, 1);
      lcd.print("2.Options");
      lcd.setCursor(0,2);
      lcd.print("Arm:     " + convertback(arm));
      lcd.setCursor(0,3);
      lcd.print("Disarm:  " + convertback(disarm));
      stackcounter = 0;
      mytime2 = "000000";
      mytime = "";
      startcode = "";
      startcode2 = "----";
    }
    void startmenu(){
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("Enter Start Code");
      lcd.setCursor(8, 2);
      lcd.print(startcode2);
      lcd.setCursor(0, 3);
      lcd.print("*: <-");
      lcd.setCursor(12, 3);
      lcd.print("#: Enter");
    }
    void timeoption(){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1. Arm Time");
      lcd.setCursor(0, 1);
      lcd.print("2. Disarm Time");
      lcd.setCursor(0, 3);
      lcd.print("*: <-");
    }
    void timeset() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1.  1:00");
      lcd.setCursor(10, 0);
      lcd.print("2.  5:00");
      lcd.setCursor(0, 1);
      lcd.print("3. 10:00");
      lcd.setCursor(10, 1);
      lcd.print("4. 15:00");
      lcd.setCursor(0, 2);
      lcd.print("5: Custom");
      lcd.setCursor(0, 3);
      lcd.print("*: <-");
      
    }

    void customtime() {
      lcd.clear();
      if(!validtime){
        lcd.setCursor(0,0);
        lcd.print("INVALID INPUT");
        validtime = true;
      }
      lcd.setCursor(0, 1);
      lcd.print("Max:  01:00:00");
      lcd.setCursor(6, 2);
      lcd.print(String(mytime2[0]) + String(mytime2[1]) + ":" + String(mytime2[2]) + String(mytime2[3])+ ":" + String(mytime2[4])+String(mytime2[5]));
      lcd.setCursor(0, 3);
      lcd.print("*: <-");
      lcd.setCursor(13, 3);
      lcd.print("#: Save");
      
      if(toblink){
        lcd.setCursor(blinkcounter, 2);
        lcd.blink();
      }
    }
    void confirm() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Confirm?");
      lcd.setCursor(0, 1);
      lcd.print("1.Yes");
      lcd.setCursor(0, 2);
      lcd.print("2.No");
      
    }
    void menuInput(char key, bool&st) {
      if (stackcounter == 0) {
        if (key == '1') {
          stackcounter--;
          startmenu();
        }
        else if (key == '2') {
          stackcounter++;
          timeoption();
        }
      }
      else if(stackcounter == -1){
        if(key == '*'){
          stackcounter++;
          menu();
        }
        else if(key == '#'){
          if(startcode == "0000"){
            st = true;
            stackcounter = 0;
          }
          else{
            startcode = "";
            startcode2 = "----";
            startmenu();
          }
        }
        else{
          if(startcode.length() < 4){
            startcode+=key;
            startcode2 = startcode;
            for(int i = startcode.length(); i < 4; i++){
              startcode2+="-";
            }
          }
          startmenu();
        }
      }
      else if(stackcounter == 1){
        if (key == '1'){
          armordisarm = true;
          stackcounter++;
          timeset();
        }
        else if(key == '2'){
          armordisarm = false;
          stackcounter++;
          timeset();
        }
        else if(key == '*'){
          stackcounter--;
          menu();
        }
      }
      else if (stackcounter == 2) {
        if(armordisarm)
          gametimer = &arm;
        else
          gametimer = &disarm;
        if (key == '1'){
          *gametimer = 60;
          stackcounter--;
          menu();
        }
        else if (key == '2'){
          *gametimer = 300;
          stackcounter--;
          menu();
        }
        else if (key == '3'){
          *gametimer = 600;
          stackcounter--;
          menu();
        }
        else if (key == '4'){
          *gametimer = 900;
          stackcounter--;
          menu();
        }
        else if (key == '5') {
          lcd.noBlink();
          stackcounter++;
          mytime2 = "000000";
          mytime = "";
          blinkcounter = 13;
          toblink = true;
          customtime();
        }
        else if (key == '*') {
          stackcounter--;
          timeoption();
        }
      }
      else if (stackcounter == 3) {
        if (key == '#') {
          lcd.noBlink();
          stackcounter++;
          confirm();
        }
        else if (key == '*') {
          lcd.noBlink();
          stackcounter--;
          timeset();
        }
        else{
          toblink = true;
          if(mytime.length() == 0 && key == '0'){
            mytime ="";
          }
          else{
            mytime+=key;
            mytime2 = mytime;
            for(int i = 5; i > i - mytime.length(); i--){
              mytime2 = "0" + mytime2;
            }
            if(blinkcounter > 6){
              blinkcounter--;
              if(blinkcounter == 11 || blinkcounter == 8)
                blinkcounter--;
            }
            else{
              toblink = false;
              lcd.noBlink();
            }
          }
          customtime();
        }
      }
      else if (stackcounter == 4) {
        if (key == '1')
          calculatetime();
        else if (key == '2') {
          stackcounter--;
          mytime2 = "000000";
          mytime = "";
          blinkcounter = 13;
          toblink = true;
          customtime();
        }
      }
    }
    void calculatetime() {
       if(mytime.length() == 0){
          *gametimer = 0;
          menu();
       }
       else{
          mytime2 = mytime2.substring(0, 6);
          unsigned long temp = mytime2.toInt();
          unsigned long timelimit = temp;
          unsigned long seconds = temp % 100;
          temp /= 100;
          unsigned long  minutes = temp %100;
          temp /= 100;
          unsigned long hours = temp%100;
          if(seconds < 60 && minutes < 60 && hours <=HOUR_LIMIT && timelimit<= TIME_LIMIT){
            *gametimer = (seconds) + (minutes * 60) + (hours * 3600);
            menu();
          }
          else{
             mytime2 = "000000";
             mytime = "";
             validtime = false;
             blinkcounter = 13;
             toblink = true;
             stackcounter--;
             customtime();
          }
       }
    }
    String convertback(unsigned long a){
      unsigned long thetime = a;
      int hours = thetime/3600;
      thetime = thetime % 3600;
      int minutes = thetime/60;
      thetime = thetime % 60;
      int seconds = thetime;
      String h, m, s;
      h = hours < 10 ? "0" + String(hours): String(hours);
      m = minutes < 10 ? "0" + String(minutes): String(minutes);
      s = seconds < 10 ? "0" + String(seconds): String(seconds);
      return h+":"+m+":"+s;
    }
    void anothermenu(unsigned long s, unsigned long m, unsigned long h, unsigned long x){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(String(s)+":"+String(m)+":"+String(h));
      lcd.setCursor(0,1);
      lcd.print(arm);
      lcd.setCursor(0,2);
      lcd.print(x);
      lcd.setCursor(0,3);
      lcd.print(mytime2);
    }
    unsigned long get_arm() {
      return arm;
    }
    unsigned long get_disarm(){
      return disarm;
    }
  private:
    int stackcounter;
    unsigned long* gametimer;
    unsigned long arm;
    unsigned long disarm;
    String mytime;
    String mytime2;
    String mytime3;
    String mytime4;
    String disarmtime;
    String disarmtime2;
    int blinkcounter; 
    bool validtime;
    bool toblink;
    String startcode;
    String startcode2;
    bool armordisarm;
};


#endif
