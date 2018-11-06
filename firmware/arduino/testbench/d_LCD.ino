#include <TimerThree.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(53, 51, 49, 47, 45, 43);

void lcdSetup(){
  Timer3.initialize(150000);
  Timer3.attachInterrupt(updateDisplay);
  lcd.begin(16, 2);
  lcd.print("Tension (lbs)");
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print(tension,5);
}
