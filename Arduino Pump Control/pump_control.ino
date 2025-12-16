/*
 * Arduino Brushless Pump Control
 * 
 * This sketch controls a 4-wire brushless pump using PWM
 * 
 * Hardware Connections:
 * - Arduino GND to Power Supply GND (IMPORTANT: Common Ground!)
 * - Arduino Pin 9 to Pump Green Wire (PWM Control)
 * - Arduino Pin 8 to Pump White Wire (Optional: Tachometer Input)
 * - 24V Power Supply (+) to Pump Red Wire
 * - 24V Power Supply (-) to Pump Black Wire
 * 
 * WARNING: Never connect 24V directly to Arduino pins!
 */

// Pin Definitions
const int PWM_PIN = 9;           // PWM output to control pump speed
const int TACH_PIN = 8;          // Tachometer input (optional)
const int POTENTIOMETER_PIN = A0; // Optional: Analog input for speed control

// PWM Settings
const int PWM_FREQUENCY = 1000;   // 1 kHz (adjust if needed: 500-2000 Hz typical)
const int PWM_MIN = 0;            // Minimum PWM value (pump off)
const int PWM_MAX = 255;          // Maximum PWM value (full speed)

// Speed control variables
int targetSpeed = 0;              // Target speed (0-255)
int currentSpeed = 0;             // Current speed (for ramping)

// Tachometer variables
volatile unsigned long tachPulses = 0;
unsigned long lastTachCheck = 0;
float pumpRPM = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Arduino Pump Control - Starting...");
  
  // Configure PWM pin
  pinMode(PWM_PIN, OUTPUT);
  
  // Configure tachometer pin (if used)
  pinMode(TACH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TACH_PIN), tachISR, RISING);
  
  // Set PWM frequency (for pins 9 and 10 on most Arduino boards)
  // This sets Timer1 prescaler for ~1kHz PWM frequency
  // Comment out if you need different frequency
  TCCR1B = (TCCR1B & 0b11111000) | 0x02; // Set prescaler to 8 (for ~4kHz)
  
  // Start with pump off
  analogWrite(PWM_PIN, 0);
  
  Serial.println("Setup complete. Pump ready.");
  Serial.println("Commands:");
  Serial.println("  0-100: Set speed percentage");
  Serial.println("  S: Stop pump");
  Serial.println("  R: Read pump RPM");
}

void loop() {
  // Check for serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
  
  // Smooth speed ramping (optional - helps reduce electrical noise)
  if (currentSpeed < targetSpeed) {
    currentSpeed++;
    analogWrite(PWM_PIN, currentSpeed);
    delay(10);
  } else if (currentSpeed > targetSpeed) {
    currentSpeed--;
    analogWrite(PWM_PIN, currentSpeed);
    delay(10);
  }
  
  // Calculate and display RPM every second
  if (millis() - lastTachCheck >= 1000) {
    calculateRPM();
    lastTachCheck = millis();
  }
}

void processCommand(String cmd) {
  cmd.toUpperCase();
  
  // Stop command
  if (cmd == "S") {
    targetSpeed = 0;
    Serial.println("Stopping pump...");
    return;
  }
  
  // RPM read command
  if (cmd == "R") {
    Serial.print("Current RPM: ");
    Serial.println(pumpRPM);
    return;
  }
  
  // Speed percentage command (0-100)
  int speedPercent = cmd.toInt();
  if (speedPercent >= 0 && speedPercent <= 100) {
    targetSpeed = map(speedPercent, 0, 100, PWM_MIN, PWM_MAX);
    Serial.print("Setting pump speed to ");
    Serial.print(speedPercent);
    Serial.print("% (PWM: ");
    Serial.print(targetSpeed);
    Serial.println(")");
  } else {
    Serial.println("Invalid command. Enter 0-100 for speed percentage, S to stop, or R to read RPM.");
  }
}

void calculateRPM() {
  // Disable interrupts while reading volatile variable
  noInterrupts();
  unsigned long pulses = tachPulses;
  tachPulses = 0;
  interrupts();
  
  // Calculate RPM (assumes 1 pulse per revolution - adjust if different)
  pumpRPM = (pulses * 60.0);  // pulses per second * 60 = RPM
  
  if (pulses > 0) {
    Serial.print("Pump RPM: ");
    Serial.print(pumpRPM);
    Serial.print(" (Speed: ");
    Serial.print((currentSpeed * 100) / 255);
    Serial.println("%)");
  }
}

// Interrupt Service Routine for tachometer
void tachISR() {
  tachPulses++;
}
