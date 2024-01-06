#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "qrcode.h"

// Code for Arduino Mega or Mega 2560

// Display Size of TFT
#define OLED_WIDTH 256
#define OLED_HEIGHT 256

// Define Three Colors for TFT display
#define BLACK   0x0000
#define RED     0xF800
#define WHITE   0xFFFF
MCUFRIEND_kbv tft;

// Allocating motor pins to coonect in Arduino Mega
#define MOTOR1_PIN_1 22
#define MOTOR1_PIN_2 24
#define MOTOR1_PIN_3 26
#define MOTOR1_PIN_4 28

#define MOTOR2_PIN_1 30
#define MOTOR2_PIN_2 32
#define MOTOR2_PIN_3 34
#define MOTOR2_PIN_4 36

#define MOTOR3_PIN_1 38
#define MOTOR3_PIN_2 40
#define MOTOR3_PIN_3 42
#define MOTOR3_PIN_4 44

#define MOTOR4_PIN_1 46
#define MOTOR4_PIN_2 48
#define MOTOR4_PIN_3 50
#define MOTOR4_PIN_4 52 

// Makes 
const int steps_per_revolution = 2048;  // Steps for one revolution

int motor1_step = 0;
int motor2_step = 0;
int motor3_step = 0;
int motor4_step = 0;

void setup() {
  // Setup Pin Modes for Each Motor.
  pinMode(MOTOR1_PIN_1, OUTPUT);
  pinMode(MOTOR1_PIN_2, OUTPUT);
  pinMode(MOTOR1_PIN_3, OUTPUT);
  pinMode(MOTOR1_PIN_4, OUTPUT);

  pinMode(MOTOR2_PIN_1, OUTPUT);
  pinMode(MOTOR2_PIN_2, OUTPUT);
  pinMode(MOTOR2_PIN_3, OUTPUT);
  pinMode(MOTOR2_PIN_4, OUTPUT);

  pinMode(MOTOR3_PIN_1, OUTPUT);
  pinMode(MOTOR3_PIN_2, OUTPUT);
  pinMode(MOTOR3_PIN_3, OUTPUT);
  pinMode(MOTOR3_PIN_4, OUTPUT);

  pinMode(MOTOR4_PIN_1, OUTPUT);
  pinMode(MOTOR4_PIN_2, OUTPUT);
  pinMode(MOTOR4_PIN_3, OUTPUT);
  pinMode(MOTOR4_PIN_4, OUTPUT);

  // Begin Serial Communication at 115200 for Esp32Cam Data through Arduino Uno
  Serial.begin(115200); 
}

void loop() {

  // Read the Data send By Arduino Uno by reading QR.
  char incomingByte = Serial.read();
  bool start = true;

  // Initialization Input String or Prescription String.
  String inputString = "";
  // Array to hold the Count and Id of Medicine
  int arr[2][4] = {0};
  
  // '|' is the starting delimeter for reading the inputString 
  if(incomingByte == '|'){
    while(start == true){
      char byte = Serial.read();
      // Ending Delimeter
      if(byte == '|'){
        start = false;
        break;
      }
      // Moves all Data to inputString
      inputString.concat(byte);
    }
  }
  // Trial for all Test Cases.
  // inputString = "prescriptions/300/3-2-2-3/4-4-3-4/1-5-2-2/";
  Serial.println(inputString);


  // Declare variables for the split function
  String *resultArray;
  int resultSize;

  // Call the split function to split by "/" ---- prescriptions 300 3-2-2-3
  split(inputString, '/', resultArray, resultSize);

  // Checking the Prefix Keyword for QR
  if(resultArray[0] == "prescriptions"){
    Serial.println(resultArray[1]);

    // String to make the QR for UPI Payment.
    String buyingString = "upi://pay?pa=6353867488@ybl&am=";

    // Make QR for Particular Amount and print the QR on tft Screen.
    buyingString += resultArray[1];
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.fillScreen(BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    displayQRCode(buyingString.c_str());

    // Store All the medicine count in arr
    for (int i = 2; i < resultSize; i++) {
      String *resultArray2;
      int resultSize2;
      
      // Split by '-' as 3 2 2 3 
      split(resultArray[i], '-', resultArray2, resultSize2);
      int totalCount = resultArray2[1].toInt();
      arr[1][resultArray2[0].toInt()-1] = totalCount;
    }

    // Make the array fill with id start from 0 to N 
    for(int i=0;i<4;i++){
        arr[0][i] = i; 
    }

    // Printing and Verifying the Array to be proper for parallel Rotation counts of motor
    printArray(arr);
    bubbleSort(arr);
    printArray(arr);
    // Rotate the motors parallel
    rotateMotors(arr);
    
    // delay between each QR Reading.
    delay(1000);
  }
  
  // Free array
  freeArray(resultArray, resultSize);
  delay(100);
}

// Sorting according to the count of the medicine.
void bubbleSort(int arr[2][4]) {
  for (int i = 0; i < 4 - 1; i++) {
    for (int j = 0; j < 4 - i - 1; j++) {
      // Compare elements in the first row
      if (arr[1][j] > arr[1][j + 1]) {
        // Swap the columns of the entire array
        for (int k = 0; k < 2; k++) {
          int temp = arr[k][j];
          arr[k][j] = arr[k][j + 1];
          arr[k][j + 1] = temp;
        }
      }
    }
  }
}

// Code for display payment QR
void displayQRCode(const char* text) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)] ;
  qrcode_initText(&qrcode, qrcodeData, 3, 0, text) ;
  // int scale = min(OLED_WIDTH / qrcode.size, OLED_HEIGHT/ qrcode.size) ;
  // int shiftX = (OLED_WIDTH - qrcode.size*scale) /2 ;
  // int shiftY = (OLED_HEIGHT - qrcode.size*scale) /2;

  int scale = 10 ;
  int shiftX = (OLED_WIDTH - qrcode.size*scale) /2  + 30;
  int shiftY = (OLED_HEIGHT - qrcode.size*scale) /2 + 120;

  for (uint8_t y = 0; y< qrcode.size; y++) {
    for (uint8_t x = 0; x< qrcode.size; x++) {
      // Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588": " ");
      if(qrcode_getModule(&qrcode,x,y)){
        tft.fillRect(shiftX + x*scale,shiftY + y*scale , scale, scale, WHITE);
      }
    }
    Serial.print("\n");
  }
}

