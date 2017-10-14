/*  
 *  SpriteManager.ino - runs the sprite manager, the screen that allows a user
 *  to select a specific sprite to edit using the SpriteMaker.
*/

// UI Vars
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
      drawSprite(x, y, i);

  }
  
}

void clickSprite(int i){
  currentSprite = i;
  pushToState(SPRITE_MAKER);
  
}

void runSpriteManager(void){
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
    if (a == 0){
     // setKeyboardMaxLength(40);
     // pushToState(KEYBOARD_INPUT);
     pushToState(BASE_ENGINE);
    }
 
  }
  
}


