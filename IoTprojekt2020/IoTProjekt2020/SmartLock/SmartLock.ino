#include <Arduino.h>    //inkluderer arduino biblotek
#include <SPI.h>      //inkluderer vi forskellige bibloteker
#include "Adafruit_BLE.h"   //inkluderer vi forskellige bibloteker
#include "Adafruit_BluefruitLE_SPI.h"  //inkluderer vi forskellige bibloteker
#include "Adafruit_BluefruitLE_UART.h"    //inkluderer vi forskellige bibloteker
#include <Servo.h>    //inkluderer vi forskellige bibloteker
#include "BluefruitConfig.h"      //inkluderer vi forskellige bibloteker

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>   //inkluderer vi forskellige bibloteker
#endif

/*=========================================================================
       -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         0             //standard bluetooth indstillinger
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"       //standard bluetooth indstillinger
#define MODE_LED_BEHAVIOUR          "MODE"        //standard bluetooth indstillinger
/*=========================================================================*/


Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);   //deklare pins på bluetooth modulet

 //Kontrollerer om der er fejl, hvis der er skriver den fejl i serial monitor
void error(const __FlashStringHelper*err) { 
  Serial.println(err);
  while (1);
}
Servo myservo; //giver servo et navn (myservo)

int pos = 0; //servo starter på position 0

void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(9600); //bruger bauddrate 9600 - så vi kan logge på CoolTerm
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) ) //Hvis bluetooth modul ikke kan starte printer den en error i serial monitor
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE ) //Hvis vi laver et factory reset, printer den "laver factory reset" ud i serial monitor
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) { //Hvis den ikke kan resete skriver den fejl
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info(); //Samler information fra bluetooth modulet i serial monitor

//fra linje 34-66 er debugging info til bluetooth modulet 

  ble.verbose(false);


  Serial.println(F("******************************"));
  // Fra linje 72-79 har det noget at gøre med LED i bluetooth modulet. Standard indstillinger
  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  //Her navngiver vi vores bluetooth module KNP så vi nemmere kan finde den.
  ble.println("AT+GAPDEVNAME=KNP"); // KNP

  // Venter på respons
  ble.waitForOK();

  // venter på en forbindelse
  while (! ble.isConnected()) {
    delay(500);
  }
  // Sætter modulet til DATA MODE- standard indstillinger 
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));

  //Vi har sat servo'en på pin 9 - det fortæller vi Arduino her
  myservo.attach(9);

  //Vi sætter start postion af servoen på 60
  myservo.write(60);
  //Delay så servoen ikke bliver overbelasted
  delay(1000);
}

void loop(void)
{
  // Venter på bruger input
  char n, inputs[BUFSIZE + 1];
  int c;
  while ( ble.available() )
  {
    c = ble.read();
    Serial.print((char)c);

   //opret en string ordre
   //ordre += ((char)c);
   //


  }

  //Inde i vores HTML dokument har vi sat (data 0) til at låse døren
  //så hvis user input C som er låst ovenover er 0 (altså knappen lås dør er trykket)
  //vil følgende ske:
  if (c == '0')

  {
  //servoen ændrer postion til 8
    myservo.write(8);

    delay(1000);
  //printer dør låst ud
    Serial.println("Door Locked");

  }
  //Hvis brugerinput C så er 1 kan unlocker den døren. Står også inde i HTML dokument
  else if (c == '1')

  {
    //Hvis døren låses op ændrer servoen postion til 100
    myservo.write(100);

    delay(1000);
    //Skriver låst op i serial monitor
    Serial.println("Door UnLocked");

  }
    
  delay(1000);

}
