#include "MC20_Common.h"
#include "MC20_Arduino_Interface.h"
#include "MC20_BT.h"

// GPSTracker gpsTracker = GPSTracker();
BlueTooth bt = BlueTooth();
int bt_state = -1;
char *deviceName = "Jonny's OP7";
bool deviceConnected = false;


void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);

  bt.Power_On();
  SerialUSB.println("\n\rMC20 power On!");
  bt.BTPowerOn();
  SerialUSB.println("\n\rBT power On!");

  while(IDLE != (bt_state = bt.getBTState())){
    SerialUSB.print("State: ");
    SerialUSB.println(bt_state);
    delay(1000);
  }

  SerialUSB.println("bt state now idle");

  SerialUSB.println("Waiting for connection...");
  if (!bt.BTFastConnect(deviceName, HF_PROFILE)) {
    int result = bt.loopHandle();
    SerialUSB.print("Connect result: ");
    result != 0 ? SerialUSB.println("Failed!") : SerialUSB.println("Success!");
    if (result == 0) {
      //deviceConnected = true;
    }
  }
}

void unpairDevice() {
  int targetDeviceID = 1;
  char cmd[30];
  
  sprintf(cmd, "AT+QBTUNPAIR=%d\r\n", targetDeviceID);
  if (!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)) {
      ERROR("\r\nERROR: AT+QBTUNPAIR\r\n");
  }
}

void loop() {
}
