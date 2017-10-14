
#define COMMHEIGHT 30

#define BASE_X_OFF 100

#define EVENT_Y_SIZE 30

const char* OP_STRINGS[] = {"Say Text", "Set Var", "If Cond.", "Transfer"};

const void (*OP_FUNCS[])(int) = {&event_say_text, &event_set_var, &event_if_cond, &event_transfer};

bool grabStringFlag = false;
bool grabVarFlag = false;


int curEType = 0;

void drawEventMaker(void){
  if (grabStringFlag == true){
   // curE->val = keyboardS;
    if (curE->val != NULL){
      free(curE->val);
    }
    curE->val = malloc(keyboardSL + 1);
    strcpy(curE->val, keyboardS);
  }
  
  
  tft.fillScreen(LIGHTGRAY);
  Serial.println("Ay");
  tempA = 10;
  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, WHITE, WHITE, "Say Text", selectOPCode, SAYTEXT));
  tempA += COMMHEIGHT;
  
  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, WHITE, WHITE, "Set Var", selectOPCode, SETVAR));
  tempA += COMMHEIGHT;

  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, WHITE, WHITE, "If Cond.", selectOPCode, IFCOND));
  tempA += COMMHEIGHT;

  drawButton(makeButton(10, tempA, 60, 20, DARKGRAY, WHITE, WHITE, "Transfer", selectOPCode, TRANSFER));
  tempA += COMMHEIGHT;

  drawButton(makeButton(10, 200, 40, 40, BLUE, WHITE, WHITE, "EXIT", exitEvent, 0));

  renderEventTree();
}

void selectOPCode(int opCode){
  if (curEType != opCode){
    drawRect(9, 9 + COMMHEIGHT * curEType, 61, 21, WHITE);
    curEType = opCode;
    drawRect(9, 9 + COMMHEIGHT * curEType, 61, 21, RED);
  }
}

void lol(int rip){
  Serial.println("rip");
}

void exitEvent(int rip){
  Serial.println("Ayo");
  Serial.println("Stack:");
  for (int i = 0; i < programStack->size(); i++){
   
    Serial.println(programStack->at(i));
  }
  popState(0);
}

void clearEButtons(){
  int x = 0;
  for (int i = buttons->size() - 1; i >= 0; i--){

    Button *b = buttons->at(i);
    if (b->callFunc != &selectOPCode && b->callFunc != &exitEvent){
      buttons->erase(buttons->begin() + i);
      x++;
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

    drawButton(makeButton(xOff, y, 100, 30, DARKGRAY, LIGHTGRAY, WHITE, OP_STRINGS[eType], OP_FUNCS[eType], i));
    if (eType == 0){
      tempA = 15;
      if (strlen((char*)e.val) < 15){
        tempA = strlen((char*)e.val);
      }
      
      char dest[tempA + 1];

      strncpy(dest, (char*)e.val, tempA);

      if (tempA == 15){
        for (int j = 1; j <= 3; j++){
          dest[tempA - j] = '.';
        }
      }
      dest[tempA] = 0; //null terminate destination
      drawText(xOff + 5, y + 20, 1, dest, WHITE);
    }
    else if (eType == 1){

      char *str = "Var 0 ";
      
      byte oper = unpackOperand(e);
      if (oper >= 10){
        str[4] = '1';
        str[5] = oper - 10 + '0';
      }
      else{
        str[4] = oper + '0';
        str[5] = 0;
      } 

      drawText(BASE_X_OFF + 5, y + 20, 1, str, WHITE);
    }
    
    
    /*else{
      drawButton(makeButton(100, y, 100, 40, DARKGRAY, LIGHTGRAY, WHITE, (char*)e.val, OP_FUNCS[eType], i));
      
    }*/
    y += EVENT_Y_SIZE;
  }
  drawButton(makeButton(xOff, y, 100, 40, WHITE, BLACK, WHITE, "", placeEvent, curEvent->size())); 
}

void testSame(int u){
  Serial.println("LOL");
}

void placeEvent(int ind){
  if (isNewTouch()){
    Event e;
    e.op = packOPCode(curEType);

    if (curEType == 0){
    //  e.val = 0;//malloc(sizeof(byte));
      e.val = 0;
    }
    if (curEType == 1){
      e.val = 0;
      
    }
   
  
    curEvent->insert(ind, e);
    renderEventTree();
  }
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

void event_say_text(int ind){
  if (isNewTouch()){
   curE = &(curEvent->at(ind));
   setKeyboardMaxLength(50);
   grabStringFlag = true;
   pushToState(KEYBOARD_INPUT);
  }
}

void event_set_var(int ind){
  if (isNewTouch()){
    Serial.println("set_var");
    Serial.println(ind);
    curE = &(curEvent->at(ind));
    pushToState(SET_VAR);//SELECT_VAR);
  }
}

void event_if_cond(int ind){
  if (isNewTouch()){
    curE = &(curEvent->at(ind));
  }
}

void event_transfer(int ind){
  if (isNewTouch()){
    curE = &(curEvent->at(ind));
  }
}
