#define DEBUG             0
#define PCSERIAL          Serial
#define SLAVESERIAL       Serial
#define IDLE_VIDEO_M      0
#define IDLE_VIDEO_S      10 
#define DELAY             ((IDLE_VIDEO_M*60)+IDLE_VIDEO_S)*1000
#include <Arduino.h>

bool read_flag = true;
unsigned long lastUpdate = 0;
uint8_t count = 0;

void processData(String data){
  // Process data here
  if (DEBUG) { PCSERIAL.println("Processing data: " + data); }
  if (data.startsWith("A") || data.startsWith("B") || data.startsWith("C") || data.startsWith("D") || data.startsWith("E") || data.startsWith("F") || data.startsWith("G") || data.startsWith("H") ){
    String number = data.substring(1);
    int num = number.toInt();
    count += num; 
    if (count >= 100){ 
      PCSERIAL.println("*PLAY#"); 
      count = 0; 
      lastUpdate = millis(); 
      read_flag = false; 
      return;
    } 
    else if (count < 100) { if (DEBUG){PCSERIAL.print("Count: ");} PCSERIAL.println(count); }
  }
}

void readSlaveSerial(){
  if(SLAVESERIAL.available()){
    String data = SLAVESERIAL.readStringUntil('\n');
    data.trim();
    if (DEBUG) { PCSERIAL.println("Received: " + String(data)); }
    processData(data);
  }
}

void setup() {
  PCSERIAL.begin(9600);
  SLAVESERIAL.begin(9600);

}

void loop() {
  if (read_flag) { readSlaveSerial();}
  if (millis() - lastUpdate >= DELAY) { read_flag = true; }
}
