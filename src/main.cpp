#define DEBUG             0                   // 1 for debugging, 0 for no debugging
#define PCSERIAL          Serial              // Primary Serial
#define SLAVESERIAL       Serial2             // Secondary Serial
#define IDLE_VIDEO_M      0                   // Minutes   
#define IDLE_VIDEO_S      10                  // Seconds
#define DELAY             ((IDLE_VIDEO_M*60)+IDLE_VIDEO_S)*1000     // Calculate Delay in milliseconds for idle video

#include <Arduino.h>              // Include Arduino Library

bool read_flag = true;            // Flag to read data from slave serial
unsigned long lastUpdate = 0;     // Last update time
uint8_t count = 0;                // Counter for data processing

/**
 * Process the given data.
 * 
 * This function processes the provided data by checking if it starts with any of the letters 'A' to 'H'. 
 * If it does, it extracts the number from the data and adds it to the 'count' variable. 
 * If the 'count' variable reaches or exceeds 100, it prints "*PLAY#" to the PCSERIAL and resets the 'count' to 0. 
 * It also updates the 'lastUpdate' variable with the current millis() value, sets the 'read_flag' to false, and returns.
 * If the 'count' is less than 100, it prints the current 'count' value to the PCSERIAL if DEBUG is enabled.
 */
void processData(String data){    
  // Process data here
  if (DEBUG) { PCSERIAL.println("Processing data: " + data); }
  if (data.startsWith("A") || data.startsWith("B") || data.startsWith("C") || data.startsWith("D") || data.startsWith("E") || data.startsWith("F") || data.startsWith("G") || data.startsWith("H") || data.startsWith("I") || data.startsWith("J") || data.startsWith("K") || data.startsWith("L") ){
  PCSERIAL.println(data);
  
    // String number = data.substring(1);
    // int num = number.toInt();
    // count += num; 
    // if (count >= 100){ 
    //   PCSERIAL.println("*PLAY#"); 
    //   count = 0; 
    //   lastUpdate = millis(); 
    //   read_flag = false; 
    //   return;
    // } 
    // else if (count < 100) { if (DEBUG){PCSERIAL.print("Count: ");} PCSERIAL.println(count); }
  }
}

/**
 * Reads data from the slave serial port and processes it.
 * 
 * This function checks if there is data available in the slave serial port. 
 * If data is available, it reads the data until it encounters a newline character ('\n'). 
 * The read data is then trimmed to remove any leading or trailing whitespace. 
 * If the DEBUG flag is enabled, the received data is printed to the PC serial port. 
 * Finally, the processed data is passed to the processData() function for further processing.
 */
void readSlaveSerial(){
  if(SLAVESERIAL.available()){
    String data = SLAVESERIAL.readStringUntil('\n');
    data.trim();
    // PCSERIAL.println(data);
    // if (DEBUG) { PCSERIAL.println("Received: " + String(data)); }
    processData(data);
  }
}

void setup() {
  PCSERIAL.begin(9600);         // Initialize Primary Serial
  SLAVESERIAL.begin(9600);      // Initialize Secondary Serial

}

void loop() {

  readSlaveSerial();                                      // Read data from slave serial
  // if (read_flag) { readSlaveSerial();}                      // Read data from slave serial
  // if (millis() - lastUpdate >= DELAY) { read_flag = true; readSlaveSerial();} // Set read_flag to true after delay
}
