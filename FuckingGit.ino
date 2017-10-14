/*  
 *  BaseEngine.ino - responsible for running the actual GAME. When playing
 *  or playtesting the game, this OS State will be used.
*/

int player_x = 159;
int player_y = 119;
bool is_running;
int target_x;
int target_y;
int TimeSinceLastMove = 0;
bool moving = false;
int txt_box_w = 320;
int txt_box_h = 59;
vector<Event> *found_event;
int curr_pos = 0;

void (*execute_events[])(Event) = { &saytext,
                                    &setvar,
                                    &ifcond,
                                    &transfer_exe 
                                    };
void drawEngine(void){
  tft.fillScreen(currentBackground);
  drawMap(0, 0, 0, standardMapRes);
  position_player();
  draw_character(player_x, player_y);
  //make_text_box("This is a text box!!");
  //drawSprite(50, 50, 0);

  
}


void runEngine(void){
  is_running = true;
    //delay(100);

  //Serial.println("*** RUN ENGINE ****");
  //Serial.print(player_x);
 //Serial.print(player_y); 
  move_character();
   //Serial.println("*** END ENGINE ***");

  
  //Serial.print(digitalRead(23));
 // if(digitalRead(23) == HIGH) {
    //Serial.print("HIGH\n");
  //  pushToState(BASE_MAKER);
 // }
}

void position_player(void)
{
  int blockx = player_x / (8 * standardMapRes);
  int blocky = player_y / (8 * standardMapRes);

  if (getFromMaps(blockx, blocky, currentMap) != 15) {
    find_space();
  }
}

void find_space(void) 
{
  int blockx = random(0,8);
  int blocky = random(0,6);

  while(check_collision(blockx, blocky)) {
    blockx = random(0,8);
    blocky = random(0,6);
  }
  player_x = blockx * (8 * standardMapRes);
  player_x = blockx * (8 * standardMapRes);
}

