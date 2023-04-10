#define USE_ARDUINO_INTERRUPTS true //--> Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h> //--> Includes the PulseSensorPlayground Library. 
#include <LiquidCrystal.h> //--> Includes the LiquidCrystal Library.

LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //--> Initialize LiquidCrystal with "lcd". lcd(RS,E,D4,D5,D6,D7).

//----------------------------------------Variable Declaration
const int PulseWire = A5; //--> PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED_3 = 13; //--> LED to detect when the heart is beating. The LED is connected to PIN 3 on the Arduino UNO.
int Threshold = 550; //--> Determine which Signal to "count as a beat" and which to ignore.
                     //--> Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                     //--> Otherwise leave the default "550" value. 
//----------------------------------------

//----------------------------------------Draw "Heart" on LCD.
/*
                     heart4 heart5
                       ===   ===
                      ‖   ‖ ‖   ‖
              = 00011 11000 00011 11000 =                      11 11       11 11   
             ‖  00111 11100 00111 11100  ‖                    111 111     111 111  
             ‖  01111 11110 01111 11110  ‖                   1111 1111   1111 1111 
             ‖  11111 11111 11111 11111  ‖                  11111 11111 11111 11111
      heart3 ‖  11111 11111 11111 11111  ‖ heart6           11111 11111 11111 11111
             ‖  11111 11111 11111 11111  ‖                  11111 11111 11111 11111
             ‖  11111 11111 11111 11111  ‖                  11111 11111 11111 11111
              = 01111 11111 11111 11110 =                    1111 11111 11111 1111 
                                               ------->
              = 00011 11111 11111 11000 =                      11 11111 11111 11   
             ‖  00001 11111 11111 10000  ‖                      1 11111 11111 1    
             ‖  00000 11111 11111 00000  ‖                        11111 11111      
      heart2 ‖  00000 11111 11111 00000  ‖ heart7                 11111 11111      
             ‖  00000 01111 11110 00000  ‖                         1111 1111       
             ‖  00000 00111 11100 00000  ‖                          111 111        
             ‖  00000 00011 11000 00000  ‖                           11 11         
              = 00000 00001 10000 00000 =                             1 1           
                      ‖   ‖ ‖   ‖
                       ===   ===
                     heart1 heart8

*/

byte heart1[8] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};
byte heart2[8] = {B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart3[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B01111};
byte heart4[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11111};
byte heart5[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B11111};
byte heart6[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11110};
byte heart7[8] = {B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart8[8] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};
//----------------------------------------

int Instructions_view = 500; //--> Variable for waiting time to display instructions on LCD.
                               
PulseSensorPlayground pulseSensor; //--> Creates an instance of the PulseSensorPlayground object called "pulseSensor"

//--------------------------------------------------------------------------------void setup
void setup() {
     
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  Serial.begin(9600);//--> Set's up Serial Communication at certain speed.
  lcd.begin(16, 2); //--> Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  
  //----------------------------------------Create a custom character (glyph) for use on the LCD
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, heart7);
  lcd.createChar(8, heart8);
  //----------------------------------------
  
  lcd.setCursor(0,0);
  lcd.print(" HeartBeat Rate ");
  lcd.setCursor(0,1);
  lcd.print("   Monitoring   ");

  //----------------------------------------Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED_3); //--> auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  //----------------------------------------

  //----------------------------------------Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); //--> This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  //----------------------------------------

  delay(2000);
  lcd.clear();
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------void loop
void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute(); //--> Calls function on our pulseSensor object that returns BPM as an "int". "myBPM" hold this BPM value now.

  //----------------------------------------Condition if the Sensor does not detect the heart rate / the sensor is not touched.
  if (Instructions_view < 500) {
    Instructions_view++;
  }
  
  if (Instructions_view > 499) {
    lcd.setCursor(0,0);
    lcd.print("Please Wear Belt");
    lcd.setCursor(0,1);
    lcd.print("Clung to Skin"); 
    delay(1000);
    lcd.clear();
    delay(500);
  }
  //----------------------------------------

  //----------------------------------------Constantly test to see if "a beat happened". 
  if (pulseSensor.sawStartOfBeat()) { //--> If test is "true", then the following conditions will be executed.
    Serial.println("♥  A HeartBeat Happened ! "); //--> Print a message "a heartbeat happened".
    Serial.print("BPM: "); //--> Print phrase "BPM: " 
    Serial.println(myBPM); //--> Print the value inside of myBPM. 

    //----------------------------------------Displays a "Heart" shape on the LCD.
    lcd.setCursor(1,1);
    lcd.write(byte(1));
    lcd.setCursor(0,1);
    lcd.write(byte(2));
    lcd.setCursor(0,0);
    lcd.write(byte(3));
    lcd.setCursor(1,0);
    lcd.write(byte(4));
    lcd.setCursor(2,0);
    lcd.write(byte(5));
    lcd.setCursor(3,0);
    lcd.write(byte(6));
    lcd.setCursor(3,1);
    lcd.write(byte(7));
    lcd.setCursor(2,1);
    lcd.write(byte(8));
    //----------------------------------------

    //----------------------------------------Displays the BPM value on the LCD.
    lcd.setCursor(5,0);
    lcd.print("Heart Rate");
    lcd.setCursor(5,1);
    lcd.print(": ");
    lcd.print(myBPM);
    lcd.print(" ");
    lcd.print("BPM     ");
    
    Instructions_view = 0; 
  }
  //----------------------------------------
  
  delay(20);
    
    if(myBPM >60)
    {
      digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    
      }
      if(myBPM <=60)
    {
      digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    }
    //----------------------------------------
     //--> considered best practice in a simple sketch.
}
