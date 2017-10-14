/*  
 *  BaseEngine.ino - responsible for running the actual GAME. When playing
 *  or playtesting the game, this OS State will be used.
*/


void drawEngine(void){
  tft.fillScreen(GREEN);
  //drawSprite(50, 50, 0);
  drawMap(0, 0, 0, standardMapRes);
  
}


void runEngine(void){
  for (int i = 0; i < 5; i++){
    delay(250);
    tft.fillRect(i * 48, 50, 48, 48, GREEN);
  }
  
}

void move_character(void) 
{
  TSPoint p = getTouchPoint();
  draw_character(x, y);
}

void draw_character(int x, int y)
