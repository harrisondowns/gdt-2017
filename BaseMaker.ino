const int tileBoxS = 24;
const int tileBoxRes = tileBoxS / 8;

bool isEventMode = false;

unsigned backcol = currentBackground;
int currentTileSelected = 15;

void drawMapMaker(void){
  Serial.print("DRAW MAP MAKER \n");
 //tft.fillScreen(GREEN);
  //drawSprite(50, 50, 0);
  fillRect(0, 0, 64, 240, BLACK);
  fillRect(64, 192, 319, 240, BLACK);
  fillRect(64, 0, 8 * 4 * 8, 6 * 4 * 8, currentBackground);
  drawMap(64, 0, currentMap, 4);

  for (int j = 0; j < 6; j++){
    for (int i = 0; i < 8; i++){
      makeButton(64 + i * 32, j * 32, 32, 32, BLACK, BLACK, BLACK, "RIPPO IN PASTA", placeMapTile, j * 8 + i);
    }
  }

  for (int j = 0; j < 2; j++){
    for (int i = 0; i < 8; i++){
      if (j * 8 + i != 15){
        drawSpriteWithRes(j * tileBoxS, i * tileBoxS, j * 8 + i, 3);
      }
      else{
        tft.drawLine(j * tileBoxS, i * tileBoxS, (j + 1) * tileBoxS - 1, (i + 1) * tileBoxS - 1, RED);
        tft.drawLine((j + 1) * tileBoxS - 1, i * tileBoxS, j * tileBoxS, (i + 1) * tileBoxS - 1, RED);
      }
      Button *b = makeButton(j * tileBoxS, i * tileBoxS, tileBoxS, tileBoxS, BLACK, BLACK, BLACK, "\0", selectTilePalette, j * 8 + i);
      
    }
  }

   // color box horizontal lines
  for (int y = 0; y < 9; y++){
    tft.drawLine(0, y * tileBoxS, tileBoxS * 2, y * tileBoxS, WHITE);
  }

  tft.drawLine(0, 0, 0, 8 * tileBoxS, WHITE);
  tft.drawLine(tileBoxS * 2, 0, tileBoxS * 2, 8 * tileBoxS, WHITE);
  tft.drawLine(tileBoxS, 0, tileBoxS, 8 * tileBoxS, WHITE);

  Button *b = makeButton(70, 200, 60, 36, LIGHTGRAY, DARKGRAY, BLACK, "EVENTS", changeToEventMode, 0);
  Button *c = makeButton(260, 200, 60, 36, LIGHTGRAY, DARKGRAY, BLACK, "BG COLOR", changeToBackgroundColor, 0);
  Button *d = makeButton(200, 200, 45, 36, LIGHTGRAY, DARKGRAY, BLACK, "MAPS", changeToMapSelect, 0);
  drawButton(d);
  drawButton(b);
  drawButton(c);

  drawButton(makeButton(5, 200, 60, 36, LIGHTGRAY, WHITE, BLACK, "SPRITES", changeToSpriteManager, 0));

  drawButton(makeButton(135, 200, 60, 36, LIGHTGRAY, WHITE, BLACK, "PLAY", play, 0));

  if (isEventMode == true){
    drawRect(69, 199, 62, 38, RED);
    for (int i = 0; i < tileEvents->size(); i++){
          TileEvent e = tileEvents->at(i);
          if (e.mapInd == currentMap){
            drawRect(e.x * 32 + 64, e.y * 32, 32, 32, WHITE);
          }
        }
  }
  drawRect(currentTileSelected / 8 * 24, currentTileSelected % 8 * 24, 25, 25, RED);



}

void changeToMapSelect(int rip){
  pushToState(SELECT_MAP);
}

void changeToBackgroundColor(int rip){
  pushToState(BG_COLOR);

}

void changeToSpriteManager(int rip){
  pushToState(SPRITE_MANAGER);
}

void play(int rip) {
  pushToState(BASE_ENGINE);
}

void selectTilePalette(int ind){
  if (currentTileSelected != ind){
    drawRect(currentTileSelected / 8 * 24, currentTileSelected % 8 * 24, 25, 25, WHITE);
    currentTileSelected = ind;
    drawRect(currentTileSelected / 8 * 24, currentTileSelected % 8 * 24, 25, 25, RED);
  }
  else{
    currentTileSelected = ind;
  }
  
}

void changeToEventMode(int rip){
  if (isNewTouch()){
    if (isEventMode == false){
      drawRect(69, 199, 61, 37, RED);
      isEventMode = true;
    }
    else{
      drawRect(69, 199, 61, 37, WHITE);
      isEventMode = false;
    }
  }
} 





void placeMapTile(int coord){
  int x = coord % 8;
  int y = coord / 8;
  int z = currentMap;
  if (isEventMode == false){
    if (getFromMaps(x, y, z) != currentTileSelected){
     if (currentTileSelected != 15){
      setMaps(x, y, z, currentTileSelected);
      fillRect(64 + x * 32, y * 32, 32, 32, currentBackground);
      drawSpriteWithRes(64 + x * 32, y * 32, currentTileSelected, 4);
     }
     else{
      setMaps(x, y, z, currentTileSelected);
      fillRect(64 + x * 32, y * 32, 32, 32, currentBackground);
     }
    }
  }
  else{
    curEvent = eventOf(x, y, z);
    
    pushToState(BASE_EVENT);
  }
}

void runMapMaker(void){
  TSPoint p = getTouchPoint();
  if (p.z == 500){
    
    for (int i = 0; i < buttons->size(); i++){
      Button *b = buttons->at(i);
      if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
        b->callFunc(b->p);
        break;
      }
    }
    
  }
}

