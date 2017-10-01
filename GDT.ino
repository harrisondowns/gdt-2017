#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>


#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // touch screen library

using namespace std;

#define PUSH 0
#define POP 1

// pin bullshit
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define sbX 110

#define sbY 40

#define sbS 24
#define sbMX (sbX + (8 * sbS))
#define sbMY (sbY + (8 * sbS))

// Assign human-readable names to some common 16-bit color values:
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

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int oldcolor, currentcolor;

#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

/* OS STATES */
#define SPRITE_MAKER 0
#define SPRITE_MANAGER 1
#define BASE_ENGINE 2

/* UI Balogna*/
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

#define spriteTileW 48

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

vector<int>* programStack;
vector<Button*>* buttons;
int osState = SPRITE_MANAGER;
int currentSprite = 0;
int colorInd = 0;


void (*drawFuncs[])(void) = {&drawSpriteMaker, &drawSpriteManager, &drawEngine};

void (*runFuncs[])(void) = {&runSpriteMaker, &runSpriteManager, &runEngine};



unsigned colors[16] = {BLACK, DARKGRAY, RED, YELLOW, GOLD, GREEN, CYAN, MAGENTA, 
                       WHITE, LIGHTGRAY, PALE, ORANGE, BROWN, DARKGREEN, BLUE, LIGHTBLUE};
#define numSprites 15
byte sprites[numSprites][8][8];
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
    
    p.z = 500;
    
  }
  return p;
}


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

void loop(void) {

  runFuncs[osState]();
 
} 

void pushToState(int state){
  clearButtons();
  programStack->push_back(state);
  drawFuncs[state]();
  osState = state;
}

/* note: int rip is there to unify the function headers for the button struct*/
void popState(int rip){
  programStack->pop_back();
  int state = programStack->back();
  pushToState(state);
}

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

void clearButtons(){
  for (int i = 0; i < buttons->size(); i++){
    Button *b = buttons->at(i);
    delete b;
  }
  buttons->clear();
}

void drawButton(Button* but){
  tft.fillRect(but->x, but->y, but->w, but->h, but->backgroundColor);
  tft.drawRect(but->x - 1, but->y - 1, but->w + 2, but->h + 2, but->frameColor);
  tft.setCursor(but->x + 3, but->y + 3);
  tft.setTextColor(but->textColor);
  tft.setTextSize(1);
  tft.println(but->text);
}
void drawSprite(int x, int y, int spriteN){
  int squareP = spriteTileW / 8;
  for (int j = 0; j < 8; j++){
    for (int i = 0; i < 8; i++){
      tft.fillRect(x + i * squareP, y + j * squareP, squareP, squareP, colors[sprites[spriteN][i][j]]);
    }
  }
}

