#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define ButL1 6
#define ButR1 7
#define ButU1 8
#define ButD1 9

#define ButL2 10
#define ButR2 11
#define ButU2 12
#define ButD2 13

#define LENGTH 8
#define WIDTH 8
#define HEIGHT 8

int Cube[LENGTH][WIDTH][HEIGHT]; //TODO: intitlaiz 8x8x8 LEDs
int PADDLE_WIDTH = 1;
int PADDLE_HEIGHT = 3;
int PADDLE_LENGTH = 4;
int TIME_DELAY = 500;               // TIME DELAY
int X=0, Y=0, Z=0, R=4;
int DX=1, DY=1;                     // speed in x and y

/***================================================================
 * Should initalize the buttons and call the create_new_game() method
 ===================================================================**/
void setup() 
{
  pinMode(ButL1, INPUT_PULLUP);
  pinMode(ButR1, INPUT_PULLUP);
  pinMode(ButU1, INPUT_PULLUP);
  pinMode(ButD1, INPUT_PULLUP);    
  
  pinMode(ButL2, INPUT_PULLUP);
  pinMode(ButR2, INPUT_PULLUP);
  pinMode(ButU2, INPUT_PULLUP);
  pinMode(ButD2, INPUT_PULLUP);
     
  randomSeed(analogRead(2));
  X=R;                                    // center x, start at the left edge
  Y=random(R*2, HEIGHT-R*2);              // center y, random starting point
  Serial.begin(115200);
}

/***================================================================
 * TODO
 * Should create the paddles for both users and intialize 
 * the starting position and trijectory of the ball 
 ===================================================================**/
void create_new_game()
{
  for(int i = 0; i < PADDLE_HEIGHT; i++)
  {
    for(int j = 0; j < PADDLE_LENGTH; j++)
    {
        LED_ON(0, 3 + i, 4 + j);           // Creates paddles at (x,y,z) locations
        LED_ON(7, 3 + i, 4 + j);
    }
  }
  LED_ON(X, Y, Z);                          // Creates ball at (X,Y,Z) which are definded above and altered in animate 
}

/***================================================================
 * TODO
 * Should handle the motion of the ball by lighting corespoding LED
 ==================================================================**/
void bounce()
{
  if(X + R >= WIDTH)
  {
    DX=-1;
  }  
  if(X - R <= 0)
  {
    DX=1;
  }
  if(Y + R >= HEIGHT)
  {
    DY=-1;
  } if(Y-R <= 0)
  { 
    DY=1;
  }
  X += DX;
  Y += DX;
  //TODO: set LED (x,y) to light up
  
}

void LED_ON(int x, int y, int z)
{

}

void LED_OFF(int x, int y, int z)
{

}

/* ====================================================================================
 *  TODO
 *  The loop should handle the button presses and move the paddles correspodiningly
 * ====================================================================================
 */
int L1input = 0, R1input = 0, U1input = 0, D1input = 0;    // USER 1's left, right, up, and down buttons
int L2input = 0, R2input = 0, U2input = 0, D2input = 0;    // USER 2's left, right, up, and down buttons
long time0 = 0;
long time1 = 0;
long time2 = 0;
long buttondelay = 100;
int score[] = {0,0};

void loop() 
{
  L1input = digitalRead(ButL1);
  R1input = digitalRead(ButR1);
  U1input = digitalRead(ButU1);
  D1input = digitalRead(ButD1);
  
  L2input = digitalRead(ButL2);
  R2input = digitalRead(ButR2);
  U2input = digitalRead(ButU2);
  D2input = digitalRead(ButD2); 
}
