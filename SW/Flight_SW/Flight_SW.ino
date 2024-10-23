/*
  This code is based off of the ENAE100 Starter Shield SW
  It has not yet been tested
*/

// Libraries
#include <SPI.h>
#include <SD.h>
#include <RTClib.h> // Install RTClib by Adafruit
#include <Adafruit_MS8607.h> // Install Adafruit MS8607 by Adafruit
#include <Adafruit_Sensor.h> // General purpose Adafruit Sensor Library

// Define constants which specify which pins we are using on our Arduino
#define CS 10 // Chip Select on SD logger
#define ledPin 9 // LED

// Sensor Objects
Adafruit_MS8607 ms8607; // Temp/Pressure sensor
RTC_DS1307 rtc; // RTC

void setup() {
  Serial.begin(9600);

  // Make sure the logger is hooked up. If it's not nothing will run after this statement
  Serial.print("Initializing SD card...");

  if (!SD.begin(CS)) {
    Serial.println(F("SD card failed, or not present"));
  }
  else {
    Serial.println(F("SD card initialized."));
  }

  // Initialize the sensors and get them to begin
  // MS8607
  if (!ms8607.begin()) {
    Serial.println("Failed to find MS8607 chip");
    while (1) { 
      delay(10); 
    }
  }
  Serial.println("MS8607 Found!");

  ms8607.setHumidityResolution(MS8607_HUMIDITY_RESOLUTION_OSR_8b);
  Serial.print("Humidity resolution set to ");
  switch (ms8607.getHumidityResolution()){
    case MS8607_HUMIDITY_RESOLUTION_OSR_12b: Serial.println("12-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_11b: Serial.println("11-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_10b: Serial.println("10-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_8b: Serial.println("8-bit"); break;
  }
  // ms8607.setPressureResolution(MS8607_PRESSURE_RESOLUTION_OSR_4096);
  Serial.print("Pressure and Temperature resolution set to ");
  switch (ms8607.getPressureResolution()){
    case MS8607_PRESSURE_RESOLUTION_OSR_256: Serial.println("256"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_512: Serial.println("512"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_1024: Serial.println("1024"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_2048: Serial.println("2048"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_4096: Serial.println("4096"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_8192: Serial.println("8192"); break;
  }

  // RTC
  rtc.begin();
  
  //Comment out any RTC adjust lines and upload your code again after you set the RTC for the first time! Otherwise your RTC will reset every time it is powered on!
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Automatically set the real-time clock, when to resetting the time of your RTC make sure to remove its battery
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // Manually set the real-time clock

  pinMode(ledPin, OUTPUT); // Declare LED as an output pin
}

void loop() {
  // Call sensor objects
  sensors_event_t temp, pres, humidity;
  ms8607.getEvent(&pres, &temp, &humidity);
  DateTime now = rtc.now();

  // Get data from sensor objects
  // ms8607 sensor
  float temperature = temp.temperature;
  float pressure = pres.pressure;
  
  // RTC
  int day = now.day(), month = now.month(), year = now.year(), hour = now.hour(), minute = now.minute(), second = now.second();

  // Open datalog file on SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // Write to file
  if (dataFile) {
    if(month < 10)
      dataFile.print(F("0"));
    dataFile.print(month);
    dataFile.print(F("/"));
    if(day < 10) // Note: For ints less than 10, add leading zero to consistently format
      dataFile.print(F("0"));
    dataFile.print(day);
    dataFile.print(F("/"));
    dataFile.print(year);
    dataFile.print(F(" "));
    if(hour < 10)
      dataFile.print(F("0"));
    dataFile.print(hour);
    dataFile.print(F(":"));
    if(minute < 10)
      dataFile.print(F("0"));
    dataFile.print(minute);
    dataFile.print(F(":"));
    if(second < 10)
      dataFile.print(F("0"));
    dataFile.print(second);
    dataFile.print(F(", ")); // End each section of data with a comma, so our output is in the comma-separated-value (.csv) format
    
    dataFile.print(temperature); 
    dataFile.print(F(", "));
    dataFile.print(pressure); 
    dataFile.print(F(", "));
    
    dataFile.println(); // End each data take (row of data) with a new line
    
    dataFile.close(); //It is good practice to close the datafile when you are done using it, we will open it up again when we loop back to this section of the code
  }
  
  // If the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening datalog.txt"));
  }

  // Print to your computer too, if its connected
  //The code below is the same code as above, except we are using Serial.print() to print to our computer instead of dataFile.print() which prints to the datafile
  if(Serial) {
    if(month < 10)
      Serial.print(F("0"));
    Serial.print(month);
    Serial.print(F("/"));
    if(day < 10)
      Serial.print(F("0"));
    Serial.print(day);
    Serial.print(F("/"));
    Serial.print(year);
    Serial.print(F(" "));
    if(hour < 10)
      Serial.print(F("0"));
    Serial.print(hour);
    Serial.print(F(":"));
    if(minute < 10)
      Serial.print(F("0"));
    Serial.print(minute);
    Serial.print(F(":"));
    if(second < 10)
      Serial.print(F("0"));
    Serial.print(second);
    Serial.print(F(", ")); 

    Serial.print(temperature); 
    Serial.print(F(", "));
    Serial.print(pressure); 
    Serial.print(F(", "));

    Serial.println();
  }

  // Flash LED to demonstrate that code is working properly
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
