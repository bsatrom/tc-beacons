/*
 * Project capture-the-flag-catcher
 * Description: 
 * Author: Brandon Satrom <brandon@particle.io>
 * Date: 07/29/2019
 */
#include "Particle.h"

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

// SYSTEM_MODE(MANUAL);

const size_t SCAN_RESULT_MAX = 30;
bool coolingDown = false;
unsigned long cooldownStart = 0;
#define COOLDOWN_DELAY 60000

// UUID FOR TEAM ONE (CATCHING ON BEACON TWO)
// const BleUuid FLAG_BEACON_UUID = "422b00f5-56e4-4969-9c70-52cc847f7e2c";

/// UUID FOR TEAM TWO (CATCHING ON BEACON ONE)
const BleUuid FLAG_BEACON_UUID = "107367af-834a-4690-bfc4-133c6e6ca6e4";

BleScanResult scanResults[SCAN_RESULT_MAX];
BlePeerDevice beacon;

void setup()
{
  pinMode(D7, OUTPUT);

  (void)logHandler;
}

void loop()
{
  if (!coolingDown)
  {
    if (BLE.connected())
    {
      BLE.disconnect();
    } 
    else
    {
      Log.info("Scanning for beacons...");
      int count = BLE.scan(scanResults, SCAN_RESULT_MAX);

      Log.info("Found: %i", count);
      for (int i = 0; i < count; i++) {
        BleUuid foundServiceId;

        size_t svcsAvailable = scanResults[i].advertisingData.serviceUUID(&foundServiceId, 1);

        Log.info("Services Available: %i", svcsAvailable);
        
        if (svcsAvailable > 0 && foundServiceId == FLAG_BEACON_UUID) {
          Log.info("Found matching service. Connecting...");
          beacon = BLE.connect(scanResults[i].address);

          if (beacon.connected()) {
            coolingDown = true;
            cooldownStart = millis();

            Log.info("Made connection to beacon.");
            
            digitalWrite(D7, HIGH);
            delay(2000);
            digitalWrite(D7, LOW);
            
            BLE.disconnect(beacon);
          } else {
            Log.error("Cannot connect to beacon.");
          }
        }
      }
    }
  }
  else if (millis() > cooldownStart + COOLDOWN_DELAY)
  {
    coolingDown = false;
  }
}