
#define COMMHEIGHT 30

#define BASE_X_OFF 100

#define EVENT_Y_SIZE 30

const char* OP_STRINGS[] = {"Say Text", "Set Var", "If Cond.", "Transfer"};

const void (*OP_FUNCS[])(int) = {&event_say_text, &event_set_var, &event_if_cond, &event_transfer};

bool grabStringFlag = false;
bool grabVarFlag = false;

bool ifSwtch = false;


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

  if (isNewTouch()){
    Serial.println("Ayo");
    Serial.println("Stack:");
    for (int i = 0; i < programStack->size(); i++){
     
      Serial.println(programStack->at(i));
    }
    popState(0);
  }
}

void clearEButtons(){
  int x = 0;
  for (int i = buttons->size() - 1; i >= 5; i--){

    Button *b = buttons->at(i);
    if (b->callFunc != selectOPCode && b->callFunc != exitEvent){
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
    ifSwtch = false;

    Event e = curEvent->at(i);
    xOff = BASE_X_OFF + 25 * e.next;
   
    
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

      // = "Var 0 ";
      char str[7];
      str[0] = 'V';
      str[1] = 'a';
      str[2] = 'r';
      str[3] = ' ';
      
      byte oper = unpackOperand(e);
      
      if (oper >= 10){
        str[4] = '1';
        str[5] = (oper - 10) + '0';
      }
      else{
        str[4] = oper + '0';
        str[5] = 0;
      } 
      str[6] = 0;

      
      drawText(xOff + 5, y + 20, 1, str, WHITE);
      
    }
    else if (eType == 2){
      ifSwtch = true;

      char str[12];
      str[0] = 'V';
      str[1] = 'a';
      str[2] = 'r';
      str[3] = ' ';

      byte oper = unpackOperand(e);
      
      if (oper >= 10){
        str[4] = '1';
        str[5] = (oper - 10) + '0';
      }
      else{
        str[4] = oper + '0';
        str[5] = ' ';
      } 
      str[6] = 0;
      drawText(xOff + 5, y + 20, 1, str, WHITE);

      
      byte cond = unpackCondType(e);
      drawText(xOff + 5 + 6 * 6, y + 20, 1, conditionals[cond], WHITE);

      byte cons = unpackCondCons(e);


      if (cons >= 10){
        str[0] = (cons / 10) + '0';
        str[1] = (cons % 10) + '0';
      }
      else{
        str[0] = cons + '0';
        str[1] = ' ';
      } 
      
      str[2] = 0;
      
      drawText(xOff + 5 + 10 * 6, y + 20, 1, str, WHITE);
      /*Serial.print("cond is ");
      Serial.println(cond);
      if (cond >= 2){
        str[6] = conditionals[cond][0];
        str[7] = conditionals[cond][1];
      }
      if (cond < 2){
        str[6] = conditionals[cond][0];
        str[7] = ' ';
      }

      str[8] = ' ';
      byte cons = unpackCondCons(e);

      if (cons >= 10){
        str[9] = '1';
        str[10] = (cons - 10) + '0';
      }
      else{
        str[9] = cons + '0';
        str[10] = ' ';
      } 
      
      str[11] = 0;*/

      
      
    //  drawText(xOff + 5, y + 20, 1, str, WHITE);
    }

   
    /*else{
      drawButton(makeButton(100, y, 100, 40, DARKGRAY, LIGHTGRAY, WHITE, (char*)e.val, OP_FUNCS[eType], i));
      
    }*/
    y += EVENT_Y_SIZE;
  }
  if (ifSwtch == true){
    xOff += 25;
  }
  else{ 
    xOff = BASE_X_OFF;
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
    e.next = 0;
    if (ind > 0 && unpackOPCode(curEvent->at(ind - 1)) == IFCOND){
      e.next = (curEvent->at(ind - 1)).next + 1;
    }
    else{
      e.next = 0;
    }
    

    
    if (curEType == 0){
    //  e.val = 0;//malloc(sizeof(byte));
      e.val = 0;
    }
    if (curEType == 1){
      e.val = 0;
      
    }
    if (curEType == 2){
      e.val = 0;
      e.next = 0;
      
    }
   
  
    curEvent->insert(ind, e);
    renderEventTree();
  
  }
}

void runEventMaker(void){

  TSPoint p = getTouchPoint();
  if (p.z == 500){
  
    for (int i = buttons->size() - 1; i >= 0; i--){
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
   setKeyboardMaxLength(25);
   grabStringFlag = true;
   pushToState(KEYBOARD_INPUT);
  }
}

void event_set_var(int ind){
  if (isNewTouch()){
    curE = &(curEvent->at(ind));
    pushToState(SET_VAR);//SELECT_VAR);
  }
}

void event_if_cond(int ind){
  if (isNewTouch()){
    curE = &(curEvent->at(ind));
    pushToState(SET_IF_COND);//SELECT_VAR);
  }
  
  
  
}

void event_transfer(int ind){
  if (isNewTouch()){
    curE = &(curEvent->at(ind));
  }
}
