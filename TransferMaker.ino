// Screen for making a transfer event

int tempMapVar = 0;

bool reapMapFlag = false;
bool reapXVal = false;
bool reapYVal = false;

void drawTransfer(void){
  if (reapMapFlag == true){
    curE->z = currentMap;
    currentMap = tempMapVar;
    reapMapFlag = false;
  }
  if (reapXVal == true){
    curE->x = atoi(keyboardS);
    reapXVal = false;
  }
  if (reapYVal == true){
    curE->y = atoi(keyboardS);
    reapYVal = false;
  }

  
  tft.fillScreen(LIGHTGRAY);

  char STR[3];
  if (curE->x >= 10){
    STR[0] = (curE->x / 10) + '0';
    STR[1] = (curE->x % 10) + '0';
    STR[2] = 0;
  }
  if (curE->x < 10){
    STR[0] = ' ';
    STR[1] = (curE->x % 10) + '0';
    STR[2] = 0;
  }

  drawButton(makeButton(20, 70, 60, 60, DARKGRAY, WHITE, WHITE, "", changeX, 0));
  drawText(25, 80, 2, STR, WHITE);

  if (curE->y >= 10){
    STR[0] = (curE->y / 10) + '0';
    STR[1] = (curE->y % 10) + '0';
    STR[2] = 0;
  }
  if (curE->y < 10){
    STR[0] = ' ';
    STR[1] = (curE->y % 10) + '0';
    STR[2] = 0;
  }

  drawButton(makeButton(110, 70, 60, 60, DARKGRAY, WHITE, WHITE, "", changeY, 0));
  drawText(115, 80, 2, STR, WHITE);



  if (curE->z >= 10){
    STR[0] = (curE->z / 10) + '0';
    STR[1] = (curE->z % 10) + '0';
    STR[2] = 0;
  }
  if (curE->z < 10){
    STR[0] = ' ';
    STR[1] = (curE->z % 10) + '0';
    STR[2] = 0;
  }

  drawButton(makeButton(240, 70, 60, 60, DARKGRAY, WHITE, WHITE, "", changeZ, 0));
  drawText(245, 80, 2, STR, WHITE);


  drawText(30, 45, 3, "X:", BLACK);
  drawText(120, 45, 3, "Y:", BLACK);
  drawText(240, 45, 3, "Map:", BLACK);
  drawText(60, 7, 3, "Transfer Maps!", BLACK);

  drawButton(makeButton(15, 190, 40, 40, BLUE, WHITE, WHITE, "EXIT!", popState, 0));
  
}


void changeX(int rip){
  reapXVal = true;
  setKeyboardMaxLength(1);
  numberKeys();
  pushToState(KEYBOARD_INPUT);
}

void changeY(int rip){
  reapYVal = true;
  setKeyboardMaxLength(1);
  numberKeys();
  pushToState(KEYBOARD_INPUT);
}

void changeZ(int rip){
  int tempMapVar = currentMap;
  reapMapFlag = true;
  pushToState(SELECT_MAP);
  
}


void runTransfer(void){
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

