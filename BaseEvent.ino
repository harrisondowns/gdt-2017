
#define COMMHEIGHT 30

#define BASE_X_OFF 100

#define EVENT_Y_SIZE 30

const char* OP_STRINGS[] = {"Say Text", "Set Var", "If Cond.", "Transfer"};



int curEType = 0;

void drawEventMaker(void){
  tft.fillScreen(LIGHTGRAY);

  tempA = 10;
  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, LIGHTGRAY, WHITE, "Say Text", selectOPCode, SAYTEXT));
  tempA += COMMHEIGHT;
  
  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, LIGHTGRAY, WHITE, "Set Var", selectOPCode, SETVAR));
  tempA += COMMHEIGHT;

  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, LIGHTGRAY, WHITE, "If Cond.", selectOPCode, IFCOND));
  tempA += COMMHEIGHT;

  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, LIGHTGRAY, WHITE, "Transfer", selectOPCode, TRANSFER));
  tempA += COMMHEIGHT;

  drawButton(makeButton(130, 70, 60, 60, DARKGRAY, LIGHTGRAY, WHITE, "BAD!!!!", lol, 2));

  renderEventTree();
}

void selectOPCode(int opCode){
  curEType = opCode;
}

void lol(int rip){
  
}

void clearEButtons(){
  for (int i = buttons->size() - 1; i >= 0; i--){
    Button *b = buttons->at(i);
    if (b->callFunc != &selectOPCode){
      buttons->erase(i);
    }
  }
}

void renderEventTree(){
  clearEButtons();
  
  int y = 10;
  fillRect(BASE_X_OFF, y, 120, curEvent->size() * 40, LIGHTGRAY);

  
  int xOff = BASE_X_OFF;
  for (int i = 0; i < curEvent->size(); i++){
    Event e = curEvent->at(i);
    byte eType = unpackOPCode(e);
    drawButton(makeButton(100, y, 100, 40, DARKGRAY, LIGHTGRAY, WHITE, OP_STRINGS[eType], testSame, 0));
    y += EVENT_Y_SIZE;
  }
  drawButton(makeButton(100, y, 100, 40, DARKGRAY, LIGHTGRAY, WHITE, " ", placeEvent, curEvent->size())); 
}

void testSame(int u){
  Serial.println("LOL");
}

void placeEvent(int ind){
  Event e;
  e.op = packOPCode(curEType);
  curEvent->insert(ind, e);
  renderEventTree();
}

void runEventMaker(void){
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


