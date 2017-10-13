
const char *keys = "1234567890qwertyuiopasdfghjkl;zxcvbnm,./ ";
const char *extra = "!@#$%^&*()QWERTYUIOPASDFGHJKL:ZXCVBNM<>? ";

int kml;
int kStartX = 0;
int kStartY = 0;


#define keyWidth 24
#define keySpace 5

void setKeyboardMaxLength(int l){
  kml = l;
}

void drawKeyboard(void){
 tft.fillScreen(LIGHTGRAY);
 keyboardSL = 0;
 
 for (int j = 0; j < 4; j++){
  for (int i = 0; i < 10; i++){
    int y = 90 + (keyWidth + keySpace) * j;
    int x = i * (keyWidth + keySpace) + (j % 2) * 12 + 12;
    int w = keyWidth;
    int h = keyWidth;
    char arr[2];
    arr[0] = keys[j * 10 + i];
    arr[1] = '\0';
    Button *b = makeButton(x, y, w, h, DARKGRAY, WHITE, WHITE, arr, keyPress, j * 10 + i);
    drawButton(b);
    
  }
 }
 Button *b = makeButton(60, 208, 200, 24, DARKGRAY, WHITE, WHITE, "              Space\0", keyPress, 40);
 drawButton(b);
 Button *e = makeButton(270, 208, 40, 24, DARKGRAY, WHITE, WHITE, "Exit\0", exitKeyboard, 0);
 drawButton(e);

 int maxX = 25;
 if (kml < 25){
  maxX = kml;
 }
 kStartX = 160 - maxX * 6;
 kStartY = 10;

 
 fillRect(kStartX, kStartY, maxX * 12 + 3, ((kml / 25) + 1) * 20, BLACK);
 kStartX += 2;
 kStartY += 2;
}

void exitKeyboard(int rip){
  popState(0);
}

void keyPress(int num){
  if (isNewTouch() && keyboardSL < kml){
    keyboardS[keyboardSL] = keys[num];
    char arr[2];
    arr[0] = keys[num];
    arr[1] = '\0';
    drawText((keyboardSL % 25) * 12 + kStartX, (keyboardSL / 25) * 16 + kStartY, 2, arr, WHITE);
    keyboardSL++;
  }
}


void runKeyboard(void){
  TSPoint p = getTouchPoint();
  if (p.z == 500){
    
    for (int i = 0; i < buttons->size(); i++){
      Button *b = buttons->at(i);
      if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
        b->callFunc(b->p);
        break;
      }
    }
    
  }
}

