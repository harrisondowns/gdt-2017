
bool grabValFlag = false;

void drawSetVar(){
  
  if (grabVarFlag == true){
    curE->op = packOPCode(unpackOPCode(*curE));//+= selectedVar;
    curE->op += selectedVar;
    grabVarFlag = false;
  }
  if (grabValFlag == true){
    keyboardS[keyboardSL] = 0;
    curE->val = atoi(keyboardS);
    
    grabValFlag = false;
  }

  
  tft.fillScreen(LIGHTGRAY);

  delay(1);
  drawText(50, 10, 3, "Set Variable", WHITE);

  drawButton(makeButton(20, 50, 100, 50, DARKGRAY, BLACK, WHITE, "", selectVar, 0));

  char *str = "Var 0 ";
  char *vStr = "xx";

  Serial.println("curE:");
  Serial.println((int)curE);
  byte oper = unpackOperand(*curE);
  if (oper >= 10){
    str[4] = '1';
    str[5] = oper - 10 + '0';
  }
  else{
    str[4] = oper + '0';
    str[5] = 0;
  } 
  drawText(30, 60, 2, str, WHITE);

  drawText(125, 60, 3, "=", WHITE);

  drawButton(makeButton(150, 50, 100, 50, DARKGRAY, BLACK, WHITE, "", changeVal, 0));
  
  byte val = (int)curE->val;
  vStr[2] = 0;
  if (val >= 10){
    vStr[0] = val / 10 + '0';
    vStr[1] = val % 10 + '0';
  }
  else{
    vStr[0] = val % 10 + '0';
    vStr[1] = 0;
  } 

  drawText(180, 60, 2, vStr, WHITE);

  drawButton(makeButton(30, 180, 50, 50, BLUE, WHITE, WHITE, "EXIT", popState, 0));
}

void selectVar(int r){
  grabVarFlag = true;
  pushToState(SELECT_VAR);
}

void changeVal(int r){
  grabValFlag = true;
  setKeyboardMaxLength(2);
  numberKeys();
  pushToState(KEYBOARD_INPUT);
}

void runSetVar(){
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

