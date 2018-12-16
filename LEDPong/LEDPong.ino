
#include <Wire.h>
// The I2C addresses of the three chips
#define BANK0 0x27
#define BANK1 0x26
#define BANK2 0x25

const byte bankaddr[] = {BANK0, BANK1, BANK2}; //latch, switch, pins

#define LATCH BANK0
#define PINS BANK2

byte buffer[64];

void writeLatch(int pin, boolean state){
  byte pinState = B00000000;
  if(state==HIGH){
    bitSet(pinState, pin);
  }else{
    bitClear(pinState, pin);
  }
  pcf8574_write(bankaddr[0], pinState);
}

void writeSwitch(int pin, boolean state){
  digitalWrite(pin+2,state);
}

void writePin(int pin, boolean state){
  byte pinState = B00000000;
  
  if(state==HIGH){
    bitSet(pinState, pin);
  }else{
    bitClear(pinState, pin);
  }
  pcf8574_write(bankaddr[2], pinState);
}

// Use I2C to write a byte to a chip at the given address
void pcf8574_write(byte addr, byte value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();  
}

void clear_buffer() {
  for(byte i=0;i<64;i++) {
    buffer[i]=0;
  }
}

void write_pixel(int x, int y, int z) {
  int index = z*64+y*8+x;
  int byte_id = index>>3;
  int bit_id  = index&0x07;
  buffer[byte_id] |= (1<<bit_id);
}

void clear_pixel(int x, int y, int z) {
  int index = z*64+y*8+x;
  int byte_id = index>>3;
  int bit_id  = index&0x07;
  buffer[byte_id] &= ~(1<<bit_id);
}

void refresh_buffer() {
  byte *pb = buffer;
  for(byte z=0;z<8;z++) {
    writeSwitch(z, HIGH);
    for(byte y=0;y<8;y++, pb++) {
      if((*pb)==0) continue;
      writeLatch(y, HIGH);
      pcf8574_write(bankaddr[2], (*pb));
      writeLatch(y, LOW);

      writeLatch(y, HIGH);
      pcf8574_write(bankaddr[2], 0);
      writeLatch(y, LOW);      
    }
    writeSwitch(z, LOW);
  }
}

void all_off() {
  pcf8574_write(PINS, 0x00);  // set all data to LOW
  pcf8574_write(LATCH, 0xff); // send data to all latches
  pcf8574_write(LATCH, 0x00); 
  for(byte i=0;i<8;i++) {
    pinMode(i+2, OUTPUT);
    digitalWrite(i+2,LOW);    // init switches and set all to low
  }
}

void setup() 
{
  Wire.begin();
  Wire.setClock(400000L);  // Use I2C fast mode (400kHz). Recommended. 
  all_off();
  randomSeed(analogRead(0));
  create_paddles();
  create_ball();
}

// pong code
#define LENGTH 8
#define WIDTH 8
#define HEIGHT 8
int player1_score = 0;
int player2_score = 0;
int PADDLE_HEIGHT = 4;
int PADDLE_LENGTH = 4;
int TIME_DELAY = 200;                  // TIME DELAY
int X=4, Y=4, Z=4, R=5;
int DX=0, DY=0, DZ=0;                  // speed in x and y
int paddle_pos_x1=2;
int paddle_pos_y1=2;
int paddle_pos_x2=2;
int paddle_pos_y2=2;

void create_paddles()
{
  for(int i = 0; i < PADDLE_HEIGHT; i++)
  {
    for(int j = 0; j < PADDLE_LENGTH; j++)
    {
        write_pixel(0, paddle_pos_y1 + i, paddle_pos_y1 + j);           // Creates paddles at (x,y,z) locations
        write_pixel(7, paddle_pos_x2 + i, paddle_pos_y2 + j);
    }
  }               
}

void update_paddles(int x1, int y1, int x2, int y2)
{
  for(int i = 0; i < PADDLE_HEIGHT; i++)
  {
    for(int j = 0; j < PADDLE_LENGTH; j++)
    {
        clear_pixel(0, paddle_pos_x1 + i, paddle_pos_y1 + j);           // Creates paddles at (x,y,z) locations
        clear_pixel(7, paddle_pos_x2 + i, paddle_pos_y2 + j);
    }
  }
  if(paddle_pos_x1 + x1 + PADDLE_HEIGHT < 9 && paddle_pos_x1 + x1 > -1)
  {
     paddle_pos_x1 += x1;
  }
  if(paddle_pos_x2 + x2 + PADDLE_HEIGHT< 9 && paddle_pos_x2 + x2 > -1 )
  {
     paddle_pos_x2 += x2;
  }
  if(paddle_pos_y1 + y1 + PADDLE_HEIGHT< 9 && paddle_pos_y1 + y1 > -1)
  {
     paddle_pos_y1 += y1;
  }
  if(paddle_pos_y2 + y2 + PADDLE_HEIGHT < 9 && paddle_pos_y2 + y2 > -1)
  {
     paddle_pos_y2 += y2;
  }
    
  for(int i = 0; i < PADDLE_HEIGHT; i++)
  {
    for(int j = 0; j < PADDLE_LENGTH; j++)
      {
          write_pixel(0, paddle_pos_x1 + i, paddle_pos_y1 + j);           // Creates paddles at (x,y,z) locations
          write_pixel(7, paddle_pos_x2 + i, paddle_pos_y2 + j);
      }
    } 
}

