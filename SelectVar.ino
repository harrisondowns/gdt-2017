

void drawSelectVar(void){
  tft.fillScreen(LIGHTGRAY);
  //Serial.println("RIP");
  char *varName = "Var 0 ";
  for (int i = 0; i < 2; i++){
    for (int j = 0; j < 8; j++){
      if (i * 8 + j < 10){
        varName[4] = (i * 8 + j) + '0';
        varName[5] = 0;
      }
      else{
        varName[4] = '1';
        varName[5] = (i * 8 + j - 10) + '0';
      }
      drawButton(makeButton(60 + i * 100, 5 + j * 30, 80, 25, DARKGRAY, BLACK, WHITE, varName, pickVar, i * 8 + j));
    }
  }
  
}

void pickVar(int pick){
  selectedVar = pick;
  popState(0);
}

void runSelectVar(void){
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
