// background color picker

#define spriteSpacingW 64
#define spriteTileBaseX 8
#define spriteTileBaseY 60

void drawBGColor(void){
  
  tft.fillScreen(LIGHTGRAY);
  tft.setCursor(60, 5);
  tft.setTextColor(BLACK);  
  tft.setTextSize(2);
  tft.println("Background Color");

  for (int i = 0; i < numSprites; i++){

      int x = (i % 5) * spriteSpacingW + spriteTileBaseX - 1;
      int y = (i / 5) * spriteSpacingW + spriteTileBaseY - 1;
      int w = spriteTileW;
      int h = spriteTileW;

    if (i == 10){
      Button *b = makeButton(x, y, w, h, colors[15], WHITE, BLACK, "", selectColor, 15);
      drawButton(b);
      continue;
    }
 
     // tft.fillRect(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, colors[sprites[currentSprite][x][y]]);
      Button *b = makeButton(x, y, w, h, colors[i], WHITE, BLACK, "", selectColor, i);
      drawButton(b);
//      drawSprite(x, y, i);

  }
  delay(1);
  drawButton(makeButton(5, 5, 40, 40, BLUE, WHITE, WHITE, "EXIT", popState, 0));
}

void selectColor(int i){
  currentBackground = colors[i];
  popState(0);
  
}




void runBGColor(void){
  TSPoint p = getTouchPoint();
  if (p.z == 500){
    int a = 0;
    for (int i = 0; i < buttons->size(); i++){
      Button *b = buttons->at(i);
      if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
        b->callFunc(b->p);
        a = 1;
        break;
      }
    }
    /*if (a == 0){
     // setKeyboardMaxLength(40);
     // pushToState(KEYBOARD_INPUT);
     pushToState(BASE_MAKER);
    }*/
 
  }
  
}

