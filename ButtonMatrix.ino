#include <Wire.h>

#define colBegin 2
#define rowBegin 8

#define cols 2
#define rows 2

uint32_t data = 0;
uint32_t dataPrev = 0;

byte dataArray[4];

int pinCol(int x) {
  return x + colBegin;
}

int pinRow(int x) {
  return x + rowBegin;
}

void setup()
{
  Wire.begin(18);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  for (int i = 0; i < cols; i++)
  {
    int collommen = colBegin + i;
    pinMode(collommen, INPUT_PULLUP);
    Serial.print("pin ");
    Serial.print(collommen);
    Serial.println(" is INPUT_PULLUP");
  }

  for (int i = 0; i < rows; i++)
  {
    int rowwen = (rowBegin + i);
    pinMode(rowwen, OUTPUT);
    Serial.print("pin ");
    Serial.print(rowwen);
    Serial.println(" is OUTPUT");
  }
}


void storeData(byte* storage, uint32_t *data){
  storage[0] = (*data >> 24) & 0xFFFFFF;
  storage[1] = (*data >> 16) & 0xFFFF;
  storage[2] = (*data >> 8) & 0xFF;
  storage[3] = *data & 0xFF;
}


void readMatrix()
{
  for (int i = 0; i < rows; i++)
  {
    digitalWrite(pinRow(i), LOW);
    for (int y = 0; y < cols; y++)
    {
      bitWrite(data, ((i * cols)+ y), digitalRead(pinCol(y)));
    }
    digitalWrite(pinRow(i), HIGH);

  }
}

void bitWriteTest() {

  for (int i = 0; i < rows; i++)
  {
    for (int y = 0; y < cols; y++)
    {
      bool zitplek;

      if (y == 3) {
        zitplek = 1;
      } 
      else {
        zitplek = 0;
      }
      
      bitWrite(data, (i*cols) + y, zitplek);
      
    }
  }
      Serial.println(data, BIN);
}

void requestEvent() {
  storeData(dataArray, &data);
  Wire.write(dataArray, 4);
}

void loop()
{
  readMatrix();
  if (dataPrev != data)
  {
    Serial.println(data + 0b10000000000000000000000000000, BIN);
    dataPrev = data;
  }
}