void bounce()
{
  if(X + R >= WIDTH + 4)
  {
    DX = -1;
  }  
  if(X - R <= 0 - 5)
  {
    DX = 1;
  }
  if(Y + R >= HEIGHT + 4)
  {
    DY = -1;
  } 
  if(Y - R <= 0 - 5)
  { 
    DY = 1;
  }
  if(Z + R >= LENGTH + 4)
  {
    DZ = -1;
  } 
  if(Z - R <= 0 - 5)
  { 
    DZ = 1;
  }
  clear_pixel(X, Y, Z);
  update_paddles(0, 0, 0, 0);
  X += DX;
  Y += DY;
  Z += DZ;
  
  if(!check_paddle_hit(X, Y, Z))
  {
    create_ball();
  }
  else
  {
     write_pixel(X, Y, Z); 
  }
}

bool check_paddle_hit(int X, int Y, int Z)
{
  if(X == 7)
  {
    if(Y >= paddle_pos_x1  && Y < paddle_pos_x1 + PADDLE_LENGTH)
    {
      if(Z >= paddle_pos_y1 && Z < paddle_pos_y1 + PADDLE_HEIGHT)
      {
        int rand_num = random(3);
        if(rand_num == 0)
        {
           DX = random(-1, 1);
        }
        if(rand_num == 1)
        {
           DY = random(-1, 1);
        }
        if(rand_num == 2)
        {
           DZ = random(-1, 1);
        }
        return true;
      }
    }
    write_pixel(X, Y, Z);
    wait(millis());
    clear_pixel(X, Y, Z);
    flash_cube();
    player1_score++;
    return false;
  }
  if(X == 0)
  {
    if(Y >= paddle_pos_x2  && Y < paddle_pos_x2 + PADDLE_LENGTH)
    {
      if(Z >= paddle_pos_y2 && Z < paddle_pos_y2 + PADDLE_HEIGHT)
      {
        int rand_num = random(3);
        if(rand_num == 0)
        {
           DX = random(0, 2);
        }
        if(rand_num == 1)
        {
           DY = random(0, 2);
        }
        if(rand_num == 2)
        {
           DZ = random(0, 2);
        }
        return true;
      }
    }
    write_pixel(X, Y, Z);
    wait(millis());
    clear_pixel(X, Y, Z);
    flash_cube();
    player2_score++;
    return false;
  }
  
  return true;
}

void wait(long time)
{ 
  while(true)
  {
    if(millis()-time > 1000)
    {
      return;
    }
    refresh_buffer();
  }
}

void flash_cube()
{
  for(int i = 0; i < 3; i++)
  {
     pcf8574_write(PINS, 0xff);  
     pcf8574_write(LATCH, 0xff); 
     pcf8574_write(LATCH, 0x00); 
     for(byte i=0;i<8;i++) 
     {
       digitalWrite(i+2, HIGH);  
     }
     delay(200);
     all_off();
     delay(200);
  }
}

void create_ball()
{
  X=4, Y=4, Z=4, R=5;
  DX=0, DY=0, DZ=0;
  while(DX == 0)
  {
    DX = random(3) - 1;
    DY = random(3) - 1;
    DZ = random(3) - 1;
  }
  write_pixel(X, Y, Z);
  wait(millis());  
}

#define ButL1 6
#define ButR1 7
#define ButU1 8
#define ButD1 9

#define ButL2 10
#define ButR2 11
#define ButU2 12
#define ButD2 13
pinMode(ButL1, INPUT_PULLUP);
pinMode(ButR1, INPUT_PULLUP);
pinMode(ButU1, INPUT_PULLUP);
pinMode(ButD1, INPUT_PULLUP);     
pinMode(ButL2, INPUT_PULLUP);
pinMode(ButR2, INPUT_PULLUP);
pinMode(ButU2, INPUT_PULLUP);
pinMode(ButD2, INPUT_PULLUP);

long prev_time = millis();
void loop() 
{
  if(player1_score == 10)
  {
    for(int z = 0; z < 8; z++)
    {
      for(int y = 0; y < 8; y++)
      {
        write_pixel(0, y, z);
      }
    }
    wait();
    all_off();
    create_ball();
  }
  else if(player2_score == 10)
  {
    for(int z = 0; z < 8; z++)
    {
      for(int y = 0; y < 8; y++)
      {
        write_pixel(7, y, z);
      }
    }
    wait();
    all_off();
    create_ball();
  }
  
  if(millis() - prev_time > TIME_DELAY )
  {
    prev_time = millis();
    //update_paddles(0,-1,0,-1);  // works needs to be sent user commands, x1,y1,x2,y2 
    bounce();
  }
  refresh_buffer();  
}
