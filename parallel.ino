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


const int steps_per_revolution = 2048;  // Steps for one revolution


int arr[2][4] = {
  {1 ,2, 3, 4},
  {4, 8, 3, 5}
}; 

int motor1_step = 0;
int motor2_step = 0;
int motor3_step = 0;
int motor4_step = 0;

void setup() {

    Serial.begin(9600);
   Serial.println("Original Array:");
    // printArray(arr);

    bubbleSort(arr);

    // Serial.println("Sorted Array based on the second row:");
    // printArray(arr);

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
}



void loop() {
    int count = 0;

    for(int i=0;i<3;i++){
      rotateMotors(arr);
      delay(1000);
    }

    delay(10000);
}

void bubbleSort(int arr[2][4]) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4 - 1; j++) {
      for (int k = 0; k < 4 - j - 1; k++) {
        // Compare elements in the same 2
        if (arr[i][k] > arr[i][k + 1]) {
          // Swap the elements
          int temp = arr[i][k];
          arr[i][k] = arr[i][k + 1];
          arr[i][k + 1] = temp;
        }
      }
    }
  }
}
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

void rotateMotors(int arr[2][4]) {


  int max = 0;
  int count = 0;
  for(int i=0;i<4;i++){
    if(max < arr[1][i]){
      max = arr[1][i];
    }
  }
  int motor1bool = 0;
  int motor2bool = 0;
  int motor3bool = 0;
  int motor4bool = 0;
 
  

  while(max != count){
    Serial.println(count);
      for (int i = 0; i < steps_per_revolution; i++) {
        if(motor1bool < arr[1][0]){
          OneStep(MOTOR1_PIN_1, MOTOR1_PIN_2, MOTOR1_PIN_3, MOTOR1_PIN_4, &motor1_step, true);  
        }
        if(motor2bool < arr[1][1]){
          OneStep(MOTOR2_PIN_1, MOTOR2_PIN_2, MOTOR2_PIN_3, MOTOR2_PIN_4, &motor2_step, true); 
        }
        if(motor3bool < arr[1][2]){
          OneStep(MOTOR3_PIN_1, MOTOR3_PIN_2, MOTOR3_PIN_3, MOTOR3_PIN_4, &motor3_step, true); 
        }
        if(motor4bool < arr[1][3]){
          OneStep(MOTOR4_PIN_1, MOTOR4_PIN_2, MOTOR4_PIN_3, MOTOR4_PIN_4, &motor4_step, true); 
        }
        delay(2);  // Adjust the delay as needed for your motors
        
      }
      count++;
    motor1bool++;
    motor2bool++;
    motor3bool++;
    motor4bool++;
  }
  
}

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