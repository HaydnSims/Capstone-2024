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
 * This sketch gradually increases pump speed from 0% to 100% over 10 seconds,
 * then decreases back to 0% and repeats.
 */

const int PWM_PIN = 9;  // PWM output pin for pump control

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Simple Pump Control Starting...");
  
  // Configure PWM pin as output
  pinMode(PWM_PIN, OUTPUT);
  
  // Start with pump off
  analogWrite(PWM_PIN, 0);
  
  delay(2000);  // Wait 2 seconds before starting
  Serial.println("Starting pump ramp-up...");
}

void loop() {
  // Ramp up from 0% to 100% over 10 seconds
  Serial.println("Ramping UP...");
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PWM_PIN, speed);
    
    // Print status every 10% increment
    if (speed % 26 == 0) {
      int percentage = (speed * 100) / 255;
      Serial.print("Speed: ");
      Serial.print(percentage);
      Serial.println("%");
    }
    
    delay(40);  // Delay for smooth ramp (total ~10 seconds for full ramp)
  }
  
  // Hold at maximum speed for 3 seconds
  Serial.println("At maximum speed (100%)");
  delay(3000);
  
  // Ramp down from 100% to 0% over 10 seconds
  Serial.println("Ramping DOWN...");
  for (int speed = 255; speed >= 0; speed--) {
    analogWrite(PWM_PIN, speed);
    
    // Print status every 10% decrement
    if (speed % 26 == 0) {
      int percentage = (speed * 100) / 255;
      Serial.print("Speed: ");
      Serial.print(percentage);
      Serial.println("%");
    }
    
    delay(40);  // Delay for smooth ramp
  }
  
  // Hold at zero speed for 3 seconds
  Serial.println("Pump stopped (0%)");
  delay(3000);
}
