#include <RGBmatrixPanel.h>

#define CLK 11 
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

const int buttonPin = 7;
int stateButton;
bool  buttonPressed = false;

/*matrix.drawPixel(63-y, 31-x, matrix.Color333(5, 4, 3))*/

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  pinMode(buttonPin, INPUT);
  matrix.begin();
  stateButton = digitalRead(buttonPin);
}

void loop() {
  int layer, x;
  int startx, endx, width;
  for (layer = 0, startx = 0, endx = 32, width = 12; layer < 64 && width > 0; layer += 4) {
    layermove(&startx, &endx, &width, layer, (130-layer)*(130-layer)/150);
  }
  while (!risingEdge())
    delay(1);
  for (layer = 0; layer < 64; layer += 4)
    block(0, layer, matrix.Color333(0, 0, 0), 32);
}

int risingEdge() {
  int temp = digitalRead(buttonPin);
  if (!stateButton)
    if (temp) {
      stateButton = 1;
      return (1);
    }
  stateButton = temp;
  return (0);
}

void layermove(int* startx, int* endx, int* width, int layer, int spd) {
  int x, i, pressed;
  int newstartx, newendx, newwidth;
  pressed = 0;
  x = -8;
  while (pressed == 0) {
    for (x += 8; x <= 32-*width; x += 4) {
      block(x, layer, matrix.Color333(1, 0, 1), *width);
      for (i = 0; i < spd; i++, delay(1))
        if (risingEdge()) {
          pressed = 1;
          break;
        }
      if (pressed == 1)
        break;
      block(x, layer, matrix.Color333(0, 0, 0), *width);
    }
    if (pressed == 1)
      break;
    for (x -= 8; x >= 0; x -= 4) {
      block(x, layer, matrix.Color333(1, 0, 1), *width);
      for (i = 0; i < spd; i++, delay(1))
        if (risingEdge()) {
          pressed = 1;
          break;
        }
      if (pressed == 1)
        break;
      block(x, layer, matrix.Color333(0, 0, 0), *width);
    }
  }
  if (*startx <= x) {
    newstartx = x;
    if (*endx > x+*width) {
      newendx = x+*width;
    }
    else
      newendx = *endx;
    newwidth = newendx-newstartx;
    *width = newwidth;
    *startx = newstartx;
    *endx = newendx;
    block(0, layer, matrix.Color333(0, 0, 0), 32);
    block(*startx, layer, matrix.Color333(2, 0, 0), *width);
  }
  else {
    newstartx = *startx;
    newendx = x+*width;
    newwidth = newendx-newstartx;
    *endx = newendx;
    *width = newwidth;
    block(0, layer, matrix.Color333(0, 0, 0), 32);
    block(*startx, layer, matrix.Color333(2, 0, 0), *width);
  }
}

void block(int x, int y, int color, int width){
  for(int i = x; i < x+width; i++){
    for(int j = y; j < y+4; j++){
      pixel(i,j,color);
    }
  }
}

void pixel(int x, int y, int color)
{
  matrix.drawPixel(63-y, 31-x, color);
}
