// map select screen for tile map changing or transfer events

#define spriteSpacingW 64
#define spriteTileBaseX 8
#define spriteTileBaseY 60


void drawMapSelect(void){
  
  tft.fillScreen(LIGHTGRAY);
  tft.setCursor(60, 5);
  tft.setTextColor(BLACK);  
  tft.setTextSize(3);
  tft.println("Map Select");

  for (int i = 0; i < 4; i++){

      int x = 60 + (90 * (i % 2));
      int y = 60 + (70 * (i / 2));
      int w = 64;
      int h = 48;
 
     // tft.fillRect(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, colors[sprites[currentSprite][x][y]]);
      Button *b = makeButton( x, y, w, h, TRANSPARENT, WHITE, BLACK, "", map_select, i);
      drawButton(b);
      fillRect(x, y, w, h, currentBackground);
      drawMap(x, y, i, 1);

  }
  delay(1);
  drawButton(makeButton(5, 5, 40, 40, BLUE, WHITE, WHITE, "EXIT", popState, 0));
}

void map_select(int i){
  currentMap = i;
  popState(0);
  
}




void runMapSelect(void){
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
