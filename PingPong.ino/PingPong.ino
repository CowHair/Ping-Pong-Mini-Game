#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen Variables
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long gameTime;
unsigned long minute;
unsigned long sec;

volatile bool pause = false;
unsigned long pause_start = 0;
unsigned long pause_time = 0;
unsigned long current;

// Score tracking
int scoreP1 = 0;
int scoreP2 = 0;

const int PADDLE_WIDTH = 2;
const int PADDLE_HEIGHT = 12;

// Position tracking for player 1 paddle
int P1_X =0;
int P1_Y = 32;
// Position tracking for player 2 paddle
int P2_X = 128-PADDLE_WIDTH;
int P2_Y = 32;
// Position tracking for ball
int ballX = 64;
int ballY = 32;

const int BTN_P1 = 4;
const int BTN_P2 = 3;
bool lastBtnP1 = HIGH;
bool lastBtnP2 = HIGH;



// This code will wait for a button press to turn the built in LED off for a short duration
int BTN_PAUSE = 2;
void buttonInterrupt(){
    // This is the code that will be executed after triggering the interrupt
    pause = !pause;
}

void setup() {
    Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);


  pinMode(BTN_P1, INPUT_PULLUP);
  pinMode(BTN_P2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  // Declares our button1Pin as an input
  // INPUT_PULLUP will by default set our pin to HIGH and will be set LOW when the button is pressed
  pinMode(BTN_PAUSE, INPUT_PULLUP);

  // INPUT_PULLDOWN will by default set the pin low when the button is not pressed, and high when it is

  attachInterrupt(digitalPinToInterrupt(BTN_PAUSE), buttonInterrupt, FALLING);
  

  /* attachInterrupt takes 3 values; digitalPinToInterrupt(pin) declares which pin to attach the interupt. 
  The 2nd value is the interrupt function which must return void. The final value is the interrupt configuration. 
  CHANGE will trigger the interrupt when the interruptPin changes state. 
  FALLING will trigger when the pin changes from 1 to 0 (We want to use this for this exercise as our buttons are active LOW). 
  RISING will trigger when the state changes from 0 to 1. */
}

void loop() {
  display.clearDisplay();
  current = millis();
  if (pause){
    if (pause_start == 0){
      pause_start = current;
    }
    return;
  } else{
      if (pause_start != 0){
        pause_time += current - pause_start;
        pause_start = 0;
      }
  }

  gameTime = current - pause_time;
  minute = gameTime/60000;
  sec = (gameTime / 1000) % 60;

  // Display must first be cleared 
  
  // Sets up text for the player scores
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.print("P1:");
  display.print(scoreP1);
  display.print("   ");
  display.print(minute);
  display.print(":");
  if (sec < 10) {
    display.print("0");
  }
  display.print(sec);
  display.print("  P2:");
  display.print(scoreP2);
  display.setCursor(40,10);

  // Set up paddles
  display.fillRect(P1_X, P1_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  display.fillRect(P2_X, P2_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  // Set up ball
  display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE);

  display.display();
  // Displays everything
  delay(15);


  bool btnP1 = digitalRead(BTN_P1);
  bool btnP2 = digitalRead(BTN_P2);
  // digitalRead() takes in the button input
  // LED_BUILT_IN is the LED built into every Arduino Board. It can also be accessed by digitalPin 13

  digitalWrite(LED_BUILTIN, HIGH);

  if (btnP1 == LOW || btnP2 == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  /* If the button states for either player change from Low to HIGH, 
  the paddle direction is flipped p1 and p2Dir need to be declared as const int before your setup. 
  Assign them a value to determine their speed. */ 

  /* If the state of the player1 button is high, the direction of the paddle is flipped. 
  lastBtnP1 tracks the state of the button on the previous loop of the code. 
  This is called edge detection and detects the change of the button state. */


  


}