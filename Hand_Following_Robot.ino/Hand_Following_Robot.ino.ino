#include <AFMotor.h>
#include <Servo.h>

// ================= MOTOR SETUP =================

// Front left
AF_DCMotor motor1(1);

// Front right
AF_DCMotor motor2(2);

// Rear left
AF_DCMotor motor3(3);

// Rear right
AF_DCMotor motor4(4);


// ================= SERVO =================

Servo scanServo;

#define SERVO_PIN 9


// ================= ULTRASONIC =================

#define TRIG_PIN A0
#define ECHO_PIN A1


// ================= BUZZER =================

#define BUZZER_PIN 2


// ================= SETTINGS =================

// Distance we want to maintain
#define TARGET_DISTANCE 20

// If hand is closer than this -> move backward
#define TOO_CLOSE 10

// Hand-following range
#define FOLLOW_MAX 15

// Dead zone around 10 cm
#define MIN_STOP_DISTANCE 
#define MAX_STOP_DISTANCE 12


// Motor speed
#define NORMAL_SPEED 170
#define SLOW_SPEED 120
#define BACK_SPEED 150


// ================= FUNCTION: READ DISTANCE =================

long getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duration == 0)
  {
    return 999;
  }

  long distance = duration * 0.0343 / 2;

  return distance;
}


// ================= MOTOR FUNCTIONS =================

void setMotorSpeed(int speed)
{
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}


// FORWARD

void moveForward(int speed)
{
  setMotorSpeed(speed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}


// BACKWARD

void moveBackward(int speed)
{
  setMotorSpeed(speed);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}


// STOP

void stopCar()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}


// ================= BUZZER =================

void beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
}


// ================= SEARCH FUNCTION =================

int searchForObject()
{
  int angles[] = {30, 60, 90, 120, 150};

  long bestDistance = 999;
  int bestAngle = 90;

  Serial.println("Searching for hand...");

  for (int i = 0; i < 5; i++)
  {
    scanServo.write(angles[i]);

    delay(250);

    long distance = getDistance();

    Serial.print("Angle: ");
    Serial.print(angles[i]);

    Serial.print("  Distance: ");
    Serial.println(distance);

    // Ignore very close objects
    if (distance > 3 && distance < bestDistance)
    {
      bestDistance = distance;
      bestAngle = angles[i];
    }
  }

  // Return servo to center
  scanServo.write(90);

  // Object found within follow range
  if (bestDistance <= FOLLOW_MAX)
  {
    Serial.print("Object found at angle: ");
    Serial.println(bestAngle);

    return bestAngle;
  }

  return -1;
}


// ================= TURN TOWARD OBJECT =================

void turnToward(int angle)
{
  // Object is on LEFT
  if (angle < 80)
  {
    Serial.println("Turning LEFT");

    setMotorSpeed(SLOW_SPEED);

    motor1.run(BACKWARD);
    motor3.run(BACKWARD);

    motor2.run(FORWARD);
    motor4.run(FORWARD);

    delay(250);

    stopCar();
  }

  // Object is on RIGHT
  else if (angle > 100)
  {
    Serial.println("Turning RIGHT");

    setMotorSpeed(SLOW_SPEED);

    motor1.run(FORWARD);
    motor3.run(FORWARD);

    motor2.run(BACKWARD);
    motor4.run(BACKWARD);

    delay(250);

    stopCar();
  }
}


// ================= SETUP =================

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  scanServo.attach(SERVO_PIN);

  // Start servo in center
  scanServo.write(90);

  // Set initial motor speed
  setMotorSpeed(NORMAL_SPEED);

  stopCar();

  Serial.println("==============================");
  Serial.println(" HAND FOLLOWING ROBOT");
  Serial.println("==============================");

  beep();

  delay(1000);
}


// ================= MAIN LOOP =================

void loop()
{
  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // =========================================
  // HAND TOO CLOSE
  // =========================================

  if (distance <= TOO_CLOSE)
  {
    Serial.println("HAND TOO CLOSE -> BACKWARD");

    beep();

    moveBackward(BACK_SPEED);

    delay(150);

    stopCar();
  }


  // =========================================
  // IDEAL DISTANCE
  // =========================================

  else if (distance >= MIN_STOP_DISTANCE &&
           distance <= MAX_STOP_DISTANCE)
  {
    Serial.println("IDEAL DISTANCE -> STOP");

    stopCar();
  }


  // =========================================
  // HAND BETWEEN 12 AND 20 CM
  // =========================================

  else if (distance > MAX_STOP_DISTANCE &&
           distance <= FOLLOW_MAX)
  {
    Serial.println("HAND DETECTED -> FOLLOW");

    moveForward(NORMAL_SPEED);

    delay(150);

    stopCar();
  }


  // =========================================
  // NO HAND / OBJECT TOO FAR
  // =========================================

  else
  {
    Serial.println("NO HAND -> SEARCH");

    stopCar();

    int foundAngle = searchForObject();

    // =====================================
    // OBJECT FOUND
    // =====================================

    if (foundAngle != -1)
    {
      Serial.println("OBJECT FOUND");

      turnToward(foundAngle);

      delay(100);
    }

    // =====================================
    // NOTHING FOUND
    // =====================================

    else
    {
      Serial.println("NOTHING FOUND");

      stopCar();

      delay(300);
    }
  }

  delay(50);
}