void move_character(void) 
{
  TimeSinceLastMove += delta;
  TSPoint p = getTouchPoint();
  if ( p.z == 500) {
    //Serial.print("FOUND NEW MOVE\n");
    moving = true;
    target_x = p.x;
    target_y = p.y;
    movement();
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
  int corn_y = (y / (8 * standardMapRes)) * (8 * standardMapRes);
  int corn_x = (x / (8 * standardMapRes)) * (8 * standardMapRes);
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

  
  


 
/*void pointa_pointb(TSPoint b){
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
          blockx2--;
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
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
          blocky2--;
          tft.fillRect(player_x, player_y, 40, 40, GREEN);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      Serial.print("Changing Y\n");
      tft.fillRect(player_x, player_y, 40, 40, GREEN);
      draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
      delay(100);
  }
}*/

bool check_collision(int blockx, int blocky) 
{
  Serial.print(getFromMaps(blockx, blocky, currentMap));
  if (getFromMaps(blockx, blocky, currentMap) != 15) {
    return true;
  }
  return false;
}


void movement(){
  if (player_x == target_x && player_y == target_y) {
    draw_character(player_x, player_y);
    return;
    }
  if(moving){
    if(TimeSinceLastMove > 250){
       tft.fillRect(player_x, player_y, 40, 40, currentBackground);
        int blockx1 = (target_x / (8 * standardMapRes));
        int blocky1 = (target_y / (8 * standardMapRes));
        int blockx2 = (player_x / (8 * standardMapRes));
        int blocky2 = (player_y / (8 * standardMapRes));
        TimeSinceLastMove = 0;
    if (blockx1 != blockx2){
      if (blockx1 < blockx2){
        blockx2 = blockx2 - 1;
        if(check_event(blockx2, blocky2)){
          return;
        }
        if(check_collision(blockx2, blocky2)) {
          blockx2++;
          tft.fillRect(player_x, player_y, 40, 40, currentBackground);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      else if (blockx1 > blockx2) {
        blockx2 = blockx2 + 1;
       if (check_event(blockx2, blocky2)) {
          return;
       }
        if(check_collision(blockx2, blocky2)) {
          blockx2--;
          tft.fillRect(player_x, player_y, 40, 40, currentBackground);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      tft.fillRect(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8), 40, 40, currentBackground);
      draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
    }

    else if (blocky1 != blocky2){
      if (blocky1 < blocky2){
        blocky2 = blocky2 - 1;
      if (check_event(blockx2, blocky2)) {
          return;
       }
        if(check_collision(blockx2, blocky2)) {
          blocky2++;
          tft.fillRect(player_x, player_y, 40, 40, currentBackground);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      else if (blocky1 > blocky2) {
        blocky2 = blocky2 + 1;
       if (check_event(blockx2, blocky2)) {
          return;
       }
        if(check_collision(blockx2, blocky2)) {
          blocky2--;
          tft.fillRect(player_x, player_y, 40, 40, currentBackground);
          draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
          return;
        }
      }
      tft.fillRect(player_x, player_y, 40, 40, currentBackground);
      draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
    }
    else {
      moving = false;
        draw_character(blockx2 * (standardMapRes * 8), blocky2 * (standardMapRes * 8));
    }
    
    }
  }

}

void make_text_box(char *text)
{ 
  tft.fillRect(0, 240 - txt_box_h, txt_box_w, txt_box_h, BLACK);
  tft.drawFastHLine(0, 240 - txt_box_h + 1, 320, LIGHTGRAY);
  drawText(10, 240 - txt_box_h + 25 , 2, text, WHITE);
  
}

bool check_event(int blockx, int blocky)
{
  found_event = find_event(blockx, blocky, currentMap);
  if (found_event == NULL) {
    return false;
  } else {
    do_events(found_event);
    return true;
  }
}

void do_events(vector<Event> *events)
{
  byte OPCode;
  byte operand;
  
  for (int i = 0; i < events->size(); i++) {
    curr_pos = i;
    OPCode = unpackOPCode((*events)[i]);
    execute_events[OPCode]((*events)[i]);
    if (OPCode == IFCOND) {
      i++;
    }
  }
}



void saytext(Event curr_event)
{
  make_text_box((char*) curr_event.val);
  TSPoint p = getTouchPoint();
  delay(330);
  while (p.z != 500){
    //Serial.print("waiting\n");
    p = getTouchPoint();
  }
  tft.fillRect(0, 240 - txt_box_h - 1, txt_box_w, txt_box_h + 1, GREEN);
  drawMap(0, 0, currentMap, standardMapRes);
  return;
  
}
                                    
void setvar(Event curr_event) 
{
  vars[unpackOperand(curr_event)] = (int) curr_event.val;                              
}

void ifcond(Event curr_event)
{
  int var_i = unpackOperand(curr_event);
  int condT = unpackCondType(curr_event);
  int condConst = unpackCondCons(curr_event);
  bool is_true;

  if(condT == 0) {
    is_true = vars[var_i] > condConst;
  } else if (condT == 1) {
    is_true = vars[var_i] < condConst;
  } else if (condT == 2) {
    is_true = vars[var_i] == condConst;
  } else if (condT == 3) {
    is_true = vars[var_i] <= condConst;
  } else if (condT == 4) {
    is_true = vars[var_i] >= condConst;
  } else if (condT == 5) {
    is_true = vars[var_i] != condConst;
  }

  if (is_true) {
    int OPCode = unpackOPCode((*found_event)[curr_pos + 1]);
    execute_events[OPCode]((*found_event)[curr_pos + 1]);
  }
}


void transfer_exe(Event curr_event)  
{
 currentMap = curr_event.z;
 player_x = curr_event.x * (standardMapRes * 8); 
 player_y = curr_event.y * (standardMapRes * 8);
// Serial.println("\n\n\n\n\n\n\nTRANSFER\n\n\n\n\n\n");
// Serial.print(player_x);
// Serial.print(player_y); 
 //target_x = player_x;
 //target_y = player_y;
 tft.fillScreen(currentBackground);
 drawMap(0, 0, currentMap, standardMapRes);
 draw_character(player_x, player_y);
 //TimeSinceLastMove = 330;
 //movement();
}


