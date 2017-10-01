
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;

#define saveX 70
#define saveY 60
#define saveW 30



void drawSpriteMaker(void){
  
   tft.fillScreen(BLACK);


  // colors
  for (int x = 0; x < 2; x++){
    for (int y = 0; y < 8; y++){

      tft.fillRect(1 + x * BOXSIZE, BOXSIZE*y + 1, BOXSIZE, BOXSIZE, colors[x * 8 + y]);
  
    }
  }

  // color box horizontal lines
  for (int y = 0; y < 9; y++){
    tft.drawLine(0, y * BOXSIZE, BOXSIZE * 2, y * BOXSIZE, WHITE);
  }

  // color pallet box frame
  tft.drawLine(0, 239, BOXSIZE * 2, 239, WHITE);
  tft.drawLine(0, 0, 0, 239, WHITE);
  tft.drawLine(BOXSIZE, 0, BOXSIZE, 240, WHITE);
  tft.drawLine(BOXSIZE * 2, 0, BOXSIZE * 2, 240, WHITE);

  unsigned long start = micros();
  
  tft.setCursor(100, 10);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println("Sprite Maker");

 // top line
  tft.drawLine(sbX, sbY, sbX, sbMY, WHITE);

  // bottom line of grid
  tft.drawLine(sbX, sbMY, sbMX, sbMY, WHITE);

  // vertical grid lines
  for (int x = 0; x < 9; x++){
    int u = x * sbS;
    tft.drawLine(sbX + u, sbY, sbX + u, sbMY, WHITE);
  }

  // horizontal grid lines
  for (int y = 0; y < 9; y++){
    int v = y * 12 * 2;
    tft.drawLine(sbX, sbY + v, sbMX, sbY + v, WHITE);
  }
/*
  tft.fillRect(saveX, saveY, saveW, saveW, BLUE);
  tft.drawRect(saveX - 1, saveY - 1, saveW + 2, saveW + 2, WHITE);
  tft.setCursor(saveX + 3, saveY + 12);
  tft.setTextSize(1);
  tft.println("Exit");
*/
 
  
  osState = 1;

  for (int y = 0; y < 8; y++){
    for (int x = 0; x < 8; x++){
      tft.fillRect(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, colors[sprites[currentSprite][x][y]]);
      makeButton(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, 
                              colors[sprites[currentSprite][x][y]], WHITE, BLACK, "", placeTile, y * 8 + x) ;

    }
  }
   Button *back = makeButton(saveX, saveY, saveW, saveW, BLUE, WHITE, WHITE, "Exit", popState, 0);
  drawButton(back);
  
}

void placeTile(int i){
  Button *b = buttons->at(i); 
  Serial.println("i: ");
  Serial.println(i);
  tft.fillRect(b->x, b->y, sbS - 1, sbS - 1, currentcolor);
  int x = i % 8;
  int y = i / 8;
  sprites[currentSprite][x][y] = colorInd;
     
}

void runSpriteMaker(void){
  TSPoint p = getTouchPoint(); //ts.getPoint();
  if (p.z == 500){
   if (p.x < BOXSIZE) {
       oldcolor = currentcolor;
       currentcolor = colors[p.y / 30];
       colorInd = (p.y / 30);
       
   }
   else if (p.x < BOXSIZE * 2){
      oldcolor = currentcolor;
      colorInd = (p.y / 30) + 8;
      currentcolor = colors[(p.y / 30) + 8];
   }

// if selecting a square on board
  for (int i = 0; i < buttons->size(); i++){
    Button *b = buttons->at(i);
    if (p.x >= b->x && p.x < b->x + b->w && p.y >= b->y && p.y < b->y + b->h){
      b->callFunc(b->p);
      break;
    }

    
  }

  /*
    // if selecting a square on board
   if (p.x > sbX && p.y > sbY && p.x <= sbMX && p.y <= sbMY){
     int x = p.x - sbX;
     int y = p.y - sbY;
     x = x / sbS;
     y = y / sbS;
     if ( x >= 0 && x < 8 && y >= 0 && y < 8){
       tft.fillRect(sbX + x * sbS + 1, sbY + y * sbS + 1, sbS - 1, sbS - 1, currentcolor);
       sprites[currentSprite][x][y] = colorInd;
     }
   }
*/
   /*
   if (p.x >= saveX && p.x < saveX + saveW && p.y >= saveY && p.y < saveY + saveW){
     popState(0);
   }*/
  }
    
  
  /*digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  p = getT*/
/*
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    int x = p.x;
    int y = p.y;

    // rotates touch position to match orientation of screen
    p.x = map(y, TS_MINY, TS_MAXY, tft.width(), 0);
    p.y = map(x, TS_MINX, TS_MAXX, tft.height(), 0);

    // if selecting color pallet

    
   


    
    
  }
  */
}

