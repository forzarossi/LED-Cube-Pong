
#include <Wire.h>
// The I2C addresses of the three chips
#define BANK0 0x27
#define BANK1 0x26
#define BANK2 0x25

const byte bankaddr[] = {BANK0, BANK1, BANK2}; //latch, transistor, pins

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
  byte pinState = B00000000;
  
  if(state==HIGH){
    bitSet(pinState, pin);
  }else{
    bitClear(pinState, pin);
  }
    pcf8574_write(bankaddr[1], pinState);
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


void setup() 
{
    Wire.begin();
    Wire.setClock(400000L);  // Use I2C fast mode (400kHz). Recommended.
    Serial.begin(9600);



  for(int i = 0; i < 8; i++)
  {
    writePin(i, LOW);
    //digitalWrite(pinSet[i], LOW);
  }
  
  for(int i = 0; i < 8; i++)
  {
    writeLatch(i, LOW);
    writeSwitch(i, LOW);
    //digitalWrite(latches[i], LOW);
    //digitalWrite(swiches[i], LOW);
  }

  for(int i= 0; i < 8; i++)
  {
    writeLatch(i, HIGH);
    //digitalWrite(latches[i], HIGH);
  }  
  delay(100);
  for(int i= 0; i < 8; i++)
  {
    writeLatch(i, LOW);
    //digitalWrite(latches[i], LOW);
  }   
}

void loop() 
{
    for(int index = 0; index < 8; index++)
    {
      writePin(index, HIGH);
      //digitalWrite(pinSet[index], HIGH);
      for(int i = 0; i < 8; i++)
      {
        writeSwitch(i, HIGH);
        //digitalWrite(swiches[i], HIGH);
        for(int j = 0; j < 8; j++)
        {
            writeLatch(j, HIGH);           
            //digitalWrite(latches[j], HIGH);
            delay(100);
            writeLatch(j, LOW);
            //digitalWrite(latches[j], LOW);   
        }
            writeSwitch(i,LOW);
            //digitalWrite(swiches[i], LOW);
        }
        writePin(index,LOW);
        //digitalWrite(pinSet[index], LOW);
    }
}
