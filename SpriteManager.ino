



#define spriteSpacingW 64
#define spriteTileBaseX 8
#define spriteTileBaseY 60







void drawSpriteManager(void){
  
  tft.fillScreen(LIGHTGRAY);
  tft.setCursor(40, 5);
  tft.setTextColor(BLACK);  
  tft.setTextSize(3);
  tft.println("Sprite Manager");

  for (int i = 0; i < numSprites; i++){
      int x = (i % 5) * spriteSpacingW + spriteTileBaseX - 1;
      int y = (i / 5) * spriteSpacingW + spriteTileBaseY - 1;
      int w = spriteTileW;
      int h = spriteTileW;
      
     // tft.fillRect(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, colors[sprites[currentSprite][x][y]]);
      Button *b = makeButton(x, y, w, h, BLACK, WHITE, BLACK, sprintf("%d", i), clickSprite, i);
      drawButton(b);

  }
/*
  for (int i = 0; i < numSprites; i++){
   // tft.fillRect((i % 5) * spriteSpacingW + spriteTileBaseX, (i / 5) * spriteSpacingW + spriteTileBaseY, spriteTileW, spriteTileW, RED);
    tft.drawRect((i % 5) * spriteSpacingW + spriteTileBaseX - 1, (i / 5) * spriteSpacingW + spriteTileBaseY - 1, spriteTileW + 2, spriteTileW + 2, WHITE);

    drawSprite((i % 5) * spriteSpacingW + spriteTileBaseX, (i / 5) * spriteSpacingW + spriteTileBaseY, i);
    
    tft.setCursor((i % 5) * 64 + spriteTileBaseX + 15, (i / 5) * 64 + spriteTileBaseY + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println(i + 1);
  }*/
  
}
/*
void drawSprite(int x, int y, int spriteN){
  int squareP = spriteTileW / 8;
  for (int j = 0; j < 8; j++){
    for (int i = 0; i < 8; i++){
      tft.fillRect(x + i * squareP, y + j * squareP, squareP, squareP, colors[sprites[spriteN][i][j]]);
    }
  }
}*/
void clickSprite(int i){
  currentSprite = i;
  pushToState(SPRITE_MAKER);
  
}

void runSpriteManager(void){
  TSPoint p = getTouchPoint();
  if (p.z == 500){
   /* for (int i = 0; i < numSprites; i++){
      int x = (i % 5) * spriteSpacingW + 8;
      int y = (i / 5) * spriteSpacingW + 40;

      if (p.x >= x && p.x < x + spriteTileW && p.y >= y && p.y < y + spriteTileW){
        currentSprite = i;
        pushToState(SPRITE_MAKER);
        
        break;
      }
      else{
        pushToState(BASE_ENGINE);
        break;
      }*/
    for (int i = 0; i < buttons->size(); i++){
      Button *b = buttons->at(i);
      if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
      b->callFunc(b->p);
      break;
    }
    pushToState(BASE_ENGINE);
        
    
      
      
      //tft.fillRect((i % 5) * 64 + 8, (i / 5) * spriteTileW + 40, 48, 48, RED);
    }
    
    
  }
  
}


