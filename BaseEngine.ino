/*  
 *  BaseEngine.ino - responsible for running the actual GAME. When playing
 *  or playtesting the game, this OS State will be used.
*/

int player_x;
int player_y;
bool is_running;

void drawEngine(void){
  tft.fillScreen(GREEN);
  //drawSprite(50, 50, 0);
  //drawMap(0, 0, 0, standardMapRes);
  
}


void runEngine(void){
  is_running = true;
  while(is_running) {
    delay(50);
    move_character();

  }
  
}

void move_character(void) 
{
  TSPoint p = getTouchPoint();
  if (isNewTouch() and p.x != player_x and p.y != player_y) {
    tft.fillRect(player_x, player_y, 40, 40, GREEN);
    draw_character(p.x, p.y);
  }

}

void draw_character(int x, int y) 
{
  int corn_y = (y / (8 * standardMapRes)) *(8 * standardMapRes);
  int corn_x = (x / (8 * standardMapRes)) *(8 * standardMapRes);
  drawSpriteWithRes(corn_x, corn_y, 0, standardMapRes);
  player_x = corn_x;
  player_y = corn_y;
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


