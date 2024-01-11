#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12
#define SOUND_SENSOR A0  // Connect the sound sensor to analog pin A0
#define MOTOR_SPEED 180
#define THRESHOLD 300  // Adjust the threshold value based on your sound sensor's sensitivity

// Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 7;
int rightMotorPin2 = 8;

// Left motor
int enableLeftMotor = 5;
int leftMotorPin1 = 9;
int leftMotorPin2 = 10;

unsigned long lastStopTime = 0;
const unsigned long stopDuration = 5000;  // 5 seconds stop duration

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000010;  // Set PWM frequency

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(SOUND_SENSOR, INPUT);

  rotateMotor(0, 0);
}

void loop() {
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);
  int soundSensorValue = analogRead(SOUND_SENSOR);

  // Line following logic
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW) {
    // Both sensors off the line, go straight
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  } else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW) {
    // Right sensor on the line, turn right
    rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
  } else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH) {
    // Left sensor on the line, turn left
    rotateMotor(MOTOR_SPEED, -MOTOR_SPEED);
  } else {
    // Both sensors on the line or other conditions, stop
    rotateMotor(0, 0);

    // Check if it's time to stop based on sound sensor input
    if (soundSensorValue > THRESHOLD) {
      // Move forward for 2 seconds
      rotateMotor(MOTOR_SPEED,MOTOR_SPEED);
      delay(1000);
      rotateMotor(0, 0);
    }
  }
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
