/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 Connect Vcc and Ground, SDA to A4, SCL to A5 on Arduino
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


/*-----( Declare Variables )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // Used to type in characters

  //lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.begin(16,2);         // initialize the lcd for 20 chars 4 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
    
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
  

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
      // NOTE: Cursor Position: Lines and Characters start at 0  
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Next clue:");
      lcd.setCursor(0,1);
      lcd.print("At Station 7");
      delay(3000);
      
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Ask a Librarian:");
      lcd.setCursor(0,1);
      lcd.print("...");
      delay(3000);

      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("What unique");
      lcd.setCursor(0,1);
      lcd.print("performances");
      delay(3000);

      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("happened on");
      lcd.setCursor(0,1);
      lcd.print("campus in 1974?");
      delay(3000);
      
      
 

  
  /*{
    // when characters arrive over the serial port...
    if (Serial.available()) {
      // wait a bit for the entire message to arrive
      delay(100);
      // clear the screen
      lcd.clear();
      // read all the available characters
      while (Serial.available() > 0) {
        // display each character to the LCD
        lcd.write(Serial.read());
      }
    }
  }*/

}/* --(end main loop )-- */


/* ( THE END ) */

