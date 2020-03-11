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
//  if (false && !bt.BTFastConnect(deviceName, HF_PROFILE)) {
//    int result = bt.loopHandle();
//    SerialUSB.print("Connect result: ");
//    result != 0 ? SerialUSB.println("Failed!") : SerialUSB.println("Success!");
//    if (result == 0) {
//      //deviceConnected = true;
//    }
//  }

  int result = bt.loopHandle();
  SerialUSB.print("Connect result: ");
  result != 0 ? SerialUSB.println("Failed!") : SerialUSB.println("Success!");
}

void unpairDevice() {
  int targetDeviceID = 1;
  char cmd[30];
  
  sprintf(cmd, "AT+QBTUNPAIR=%d\r\n", targetDeviceID);
  if (!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)) {
      ERROR("\r\nERROR: AT+QBTUNPAIR\r\n");
  }
}

int connectInSPP(int deviceID) //Serial Port Profile
{
   char Buffer[200];
   char cmd[20];
   char *s;
   sprintf(cmd,"AT+QSPPREAD=1,1500\r\n");
   MC20_send_cmd(cmd);
   MC20_read_buffer(Buffer,200,DEFAULT_TIMEOUT);
   SerialUSB.println(Buffer);
   MC20_clean_buffer(cmd,20);
   sprintf(cmd,"AT+QBTGPROF=%d\r\n",deviceID);
   MC20_send_cmd(cmd);
   MC20_read_buffer(Buffer,200,DEFAULT_TIMEOUT);
   SerialUSB.println(Buffer);
   if(NULL == ( s = strstr(Buffer,"\"SPP\""))) {
       ERROR("\r\nERROR: No SPP Profile\r\n");
       return -1;
   }
   MC20_clean_buffer(cmd,20);
   sprintf(cmd,"AT+BTCONNECT=%d,%c\r\n",deviceID,*(s-2));
   if(!MC20_check_with_cmd(cmd, "OK", CMD, DEFAULT_TIMEOUT)){
       ERROR("\r\nERROR:AT+BTCONNECT\r\n");
       return -1;
   }
   return 0;
}

void loop() {
  
//  /* Debug */
//  if (SerialUSB.available()) {
//      serialMC20.write(SerialUSB.read());
//  }
//  if (serialMC20.available()) {
//      SerialUSB.write(serialMC20.read());
//  }
}
