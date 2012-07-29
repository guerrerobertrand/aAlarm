#include <Wire.h>
#include <i2ckeypadMod.h>

//Keypad
#define ROWS 4
#define COLS 3

//Adresses
#define I2Ck 0x20
#define aLedRed 0xA0
#define aLedGreen 0x90
#define aBuzz 0xC0

//Sensor states
#define SENSOR_OPEN  "OPEN"
#define SENSOR_CLOSED  "CLOSE"
#define SENSOR_UNKNOWN  "UNKNOWN"
#define NB_SENSORS 1

//SensorState
int lastSensorValue = 0;

//Pins
int buzzerPin = 3;
int sirenPin = 3;
int sensorPin = 2;

i2ckeypad kpd = i2ckeypad(I2Ck, ROWS, COLS);
int sensorValue = -1;
String keys = "";
String keysBuffer = "";
int nbSensors = NB_SENSORS;
String sensors = "";
int tabSensorCurrent[NB_SENSORS];
int tabSensorLast[NB_SENSORS];

void setup()
{
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);  
  pinMode(sirenPin, OUTPUT);  
  pinMode(sensorPin, INPUT);  
  Wire.begin();
  kpd.init();
  
  //init sensor tab
  for(int i = 0; i < NB_SENSORS; i++)
  {
    tabSensorCurrent[i] = 0;
  }
  //init sensor tab last
  for(int i = 0; i < NB_SENSORS; i++)
  {
    tabSensorLast[i] = 0;
  }
  
  ledsAnim();
}

String pollSensor(int nb)
{
  String newStatus = "";
  
  tabSensorCurrent[nb] = digitalRead(sensorPin);
  if (tabSensorCurrent[nb] != tabSensorLast[nb]) {
    tabSensorLast[nb] = tabSensorCurrent[nb];
    newStatus = getSensorStatus(tabSensorCurrent[nb]);
  }
  
  return newStatus;
}

void pollSensors()
{
  for (int i = 0; i < NB_SENSORS; i++)
  {
    String sensorStatus = pollSensor(i);
    if (sensorStatus.length() > 0)
    {
       Serial.println("sensor" + String(i) + ":" + sensorStatus);
    }
  }
}

void loop()
{
  
  
  //keypad section
  char key = kpd.get_key();
  if(key != '\0')
  {
    
    keysBuffer += key;
    
    //Serial.println(key);
    
    switch (key)
    {
      case '*':
        //keys = keysBuffer;
        
        Serial.println(keysBuffer);
        keysBuffer = "";
        break;
    }
    
  }
  
}

/*
UTILITIES
*/
void serialReader()
{
  int makeSerialStringPosition;
  int inByte;
  
  char serialReadString[50];
  const int charCR = 10; //Terminate lines with CR
  const int charNL = 13; //Terminate lines with NL

  inByte = Serial.read();
  makeSerialStringPosition=0;
  

  if (inByte > 0 && (inByte != charCR ||  inByte != charNL)) { //If we see data (inByte > 0) and that data isn't a carriage return
	delay(100); //Allow serial data time to collect (I think. All I know is it doesn't work without this.)

	while ((inByte != charCR || inByte != charNL) && Serial.available() > 0){ // As long as EOL not found and there's more to read, keep reading
	  serialReadString[makeSerialStringPosition] = inByte; // Save the data in a character array
	  makeSerialStringPosition++; //Increment position in array
	  //if (inByte > 0) Serial.println(inByte); // Debug line that prints the charcodes one per line for everything recieved over serial
	  inByte = Serial.read(); // Read next byte
	}

	if (inByte == charCR || inByte == charNL) //If we terminated properly
	{
	  serialReadString[makeSerialStringPosition] = 0; //Null terminate the serialReadString (Overwrites last position char (terminating char) with 0
	  //Serial.println(serialReadString);
          execCommand(serialReadString);
	}
  }
}

//not sure of this function, still it works :p
int stringToNumber(String thisString) {
  int i, value = 0, length;
  length = thisString.length();
  for(i=0; i<length; i++) {
    value = (10*value) + thisString.charAt(i)-(int) '0';;
  }
  return value;
}

int getNbAfterCommand(String command, String commandString)
{
  String strNb = commandString.substring(command.length());
  strNb.trim();
  int sensorNb = stringToNumber(strNb);
  return sensorNb;
}

void execCommand(String serialReadString)
{
  //String cmdGetSensorState = "getSensorState";
  //String cmdGetKeys = "getKeys";
  String cmdGetStatus = "getStatus";
  String cmdSetLedRed = "setLedRed";
  String cmdSetLedGreen = "setLedGreen";
  String cmdSetBuzzer = "setBuzzer";
  String cmdSetSiren = "setSiren";
  
  /*
  if(serialReadString.startsWith(cmdGetSensorState))
  {
    int sensorNb = getNbAfterCommand(cmdGetSensorState, serialReadString);
    //Serial.print(sensorNb);
    Serial.println(getSensorStatus(sensorNb));
  }
  else if(serialReadString.startsWith(cmdGetKeys))
  {
   Serial.println(keys); 
   keys = "";
  }
  */
  if(serialReadString.startsWith(cmdGetStatus))
  {
    //String strKeys = keys;
    
    //String response = sensors;
    Serial.println(keys+"|"+sensors);
    keys="";
    //Serial.println("response");
    //keys = "";
  }
  else if(serialReadString.startsWith(cmdSetLedRed))
  {
  }
  else if(serialReadString.startsWith(cmdSetLedGreen))
  {
  }
  else if(serialReadString.startsWith(cmdSetBuzzer))
  {
  }
  else if(serialReadString.startsWith(cmdSetSiren))
  {
  }
}


String getSensorStatus(int value)
{
  //for now n (sensor number) is unused
  
  //if(statusValue == HIGH) //if sensor connected to "T" (high when pressed)
  if(value == LOW) //if sensor connected to "R" (low when pressed)
  {
    return SENSOR_OPEN;
    
  }
  //else if (statusValue == LOW) //if sensor connected to "T" (high when pressed)
  else if (value == HIGH) //if sensor connected to "R" (low when pressed)
  {
    return SENSOR_CLOSED;
  }
  else
  {
    return SENSOR_UNKNOWN;
  }
}



void i2cKeypadWrite(int data)
{
  Wire.beginTransmission(I2Ck);
  Wire.write(data); 
  Wire.endTransmission();
  Wire.beginTransmission(I2Ck);
  Wire.write((uint8_t)0x70); 
  Wire.endTransmission();
}

void buzz()
{
    i2cKeypadWrite(aBuzz);
}

void ledRed()
{
    i2cKeypadWrite(aLedRed);
}

void ledGreen()
{
    i2cKeypadWrite(aLedGreen);
}

void ledsAnim()
{
  i2cKeypadWrite(aLedRed);
  delay(100);
  i2cKeypadWrite(aLedRed);
  delay(100);
  i2cKeypadWrite(aLedGreen);
  delay(100);
  i2cKeypadWrite(aLedGreen);
  delay(100);
}

void ledRedBuz()
{
  i2cKeypadWrite(aLedRed | aBuzz);
}

void ledGreenBuz()
{
  i2cKeypadWrite(aLedGreen | aBuzz);
}

void sirenOn()
{
  digitalWrite(sirenPin, HIGH);
}

void sirenOff()
{
  digitalWrite(sirenPin, LOW);
}

