/*
* Control.cpp
* Handles fan control
*
*/

typedef struct _PAIR{
  float temp;
  int percent;
} PAIR;

#define MAX_TABLE 6

_PAIR tempTable[MAX_TABLE];
const int minFanSpeed = 25; // Min speed in percentage
const int maxFanSpeed = 100; // Max speed in percentage
bool gIsRunning = false;


void initControl()
{
    tempTable[0].temp = 72.0F;
    tempTable[0].percent = 30;

    tempTable[1].temp = 75.0F;
    tempTable[1].percent = 35;

    tempTable[2].temp = 80.0F;
    tempTable[2].percent = 50;

    tempTable[3].temp = 85.0F;
    tempTable[3].percent = 75;

    tempTable[4].temp = 90.0F;
    tempTable[4].percent = 80;

    tempTable[5].temp = 100.0F;
    tempTable[5].percent = 90;
  }

int processFanControl(float temperature)
{
  // 1st if below lowest disable fan
  if (temperature < tempTable[0].temp) {
    return 0;
  }
  // 2nd if above max
  if (temperature >= tempTable[MAX_TABLE - 1].temp) {
    return maxFanSpeed;
  }

  // Control from table
  for( int i =1; i < MAX_TABLE; i++) {
    if ((temperature > tempTable[i].temp) && 
        (temperature <= tempTable[i+1].temp)) {
      return tempTable[i].percent;
    }
  }

}