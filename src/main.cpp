#define DEBUG             0                   // 1 for debugging, 0 for no debugging
#define PCSERIAL          Serial              // Primary Serial
#define SLAVESERIAL       Serial2             // Secondary Serial
#define PLAY_PIN         33                   // Relay Pin
#define RESET_PIN         18                   // Reset Pin

#include <Arduino.h>              // Include Arduino Library

unsigned long lastUpdate = 0;     // Last update time


/**
 * Processes the given data string and prints it if it starts with a character from 'A' to 'H'.
 * 
 * This function checks if the input data string starts with any character from 'A' to 'H'. If it does,
 * the data string is printed to the serial output. Additionally, if debugging is enabled, it prints
 * a message indicating that the data is being processed.
 */
void processData(String data){    
  // Process data here
  if (DEBUG) { PCSERIAL.println("Processing data: " + data); }
  for (char c = 'A'; c <= 'H'; c++) {
    if (data.startsWith(String(c))) {
      PCSERIAL.println(data);
    }
  }
  // if (data.startsWith("A") || data.startsWith("B") || data.startsWith("C") || data.startsWith("D") || data.startsWith("E") || data.startsWith("F") || data.startsWith("G") || data.startsWith("H") || data.startsWith("I") || data.startsWith("J") || data.startsWith("K") || data.startsWith("L") ){
  //   PCSERIAL.println(data);
  // }
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

/**
 * Reads the state of the RF remote control buttons and sends commands via serial.
 *
 * This function checks the state of the PLAY and RESET pins connected to the RF remote control.
 * If the PLAY button is pressed (pin is LOW), it sends the "ZVIP" command via the PC serial interface.
 * If the RESET button is pressed (pin is LOW), it sends the "R" command via the PC serial interface.
 * The function ensures that commands are not sent more frequently than every 500 milliseconds.
 */
void readRFRemote(){
  // if (DEBUG){Serial.println(digitalRead(PLAY_PIN));}
  if (millis() - lastUpdate <500){return;}
  if (digitalRead(PLAY_PIN) == LOW){
    PCSERIAL.println("ZVIP");
    lastUpdate = millis();
  } 
  if (digitalRead(RESET_PIN) == LOW){
    PCSERIAL.println("R");
    lastUpdate = millis();
  }
}

void setup() {
  PCSERIAL.begin(9600);               // Initialize Primary Serial
  SLAVESERIAL.begin(9600);            // Initialize Secondary Serial
  pinMode(PLAY_PIN, INPUT_PULLUP);   // Set Relay Pin as INPUT_PULLUP
  pinMode(RESET_PIN, INPUT_PULLUP);   // Set Reset Pin as INPUT_PULLUP
}

void loop() {
  readRFRemote();
  readSlaveSerial();                                      // Read data from slave serial
}
