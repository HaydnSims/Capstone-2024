/*
 * Simple Arduino Brushless Pump Control
 * 
 * This is a basic example to get started with pump control
 * 
 * Hardware Connections:
 * - Arduino GND to Power Supply GND (IMPORTANT: Common Ground!)
 * - Arduino Pin 9 to Pump Green Wire (PWM Control)
 * - 24V Power Supply (+) to Pump Red Wire
 * - 24V Power Supply (-) to Pump Black Wire
 * 
 * WARNING: Never connect 24V directly to Arduino pins!
 * 
 * This sketch runs the pump at a specified PWM value for 5 seconds,
 * then stops. Change the PUMP_PWM variable to adjust speed.
 */

const int PWM_PIN = 9;  // PWM output pin for pump control

// === CHANGE THIS VALUE TO CONTROL PUMP SPEED ===
// PWM value: 0 (off) to 255 (full speed)
// Recommended starting value: 64 (25% speed)
const int PUMP_PWM = 64;  
// ===============================================

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Simple Pump Control Starting...");
  
  // Configure PWM pin as output
  pinMode(PWM_PIN, OUTPUT);
  
  // Start with pump off
  analogWrite(PWM_PIN, 0);
  
  delay(2000);  // Wait 2 seconds before starting
  
  // Calculate and display percentage
  int percentage = (PUMP_PWM * 100) / 255;
  Serial.print("Setting pump to ");
  Serial.print(percentage);
  Serial.print("% (PWM: ");
  Serial.print(PUMP_PWM);
  Serial.println(")");
}

void loop() {
  // Turn pump on at specified PWM value
  analogWrite(PWM_PIN, PUMP_PWM);
  Serial.println("Pump ON");
  
  // Run for 5 seconds
  delay(5000);
  
  // Turn pump off
  analogWrite(PWM_PIN, 0);
  Serial.println("Pump OFF");
  
  // Wait 5 seconds before repeating
  delay(5000);
}