// Splitting function with taking parameter as string delimeter resultString and size
void split(const String &input, char delimiter, String *&result, int &size) {
  int count = 1;

  // Count the number of delimiters
  for (size_t i = 0; i < input.length(); i++) {
    if (input.charAt(i) == delimiter) {
      count++;
    }
  }
  // Allocate memory for the result array
  result = new String[count];
  size = count;

  int index = 0;
  int from = 0;

  // Iterate through each character in the input string
  for (size_t to = 0; to < input.length(); to++) {
    // Check if the current character is the delimiter
    if (input.charAt(to) == delimiter) {
      // Extract the substring and store it in the result array
      result[index++] = input.substring(from, to);
      // Move the 'from' position to the next character after the delimiter
      from = to + 1;
    }
  }
  // If there is a substring after the last delimiter, store it in the result array
  if (from < input.length()) {
    result[index++] = input.substring(from);
  }
}

// Printing Array of MedCount and MedId in proper format
void printArray(int arr[2][4]) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      Serial.print(arr[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}


// Function to deallocate the memory of Arrays
void freeArray(String *&arr, int size) {
  for (int i = 0; i < size; i++) {
    arr[i] = "";  // Clear each String object
  }
  delete[] arr;  // Free the memory
}

// Function to Stepper rotate motor one step each at a time.
void OneStep(int pin1, int pin2, int pin3, int pin4, int *step, bool dir) {
  if (dir) {
    switch (*step) {
      case 0:
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
        break;
      case 1:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, LOW);
        break;
      case 2:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, HIGH);
        digitalWrite(pin4, LOW);
        break;
      case 3:
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        digitalWrite(pin4, HIGH);
        break;
    }
  } else {
    // Code for counter-clockwise rotation (if needed)
    // Similar to the clockwise rotation but in reverse order
  }

  (*step)++;
  if (*step > 3) {
    *step = 0;
  }
}

// Main function for rotating motors parallel
void rotateMotors(int arr[2][4]) {

  int max = 0;
  int count = 0;
  for(int i=0;i<4;i++){
    if(max < arr[1][i]){
      max = arr[1][i];
    }
  }
  // Take the max and count from the array

  // Mapping the medId with MotorPins.
  int motorPinsComb[4][5] = {
    {MOTOR1_PIN_1, MOTOR1_PIN_2, MOTOR1_PIN_3, MOTOR1_PIN_4, &motor1_step},
    {MOTOR2_PIN_1, MOTOR2_PIN_2, MOTOR2_PIN_3, MOTOR2_PIN_4, &motor2_step},
    {MOTOR3_PIN_1, MOTOR3_PIN_2, MOTOR3_PIN_3, MOTOR3_PIN_4, &motor3_step},
    {MOTOR4_PIN_1, MOTOR4_PIN_2, MOTOR4_PIN_3, MOTOR4_PIN_4, &motor4_step}
  };

  // Holding Counter for Each Medicine to Fall.
  int motorCounter[4] = {0};
 
  Serial.println(max);
 
  // Loop till the max Count medicine in Array.
  while(max > count){
    Serial.println(count);
      for (int i = 0; i < steps_per_revolution; i++) {
        // Checking if the Count Requirement of Medicine is fulfilled
        // according to the Presciptions and stop rotating if Med Count Reached. 
        for(int j=0;j<4;j++){
          if(motorCounter[j] < arr[1][j]){
            OneStep(motorPinsComb[arr[0][j]][0], motorPinsComb[arr[0][j]][1], motorPinsComb[arr[0][j]][2], motorPinsComb[arr[0][j]][3], motorPinsComb[arr[0][j]][4], true);  
          }
        }
        delay(2);  // Adjust the delay as needed for your motors 
      }
      count++;

    // Increasing each motor counter for each iteration
    for(int i=0;i<4;i++){
      motorCounter[i]++;
    }
  }  
}
