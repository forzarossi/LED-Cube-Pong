int latches[] = {5, 6, 7, 8, 9, 10, 11, 12};
int swiches[] = {3, 4, A0, A1, A2, A3, A4, A5}; 
int pinSets[3][8] = {{0, 1, 2, 3, 4, 5, 6, 7},
                     {8, 9, 10, 11, 12, 13, 14, 15},
                     {16, 17, 18, 19, 20, 21, 22, 23}};
void setup() 
{
  for(int j = 0; j < 3; j++) 
  {
    for(int i = 0; i < 8; i++)
    {
      pinMode(pinSets[j][i], OUTPUT);
      digitalWrite(pinSets[j][i], LOW);
    }
  }
  
  for(int i = 0; i < 8; i++)
  {
    pinMode(latches[i], OUTPUT);
    pinMode(swiches[i], OUTPUT);
    digitalWrite(latches[i], LOW);
    digitalWrite(swiches[i], LOW);
  }

  for(int i= 0; i < 8; i++)
  {
    digitalWrite(latches[i], HIGH);
  }  
  delay(100);
  for(int i= 0; i < 8; i++)
  {
    digitalWrite(latches[i], LOW);
  }   
}

void loop() 
{
  for(int set = 0; set < 3; set++)
  {
    for(int index = 0; index < 8; index++)
    {
      digitalWrite(pinSets[set][index], HIGH);
      for(int i = 0; i < 8; i++)
      {
        digitalWrite(swiches[i], HIGH);
        for(int j = 0; j < 8; j++)
        {
            //digitalWrite(testPins, HIGH);
            digitalWrite(latches[j], HIGH);
            delay(1000);
            //digitalWrite(testPin, LOW);
            digitalWrite(latches[j], LOW);   
        }
            digitalWrite(swiches[i], LOW);
        }
        digitalWrite(pinSets[set][index], LOW);
    }
  }
}
