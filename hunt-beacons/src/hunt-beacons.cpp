/*
 * Project hunt-beacons
 * Description: Project for advertising string values via BLE using a 
 * 3rd Gen Particle Device
 * Author: Brandon Satrom <brandon@particle.io>
 * Date:
 */
#include "Particle.h"

String advText = "Sample";

void setAdvertisingData(bool updateOnly);
int setAdvertisingText(String args);

void setup()
{
  setAdvertisingData(false);

  Particle.function("setAdvText", setAdvertisingText);
}

void loop()
{
}

int setAdvertisingText(String args)
{
  advText = args;

  setAdvertisingData(true);

  return 1;
}

void setAdvertisingData(bool updateOnly)
{
  uint8_t buf[BLE_MAX_ADV_DATA_LEN];

  size_t offset = 0;

  buf[offset++] = 0xff;
  buf[offset++] = 0xff;
  buf[offset++] = 0x55;

  uint8_t strLen = advText.length();
  buf[offset++] = strLen;

  Particle.publish("beacon/advText", advText.c_str(), PRIVATE);

  memcpy(&buf[offset], advText.c_str(), strLen);
  offset += 4;

  BleAdvertisingData advData;
  advData.appendCustomData(buf, offset);

  if (updateOnly)
  {
    // Only update data
    BLE.setAdvertisingData(&advData);
  }
  else
  {
    BLE.setAdvertisingInterval(130);
    BLE.advertise(&advData);
  }
}
