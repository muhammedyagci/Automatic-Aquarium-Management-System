#include <virtuabotixRTC.h>

// RTC module connections to manage water changes and lighting for the aquarium
virtuabotixRTC myRTC(6, 7, 8);

unsigned long previous_time;
unsigned long current_time;

int H, M, X = 1; // Hour, Minute, and feeding control variable
int S; // Seconds
int Y = 1;
int light = 0; // Light control variable
int warning = 0; // Warning control variable
int hold1 = 0; // Temporary storage for water flow measurement
int hold2 = 0; // Second temporary storage

const int button = 3; // Button connection for manual control
const int relay1 = 4; // Relay connections for lighting and pump
const int relay2 = 5;
const int led = 13; // LED connection for warnings
const int buzzer = A0; // Buzzer for alert

int period = 900000; // Interval for various operations

// Water flow sensor settings
byte sensorInterrupt = 0; // 0 = digital pin 2 for water sensor
byte sensorPin = 2;
float calibrationFactor = 50; // Water sensor calibration factor (higher = slower measurement)
volatile byte pulseCount; 
float flowRate; // Flow rate in L/min
unsigned int flowMilliLitres; // Flow measurement in milliliters
unsigned long totalMilliLitres; // Total flow measurement
unsigned long litre; // Flow measurement in liters
unsigned long oldTime;

int duration = 3; // Delay time for each step in stepper motor control
#define IN1 12 // Step motor connections
#define IN2 11 
#define IN3 10  
#define IN4 9 

void setup() {
  Serial.begin(9600);

  // Set initial date and time (only run once to set the clock)
  myRTC.setDS1302Time(40, 9, 12, 7, 4, 12, 2022); // (second, minute, hour, day of the week, day, month, year)
  
  // Initialize pins for button, relays, LED, and buzzer
  pinMode(button, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(buzzer, OUTPUT);

  // Initialize pins for step motor control
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 

  // Water sensor setup
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  litre = 0;
  oldTime = 0;

  // Set interrupt for water sensor pulse counting
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void loop() {
  // Update current time in milliseconds
  current_time = millis();
  delay(10);

  // Water flow calculation and display (every second)
  if ((millis() - oldTime) > 1000) {
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;   
    litre = totalMilliLitres / 1000;

    // Print flow rate and total flow
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  
    Serial.print(" L/min\t");       
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres);
    Serial.println(" mL\t");      
    Serial.print(totalMilliLitres / 1000);
    Serial.print(" L");

    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }

  // Real-time clock update and display
  myRTC.updateTime();
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth); Serial.print("/");
  Serial.print(myRTC.month); Serial.print("/");
  Serial.print(myRTC.year); Serial.print(" ");
  Serial.print(myRTC.hours); Serial.print(":");
  Serial.print(myRTC.minutes); Serial.print(":");
  Serial.println(myRTC.seconds);
  delay(500);

  // Get current hour, minute, and second
  H = myRTC.hours;
  M = myRTC.minutes;
  S = myRTC.seconds;

  // Button press detection to control step motor
  int button_state = digitalRead(button);
  if (button_state == HIGH) {
    while (button_state == HIGH) {
      button_state = digitalRead(button);
    }
    rotateClockwise(512);
  }

  // Light control based on time (for aquarium plants)
  if (light == 1) digitalWrite(relay1, HIGH);
  else if (light == 0) digitalWrite(relay1, LOW);

  // Automatic light on/off based on hourly schedule
  if ((H == 22 && M == 1) || (H == 23 && M == 1) || (H == 0 && M == 1) || (H == 1 && M == 1) || (H == 2 && M == 1)) light = 1;
  if ((H == 22 && M == 50) || (H == 23 && M == 50) || (H == 0 && M == 50) || (H == 1 && M == 50) || (H == 2 && M == 50)) light = 0;

  // Feeding mechanism based on time schedule
  if (H == 12 && M == 10 && X == 1) {
    rotateClockwise(512);
    X = 0;
  }

  if (H == 13 || H == 11) X = 1;

  // Diver control for specific hours
  if (H == 10 && M <= 10) digitalWrite(relay2, HIGH);
  else if (H == 20 && M <= 10) digitalWrite(relay2, HIGH);
  else digitalWrite(relay2, LOW);

  // Check if water flow is less than expected and activate warning if needed
  if ((H == 10 && M == 0 && S <= 10) || (H == 20 && M == 0 && S <= 10)) hold1 = litre;
  if ((H == 10 && M == 10 && S <= 10) || (H == 20 && M == 10 && S <= 10)) {
    hold2 = litre;
    if ((hold2 - hold1) < 10) warning = 1; // Trigger warning if flow is less than 10 liters
  }

  // Activate warning system if needed
  if (warning == 1) {
    digitalWrite(relay2, LOW);
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
    tone(buzzer, 1000);
  }
}

// Water pulse counting function
void pulseCounter() {
  pulseCount++;
}

// Step motor control function to rotate clockwise
void rotateClockwise(int stepCount) {
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(IN1, HIGH);
    delay(duration);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    delay(duration);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    delay(duration);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(duration);
    digitalWrite(IN4, LOW);
  }
}
