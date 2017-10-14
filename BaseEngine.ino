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


TSPoint match_grid(TSPoint ot){
  ot.x = (x / (standardMapRes * 8)) * (standardMapRes * 8);
  ot.y = (y / (standardMapRes * 8)) * (standardMapRES * 8);

  return ot;
}

void pointa-pointb(TSPoint b){
  (while player_x != b.x){
      if (player_x < b.x){
        player_x = player_x + (standardMapRes * 8);
      }
      else {
        player_x = player_x - (standardMapRes * 8);
      }
      draw_character(player_x, player_y);
  }
  (while player_y != b.y){
     if (player_y < b.y){
        player_y = player_y + (standardMapRes * 8);
      }
      else {
        player_y = player_y - (standardMapRes *8);
      }
      draw_character(player_x, player_y);
  }
}

