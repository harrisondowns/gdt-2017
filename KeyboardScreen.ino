
char *keys = "1234567890qwertyuiopasdfghjkl;zxcvbnm,./";



#define keyWidth 24
#define keySpace 5

void drawKeyboard(void){
 tft.fillScreen(LIGHTGRAY);
 for (int j = 0; j < 4; j++){
  for (int i = 0; i < 10; i++){
    int y = 120 + (keyWidth + keySpace) * j;
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
}

void keyPress(int num){
  Serial.print(keydown);
  if (keydown == 1){
    keydown = 0;
    keyboardS[keyboardSL] = keys[num];
    char arr[2];
    arr[0] = keys[num];
    arr[1] = '\0';
    drawText(keyboardSL * 12, 5, 1, arr, WHITE);
    keyboardSL++;
  }
  Serial.print(keydown);
}


void runKeyboard(void){
  TSPoint p = getTouchPoint();
  bool buttonPress = false;
  if (p.z == 500){
    Serial.print("p.x:");
    Serial.print(p.x);
    Serial.print("p.y: ");
    Serial.println(p.y);
    for (int i = 0; i < buttons->size(); i++){
      Button *b = buttons->at(i);
      if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
        b->callFunc(b->p);
        buttonPress = true;
        Serial.println("press");
        break;
      }
    }
    
  }
  Serial.println("Check");
  if (buttonPress == false){
     keydown = 1;
  }
  
  
}

