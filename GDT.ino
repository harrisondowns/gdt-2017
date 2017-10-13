/*  
 *  GDT.ino - this file is the core of the program. It contains functions and variables
 *  pertaining to Backbone, the underlying graphics and OS functions that run the program 
 *  itself. Variables and functionsthat need to passed or used between program states must 
 *  be declared here in order to be used.
*/

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // touch screen library

using namespace std;

/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                                   UI Vars                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/

// COLORS
#define  BLACK   0x0000
#define LIGHTGRAY 0xCE79
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define DARKGRAY 0x9CD3
#define LIGHTBLUE 0xAD5F
#define ORANGE 0xECA0
#define DARKGREEN 0x0B80
#define BROWN 0x7260
#define PALE 0xE637
#define GOLD 0xD566

#define BOXSIZE 30
#define spriteTileW 48

/* button struct */
struct Button {
  int x;
  int y;
  int w;
  int h;
  unsigned backgroundColor;
  unsigned frameColor;
  unsigned textColor;
  char* text;
  void (*callFunc)(int par);
  int p;
};

unsigned colors[16] = {BLACK, DARKGRAY, RED, YELLOW, GOLD, GREEN, CYAN, MAGENTA, 
                       WHITE, LIGHTGRAY, PALE, ORANGE, BROWN, DARKGREEN, BLUE, LIGHTBLUE};
#define numSprites 15
byte sprites[numSprites][8][8];


/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                     LCD Touch Screen vars                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/
// pin bullshit
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define MINPRESSURE 10
#define MAXPRESSURE 1000


#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);




/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                        Backbone / OS Vars                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/
// Os state variables
#define PUSH 0
#define POP 1

/* OS STATES */
#define SPRITE_MAKER 0 
#define SPRITE_MANAGER 1 // screen that shows each sprite that can be selected
#define BASE_ENGINE 2    // base game play
#define KEYBOARD_INPUT 3

// dynamic array of OSState used in popState() and pushToState()
vector<int>* programStack;

// collection of buttons used in drawing
vector<Button*>* buttons;

// osState: current screen of the OS
int osState = KEYBOARD_INPUT;//SPRITE_MANAGER;

// functions that draw each OS State
void (*drawFuncs[])(void) = {&drawSpriteMaker, 
                             &drawSpriteManager, 
                             &drawEngine,
                             &drawKeyboard};

// functions that get called every loop for each OS State
void (*runFuncs[])(void) = {&runSpriteMaker, 
                            &runSpriteManager, 
                            &runEngine,
                            &runKeyboard};

/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                Shared Vars Between States                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/

// state variable passed between Sprite Manager and Sprite Maker
int currentSprite = 0;

// keyboard string variable.
char keyboardS[100];
int keyboardSL = 0;





/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                       Backbone / OS Funcs                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/


/* getTouchPoint() - returns a TSPoint variable representing the point 
 *                   that is being touched. X and Y represent coordinates
 *                   on the screen. Z represents touch strength. If Z = 500,
 *                   then the touch is valid / strong enough.
 *                   Example use:
 *                   void foo(){
 *                    TSPoint p = getTouchPoint();
 *                    if (p.z > 500){
 *                      Serial.print("x is ");
 *                      Serial.print(p.x);
 *                      Serial.print(" and y is ");
 *                      Serial.print(p.y);
 *                      Serial.print("\n");
 *                    }
 *                    else{
 *                      Serial.println("Not pressing hard enough!");
 *                    }
 *                   }
  */
TSPoint getTouchPoint(){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    int x = p.x;
    int y = p.y;

    // rotates touch position to match orientation of screen
    p.x = map(y, TS_MINY, TS_MAXY, tft.width(), 0);
    p.y = map(x, TS_MINX, TS_MAXX, tft.height(), 0);

    // set p to 500 if its pressed enough (500 is the signal code)
    p.z = 500;
    
  }
  return p;
}

/* setup() - Arduino's required "setup()" function that initializes all necessary
 *           OS Variables, as well as the touch screen and the graphics library.
  */
