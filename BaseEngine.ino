/*  
 *  BaseEngine.ino - responsible for running the actual GAME. When playing
 *  or playtesting the game, this OS State will be used.
*/


void drawEngine(void){
  tft.fillScreen(GREEN);
  //drawSprite(50, 50, 0);
  
}


void runEngine(void){
  for (int i = 0; i < 5; i++){
    drawSprite(i * 48, 50, 0);
    delay(500);
    tft.fillRect(i * 48, 50, 48, 48, GREEN);
  }
  
}

