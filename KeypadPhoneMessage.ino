

//code rearranged by Javier Muñoz 10/11/2016 ask me at javimusama@hotmail.co

#include <Keypad.h> 

// Uncomment SoftwareSerial for Arduino Uno or Nano.

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
//#define ARDUINO_RX 5  //should connect to TX of the Serial MP3 Player module
//#define ARDUINO_TX 6  //connect to RX of the module


//SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
// Uncomment next line if you are using an Arduino Mega.
#define mp3 Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;           // Answer from the MP3.


String sanswer(void);
String sbyte2hex(uint8_t b);


/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16  // Stop playing continuously. 
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

/************ Opitons **************************/
#define DEV_TF            0X02


const byte numRows= 4; //number of rows on the keypad
const byte numCols= 3; //number of columns on the keypad
int code = 1897;  //The code I used, you can change it
//int code = 2356;  //The code I used, you can change it
int tot,i1,i2,i3,i4;
char c1,c2,c3,c4;
//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3'}, 
{'4', '5', '6'}, 
{'7', '8', '9'},
{'*', '0', '#'}

};
//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {5,6,7,8}; //Rows 0 to 3
byte colPins[numCols]= {2,3,4}; //Columns 0 to 2/

int relay_pin = 10;
//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
void setup()
           {
           Serial.begin(9600);            
           lcd.begin(16,2);         // initialize the lcd for 20 chars 4 lines, turn on backlight
           // ------- Quick 3 blinks of backlight  -------------
         /* for(int i = 0; i< 2; i++)
          {
            lcd.backlight();
            delay(250);
            lcd.noBacklight();
            delay(250);            
          }*/
          lcd.backlight(); // finish with backlight on  
          lcd.clear();
          lcd.setCursor(0,0); //Start at character 4 on line 0
          lcd.print("Press *");
          lcd.setCursor(0,1);
          lcd.print("4 Digit code");
               
          mp3.begin(9600);
          delay(100);
          //sendCommand(CMD_SEL_DEV, 0, DEV_TF);
          sendCommand(CMD_SEL_DEV, DEV_TF);
          delay(100);
          sendCommand(CMD_FOLDER_CYCLE, 0x0301);
          //sendCommand(CMD_PLAY);
}

           
void loop()
{            
          char keypressed = myKeypad.getKey();  //The getKey fucntion keeps the program runing, as long you didn't press "*" the whole thing bellow wouldn't be triggered
         //Serial.println(keypressed);
         if (keypressed == '*')             // and you can use the rest of you're code simply
             {      //when the "*" key is pressed you can enter the passcode

                    lcd.clear();
                    lcd.setCursor(0,0); //Start at character 4 on line 0
                    lcd.print("Code:");
                    lcd.setCursor(0,1);
                    keypressed = myKeypad.waitForKey();  // here all programs are stopped until you enter the four digits then it gets compared to the code above
                    if (keypressed != NO_KEY)
                      {
                       c1 = keypressed;
                        Serial.println(keypressed);
                        lcd.print("*");
                       }
                    keypressed = myKeypad.waitForKey();
                    if (keypressed != NO_KEY)
                      {
                       c2 = keypressed;
                       Serial.println(keypressed);
                       lcd.print("*");
                       }
                     keypressed = myKeypad.waitForKey();
                   if (keypressed != NO_KEY)
                      {
                       c3 = keypressed;
                        Serial.println(keypressed);
                        lcd.print("*");
                       }
                      keypressed = myKeypad.waitForKey();
                   if (keypressed != NO_KEY)
                      {
                       c4 = keypressed;
                       Serial.println(keypressed);
                       lcd.print("*");
                       }                                   
                     
                     i1=(c1-48)*1000;
                     i2=(c2-48)*100;
                     i3=(c3-48)*10;
                     i4=c4-48;                    
                         
                     tot=i1+i2+i3+i4;
         Serial.println(tot);
         Serial.println(code);
        
         if (tot == code) //if the code is correct you trigger whatever you want here it just print a message on the screen
         { 
          sendCommand(CMD_FOLDER_CYCLE, 0x0101);
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
         }
         else 
         {             
          sendCommand(CMD_FOLDER_CYCLE, 0x0201);
          lcd.clear();
          lcd.setCursor(0,0); //Start at character 4 on line 0
          lcd.print("Sorry wrong code");
          lcd.setCursor(0,1);
          lcd.print("Hangup,try again");
          delay(3000);
          
         }
           
     } 
}




/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer += " -> Error";
      break;

    case 0x41:
      decodedMP3Answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;

    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;

    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;

    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;

    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
  }

  return decodedMP3Answer;
}






/********************************************************************************/
/*Function: Send command to the MP3                                             */
/*Parameter: byte command                                                       */
/*Parameter: byte dat1 parameter for the command                                */
/*Parameter: byte dat2 parameter for the command                                */

/*void sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat1, byte dat2){
  delay(20);
  Send_buf[0] = 0x7E;    //
  Send_buf[1] = 0xFF;    //
  Send_buf[2] = 0x06;    // Len
  Send_buf[3] = command; //
  Send_buf[4] = 0x01;    // 0x00 NO, 0x01 feedback
  Send_buf[5] = dat1;    // datah
  Send_buf[6] = dat2;    // datal
  Send_buf[7] = 0xEF;    //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}*/


void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e;   //
  Send_buf[1] = 0xff;   //
  Send_buf[2] = 0x06;   // Len
  Send_buf[3] = command;//
  Send_buf[4] = 0x01;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;   //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}



/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                       */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}


/********************************************************************************/
/*Function: shex2int. Returns a int from an HEX string.                         */
/*Parameter: s. char *s to convert to HEX.                                      */
/*Parameter: n. char *s' length.                                                */
/*Return: int                                                                   */

int shex2int(char *s, int n){
  int r = 0;
  for (int i=0; i<n; i++){
     if(s[i]>='0' && s[i]<='9'){
      r *= 16; 
      r +=s[i]-'0';
     }else if(s[i]>='A' && s[i]<='F'){
      r *= 16;
      r += (s[i] - 'A') + 10;
     }
  }
  return r;
}


/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

String sanswer(void)
{
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10))
  {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
  {
    return mp3answer;
  }

  return "???: " + mp3answer;
}
