/*
 *  Arduino Temperature Data Logging
 *  
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 */

#include <SD.h>
#include <SPI.h>
#include <DS3231.h> // rinkydink
#include <NewPing.h>

File myFile;
DS3231  rtc(SDA, SCL);

int pinCS = 10; // Pin 10 on Arduino Uno
#define trigPin 2
#define echoPin 3
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar = NewPing(trigPin, echoPin, MAX_DISTANCE);
float duration, distance;

void setup() {
    
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  pinMode(7, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
    digitalWrite(7, HIGH);
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  rtc.begin();    
}
void loop() {
  Serial.print(rtc.getDateStr());
  Serial.print(", ");
  Serial.print(rtc.getTimeStr());
  Serial.print(", ");
  Serial.print(float(rtc.getTemp()));
  Serial.print("c, ");
  Serial.print("d = ");
  Serial.print(getdistance()); 
  Serial.println(" cm");
  
 
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(rtc.getDateStr());
    myFile.print(",");    
    myFile.print(rtc.getTimeStr());
    myFile.print(",");    
    myFile.print(float(rtc.getTemp()));
    myFile.print("c,");
    myFile.print("d=");
    myFile.print(getdistance()); 
    myFile.println("cm");
   
    myFile.close(); // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
  delay(1000);
}

float getdistance()  // Measure distance 
{
  int iterations = 5;
  duration = sonar.ping_median(iterations);
  distance = (duration / 2) * 0.0343;
  delay(100);
  return distance;
}