void setup(void) {

  programStack = new vector<int>();
  buttons = new vector<Button*>();

  Serial.begin(9600);


#ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    identifier=0x9328;
  
  }
  tft.begin(identifier);
  tft.setRotation(1);

  pushToState(osState);
 
}

/* loop() - Arduino's required "loop()" function that actually runs the project. Calls
 *          a corresponding run function depending on the current state of the OS. No
 *          run functions are allowed to block the OS, i.e. no infinite loops inside.
  */
void loop(void) {

  runFuncs[osState]();
 
} 


/* pushToState() - OS Function that pushes to a new state. state corresponds to an index in the drawFuncs and
 *          runFuncs array. Each OS State has a draw function and a run function associated with it and
 *          should be #defined at the top of this file under "OS States".
 *      Inputs:
 *          state - the index of the OS State to push to.
  */
void pushToState(int state){
  clearButtons();
  programStack->push_back(state);
  drawFuncs[state]();
  osState = state;
}

/* popState() - OS Function that pops back to the previous state. 
 *      Inputs:
 *          rip - this variable is not used but is part of the header in order to 
 *                unify the function headers for popState and pushToState
  */
void popState(int rip){
  programStack->pop_back();
  int state = programStack->back();
  pushToState(state);
}

/* 
 *  #########################################################################
 *  ###                                                                   ###
 *  ###                              UI Functions                         ###
 *  ###                                                                   ###
 *  #########################################################################
*/


/* makeButton() - UI Function that creates a button struct with a given set of variables. DOES NOT
 *                DRAW THE ACTUAL BUTTON. To draw a button, see "drawButton()" down below.
 *          Inputs:
 *                x - the x coordinate to draw at. Top left corner is 0,0.
 *                y - the y coordinate to draw at. Top left corner is 0,0.
 *                w - the width of the button in pixels.
 *                h - the height of the button in pixels.
 *                background - the color of the background for the button.
 *                frame - the color of the frame around the button.
 *                textColor - the color of whatever text is on the button.
 *                text - the string to display over the button.
 *                callFunc - the function to call when the button is called.
 *                par - the variable to pass in to callFunc when it gets called.
  */
Button* makeButton(int x, int y, int w, int h, unsigned background, unsigned frame, unsigned textColor, char* text, void (*callFunc)(int x), int par){
  Button *newBut = malloc(sizeof(Button));
  newBut->x = x;
  newBut->y = y;
  newBut->w = w;
  newBut->h = h;
  newBut->backgroundColor = background;
  newBut->frameColor = frame;
  newBut->text = text;
  newBut->textColor = textColor;
  newBut->callFunc = callFunc;
  newBut->p = par;
  buttons->push_back(newBut);
  return newBut;
}

/* clearButtons() - Clears the buttons array and frees all memory. Called on pushToState and popState.
  */
void clearButtons(){
  for (int i = 0; i < buttons->size(); i++){
    Button *b = buttons->at(i);
    delete b;
  }
  buttons->clear();
}


/* drawButton() - draws a given button.
 *          Inputs:
 *                but - the button struct to draw. Must be created using makeButton() or manually.
  */
void drawButton(Button* but){
  tft.fillRect(but->x, but->y, but->w, but->h, but->backgroundColor);
  tft.drawRect(but->x - 1, but->y - 1, but->w + 2, but->h + 2, but->frameColor);
  tft.setCursor(but->x + 3, but->y + 3);
  tft.setTextColor(but->textColor);
  tft.setTextSize(1);
  tft.println(but->text);
}

void drawText(int x, int y, int size, char *text, unsigned color){
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.println(text);
}

/* drawSprite() - draws a sprite at a given set of coordinates.
 *          Inputs:
 *                x - the x coordinate to draw at. Top left corner is 0,0.
 *                y - the y coordinate to draw at. Top left corner is 0,0.
  */
void drawSprite(int x, int y, int spriteN){
  int squareP = spriteTileW / 8;
  for (int j = 0; j < 8; j++){
    for (int i = 0; i < 8; i++){
      tft.fillRect(x + i * squareP, y + j * squareP, squareP, squareP, colors[sprites[spriteN][i][j]]);
    }
  }
}

