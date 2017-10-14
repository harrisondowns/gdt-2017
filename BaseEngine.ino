/*  
 *  BaseEngine.ino - responsible for running the actual GAME. When playing
 *  or playtesting the game, this OS State will be used.
*/

int player_x = 159;
int player_y = 119;
bool is_running;
bool moving = false;

void drawEngine(void){
  tft.fillScreen(GREEN);
  draw_character(player_x, player_y);
  //drawSprite(50, 50, 0);
  //drawMap(0, 0, 0, standardMapRes);
  
}


void runEngine(void){
  is_running = true;
    //delay(100);
  move_character();
  
}

void move_character(void) 
{
  TSPoint p = getTouchPoint();
  if (/*is_move(p.x, p.y) &&*/ p.z == 500) {
    Serial.print("FOUND NEW MOVE\n");
    tft.fillRect(player_x, player_y, 40, 40, GREEN);
    pointa_pointb(p);
  }

}

bool is_move(int x, int y) 
{
  if (x == 0 && y == 0) {
    return false;
  }
  int blockx1 = (x / (8 * standardMapRes)) *(8 * standardMapRes);
  int blocky1 = (y / (8 * standardMapRes)) *(8 * standardMapRes);
  int blockx2 = (player_x / (8 * standardMapRes)) *(8 * standardMapRes);
  int blocky2 = (player_y / (8 * standardMapRes)) *(8 * standardMapRes);

  if ((blockx1 != blockx2) || (blocky1 != blocky2)) {
    return true;
  }
  return false;
}

void draw_character(int x, int y) 
{
  int corn_y = (y / (8 * standardMapRes)) *(8 * standardMapRes);
  int corn_x = (x / (8 * standardMapRes)) *(8 * standardMapRes);
  drawSpriteWithRes(corn_x, corn_y, 0, standardMapRes);
  player_x = corn_x;
  player_y = corn_y;
}




/*TSPoint match_grid(TSPoint ot){
  ot.x = (x / (standardMapRes * 8)) * (standardMapRes * 8);
  ot.y = (y / (standardMapRes * 8)) * (standardMapRES * 8);
>>>>>>> Stashed changes

  return ot;
}*/

void movement(){
  if(moving){
    if(TimeSinceLastLoop > 250){
      
      }
  }
}
  
  


 
void pointa_pointb(TSPoint b){
  int blockx1 = (b.x / (8 * standardMapRes));
  int blocky1 = (b.y / (8 * standardMapRes));
  int blockx2 = (player_x / (8 * standardMapRes));
  int blocky2 = (player_y / (8 * standardMapRes));
  while (blockx1 != blockx2){
      if (blockx1 < blockx2){
        blockx2 = blockx2 - 1;
        if(check_collision(blockx2, blocky2)) {
          blockx2++;
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      else if (blockx1 > blockx2) {
        blockx2 = blockx2 + 1;
        if(check_collision(blockx2, blocky2)) {
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          blockx2--;
          return;
        }
      }
      tft.fillRect(player_x, player_y, 40, 40, GREEN);
      draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
      Serial.print("Changing X\n");
      delay(100);
  }

  while (blocky1 != blocky2){
      if (blocky1 < blocky2){
        blocky2 = blocky2 - 1;
        if(check_collision(blockx2, blocky2)) {
          blocky2++;
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      else if (blocky1 > blocky2) {
        blocky2 = blocky2 + 1;
        if(check_collision(blockx2, blocky2)) {
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          blocky2--;
          return;
        }
      }
      Serial.print("Changing Y\n");
      tft.fillRect(player_x, player_y, 40, 40, GREEN);
      draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
      delay(100);
  }
}

bool check_collision(int blockx, int blocky) 
{
  Serial.print(getFromMaps(blockx, blocky, currentMap));
  if (getFromMaps(blockx, blocky, currentMap) != 15) {
    return true;
  }
  return false;
  
}
  

