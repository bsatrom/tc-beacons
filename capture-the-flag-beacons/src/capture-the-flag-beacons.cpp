/*
 * Project capture-the-flag-beacons
 * Description: CTF "Flag" devices that operate as BLE Peripheral 
 * nodes and accept connections from central devices.
 * Author: Brandon Satrom <brandon@particle.io>
 * Date: 07/29/2019
 */
#include "Particle.h"

// SYSTEM_MODE(MANUAL);

// #define TEAM_NAME "TeamOne"
// #define BLE_UUID "422b00f5-56e4-4969-9c70-52cc847f7e2c" // UUID for TEAM ONE

#define TEAM_NAME "TeamTwo"
#define BLE_UUID "107367af-834a-4690-bfc4-133c6e6ca6e4" // UUID for TEAM TWO

const BleUuid serviceUuid(BLE_UUID);

bool coolingDown = false;
unsigned long cooldownStart = 0;
#define COOLDOWN_DELAY 60000

void setup()
{
  Serial.begin();

  pinMode(D7, OUTPUT);

  // Accept connections from Central devices (Badges)
  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
}

void loop()
{
  if (!coolingDown) {
    /* When a connection is made, award a point by publishing a 
    message to the Particle Device Cloud */
    if (BLE.connected()) {
      coolingDown = true;
      cooldownStart = millis();
    
      Particle.publish("tc-ctf/point-scored", TEAM_NAME, PRIVATE);
      Particle.publish("tc-ctf/uuid", BLE_UUID, PRIVATE);
      
      digitalWrite(D7, HIGH);
      delay(2000);
      digitalWrite(D7, LOW);
    }
  } 
  else if (millis() > cooldownStart + COOLDOWN_DELAY)
  {
    coolingDown = false;
  }
}