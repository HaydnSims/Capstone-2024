/*
 * Advanced Arduino Brushless Pump Control
 * 
 * Features:
 * - Serial command interface
 * - Potentiometer control mode
 * - Auto-cycling mode
 * - Speed feedback via tachometer
 * - Emergency stop functionality
 * 
 * Hardware Connections:
 * - Arduino GND to Power Supply GND (IMPORTANT: Common Ground!)
 * - Arduino Pin 9 to Pump Green Wire (PWM Control)
 * - Arduino Pin 8 to Pump White Wire (Tachometer Input)
 * - Arduino Pin A0 to Potentiometer center pin (Optional)
 * - Arduino Pin 2 to Emergency Stop Button (Optional)
 * - 24V Power Supply (+) to Pump Red Wire
 * - 24V Power Supply (-) to Pump Black Wire
 */

// Pin Definitions
const int PWM_PIN = 9;
const int TACH_PIN = 8;
const int POT_PIN = A0;
const int ESTOP_PIN = 2;

// Control Modes
enum ControlMode {
  MANUAL_SERIAL,    // Control via serial commands
  POTENTIOMETER,    // Control via analog pot
  AUTO_CYCLE,       // Automatic speed cycling
  STOPPED           // Emergency stopped
};

ControlMode currentMode = MANUAL_SERIAL;

// Speed control
int targetSpeed = 0;
int currentSpeed = 0;
const int RAMP_DELAY = 10;  // milliseconds between speed changes

// Tachometer
volatile unsigned long tachPulses = 0;
unsigned long lastTachCheck = 0;
float pumpRPM = 0;

// Auto-cycle mode
unsigned long lastCycleUpdate = 0;
int cycleDirection = 1;  // 1 for up, -1 for down

// Emergency stop
volatile bool emergencyStop = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("================================="));
  Serial.println(F("Advanced Pump Control System"));
  Serial.println(F("================================="));
  
  // Configure pins
  pinMode(PWM_PIN, OUTPUT);
  pinMode(TACH_PIN, INPUT_PULLUP);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  
  // Setup interrupts
  attachInterrupt(digitalPinToInterrupt(TACH_PIN), tachISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ESTOP_PIN), emergencyStopISR, FALLING);
  
  // Start with pump off
  analogWrite(PWM_PIN, 0);
  
  printMenu();
}

void loop() {
  // Handle emergency stop
  if (emergencyStop) {
    targetSpeed = 0;
    currentSpeed = 0;
    analogWrite(PWM_PIN, 0);
    currentMode = STOPPED;
    Serial.println(F("\n!!! EMERGENCY STOP ACTIVATED !!!"));
    emergencyStop = false;
    return;
  }
  
  // Process serial commands
  if (Serial.available() > 0) {
    processSerialCommand();
  }
  
  // Update target speed based on mode
  switch (currentMode) {
    case MANUAL_SERIAL:
      // Speed set by serial commands
      break;
      
    case POTENTIOMETER:
      updatePotentiometerSpeed();
      break;
      
    case AUTO_CYCLE:
      updateAutoCycleSpeed();
      break;
      
    case STOPPED:
      targetSpeed = 0;
      break;
  }
  
  // Smooth speed ramping
  if (currentSpeed < targetSpeed) {
    currentSpeed++;
    analogWrite(PWM_PIN, currentSpeed);
    delay(RAMP_DELAY);
  } else if (currentSpeed > targetSpeed) {
    currentSpeed--;
    analogWrite(PWM_PIN, currentSpeed);
    delay(RAMP_DELAY);
  }
  
  // Update RPM display every second
  if (millis() - lastTachCheck >= 1000) {
    calculateRPM();
    lastTachCheck = millis();
  }
}

void processSerialCommand() {
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();
  
  // Mode commands
  if (cmd == "M") {
    currentMode = MANUAL_SERIAL;
    Serial.println(F("Mode: MANUAL (Serial Control)"));
    return;
  }
  
  if (cmd == "P") {
    currentMode = POTENTIOMETER;
    Serial.println(F("Mode: POTENTIOMETER"));
    return;
  }
  
  if (cmd == "A") {
    currentMode = AUTO_CYCLE;
    Serial.println(F("Mode: AUTO CYCLE"));
    return;
  }
  
  if (cmd == "H") {
    printMenu();
    return;
  }
  
  if (cmd == "S") {
    targetSpeed = 0;
    Serial.println(F("Stopping pump..."));
    return;
  }
  
  if (cmd == "R") {
    printStatus();
    return;
  }
  
  // Speed setting (0-100)
  int speedPercent = cmd.toInt();
  if (speedPercent >= 0 && speedPercent <= 100) {
    if (currentMode != MANUAL_SERIAL) {
      currentMode = MANUAL_SERIAL;
      Serial.println(F("Switched to MANUAL mode"));
    }
    targetSpeed = map(speedPercent, 0, 100, 0, 255);
    Serial.print(F("Speed set to: "));
    Serial.print(speedPercent);
    Serial.println(F("%"));
  } else {
    Serial.println(F("Invalid command. Type H for help."));
  }
}

void updatePotentiometerSpeed() {
  int potValue = analogRead(POT_PIN);
  targetSpeed = map(potValue, 0, 1023, 0, 255);
}

void updateAutoCycleSpeed() {
  if (millis() - lastCycleUpdate >= 50) {
    targetSpeed += cycleDirection * 2;
    
    if (targetSpeed >= 255) {
      targetSpeed = 255;
      cycleDirection = -1;
    } else if (targetSpeed <= 0) {
      targetSpeed = 0;
      cycleDirection = 1;
    }
    
    lastCycleUpdate = millis();
  }
}

void calculateRPM() {
  noInterrupts();
  unsigned long pulses = tachPulses;
  tachPulses = 0;
  interrupts();
  
  pumpRPM = pulses * 60.0;
}

void printStatus() {
  Serial.println(F("\n----- PUMP STATUS -----"));
  Serial.print(F("Mode: "));
  switch (currentMode) {
    case MANUAL_SERIAL: Serial.println(F("MANUAL")); break;
    case POTENTIOMETER: Serial.println(F("POTENTIOMETER")); break;
    case AUTO_CYCLE: Serial.println(F("AUTO CYCLE")); break;
    case STOPPED: Serial.println(F("STOPPED")); break;
  }
  Serial.print(F("Speed: "));
  Serial.print((currentSpeed * 100) / 255);
  Serial.println(F("%"));
  Serial.print(F("PWM Value: "));
  Serial.println(currentSpeed);
  Serial.print(F("RPM: "));
  Serial.println(pumpRPM);
  Serial.println(F("-----------------------\n"));
}

void printMenu() {
  Serial.println(F("\n========== MENU =========="));
  Serial.println(F("Control Modes:"));
  Serial.println(F("  M - Manual (Serial) mode"));
  Serial.println(F("  P - Potentiometer mode"));
  Serial.println(F("  A - Auto-cycle mode"));
  Serial.println(F("\nCommands:"));
  Serial.println(F("  0-100 - Set speed percentage"));
  Serial.println(F("  S - Stop pump"));
  Serial.println(F("  R - Read current status"));
  Serial.println(F("  H - Show this menu"));
  Serial.println(F("==========================\n"));
}

// Interrupt Service Routines
void tachISR() {
  tachPulses++;
}

void emergencyStopISR() {
  emergencyStop = true;
}
