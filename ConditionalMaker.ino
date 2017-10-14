
bool grabVFlag = false;
bool grabValueFlag = false;

int cType = 0;

void drawConditionalMaker(void){
  cType = unpackCondType(*curE);
  if (grabVarFlag == true){
    curE->op = packOPCode(unpackOPCode(*curE));//+= selectedVar;
    curE->op += selectedVar;
    grabVarFlag = false;
  }

  if (grabValueFlag == true){
    keyboardS[keyboardSL] = 0;
    curE->val = pack2Bytes(cType, (byte)atoi(keyboardS));
    Serial.println("curE->val:");
    Serial.println((int)curE->val);
    //curE->val = atoi(keyboardS);
    
    grabValueFlag = false;
  }
  
  tft.fillScreen(LIGHTGRAY);

  drawButton(makeButton(5, 50, 100, 50, DARKGRAY, WHITE, WHITE, "", pickVariable, 0)); 

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
  drawText(15, 60, 2, str, WHITE);

  for (int i = 0; i < 6; i++){
    drawButton(makeButton(125, 35 * i + 20, 60, 25, DARKGRAY, WHITE, WHITE, "", pickCondType, i));
    drawText(140, 35 * i + 22, 3, conditionals[i], WHITE);
  }

  drawButton(makeButton(200, 50, 100, 50, DARKGRAY, WHITE, WHITE, "", changeValue, 0));

  byte val = unpackCondCons(*curE);
  char vStr2[2];
  if (val >= 10){
    vStr2[0] = val / 10 + '0';
    vStr2[1] = val % 10 + '0';
  }
  else{
    vStr2[0] = val % 10 + '0';
    vStr2[1] = 0;
  } 

  drawText(240, 60, 2, vStr2, WHITE);

  drawRect(124, 35 * cType + 19, 62, 27, RED);

  drawButton(makeButton(5, 160, 40, 40, BLUE, WHITE, WHITE, "EXIT", popState, 0));
}

void pickCondType(int type){
  if (cType != type){
    drawRect(124, 35 * cType + 19, 62, 27, WHITE);
    cType = type;
    drawRect(124, 35 * cType + 19, 62, 27, RED);
    curE->val = pack2Bytes(type, unpackCondCons(*curE));
    Serial.print("val is ");
    Serial.println((int)curE->val);
  }
}

void pickVariable(int ind){
  grabVarFlag = true;
  pushToState(SELECT_VAR);
}

void changeValue(int r){
  grabValueFlag = true;
  setKeyboardMaxLength(2);
  numberKeys();
  pushToState(KEYBOARD_INPUT);
}



void runConditionalMaker(void){
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

