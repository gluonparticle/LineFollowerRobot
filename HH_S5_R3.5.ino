#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12
#define MOTOR_SPEED 125

// Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 7;
int rightMotorPin2 = 8;

// Left motor
int enableLeftMotor = 5;
int leftMotorPin1 = 9;
int leftMotorPin2 = 10;

unsigned long stopStartTime = 0; // Variable to store the time when stop started
const unsigned long stopDuration = 5000;  // 5 seconds stop duration
const unsigned long ignoreSensorDuration = 3500;  

void setup() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  rotateMotor(0, 0);
}

void loop() {
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // Line following logic
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW) {
    // Both sensors off the line, go straight
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
    
    // Reset stop start time
    stopStartTime = 0;
  } else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW) {
    // Right sensor on the line, turn slightly right
    rotateMotor(-MOTOR_SPEED,MOTOR_SPEED);
  } else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH) {
    // Left sensor on the line, turn slightly left
    rotateMotor(MOTOR_SPEED,-MOTOR_SPEED);
  } else {
    // Both sensors on the line or other conditions, stop
    rotateMotor(0, 0);

    // Check if it's time to stop
    if (stopStartTime == 0) {
      stopStartTime = millis(); // Record the start time of the stop
    } else {
      // Check if it's time to resume after the stop duration
      if (millis() - stopStartTime >= stopDuration) {
        // Resume moving with line following logic after stopping for 5 seconds
        rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
        
        // Ignore sensor input for 2 seconds after resuming
        delay(ignoreSensorDuration);
        
        // Reset stop start time
        stopStartTime = 0;
      }
    }
  }
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else 
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}
