# Wiring Diagram for Arduino Brushless Pump Control

## Basic Wiring Schematic

```
                    24V DC POWER SUPPLY
                    ┌─────────────────┐
                    │                 │
                    │  +24V       GND │
                    └───┬───────────┬─┘
                        │           │
                        │ RED       │ BLACK
                        │           │
                    ┌───▼───────────▼───┐
                    │                   │
                    │   BRUSHLESS PUMP  │
                    │                   │
                    │  GREEN       WHITE│
                    └───┬───────────┬───┘
                        │           │
                        │           │ (Optional: Tachometer)
                        │           │
                    ┌───▼───────────▼───┐
                    │                   │
                    │     ARDUINO       │
                    │                   │
                    │  Pin 9      Pin 8 │
                    │   PWM       INPUT │
                    │                   │
                    │  GND ─────────────┼─┐
                    └───────────────────┘ │
                        │                 │
                        └─────────────────┘
                        (Common Ground to 24V Supply GND)
```

## Detailed Connection Table

| Component | Pin/Terminal | Connects To | Wire Color | Notes |
|-----------|-------------|-------------|------------|-------|
| 24V Power Supply | +24V | Pump Power + | Red | High current capable |
| 24V Power Supply | GND | Pump Power - | Black | Common with Arduino GND |
| 24V Power Supply | GND | Arduino GND | Wire | **CRITICAL: Common Ground** |
| Arduino | Pin 9 (PWM) | Pump Control | Green | PWM signal for speed control |
| Arduino | Pin 8 (Digital) | Pump Tach | White | Optional: Speed feedback |
| Arduino | GND | Power Supply GND | Wire | **CRITICAL: Common Ground** |

## Step-by-Step Wiring Instructions

### Step 1: Power Supply Connections
1. Connect the **RED wire** from the pump to the **+24V terminal** of your power supply
2. Connect the **BLACK wire** from the pump to the **GND terminal** of your power supply

### Step 2: Establish Common Ground (CRITICAL!)
3. Connect a wire from **Arduino GND** to the **GND terminal** of the 24V power supply
   - This creates a common reference point between the Arduino and the pump
   - **Without this, the PWM signal won't work correctly!**

### Step 3: Signal Connections
4. Connect the **GREEN wire** from the pump to **Arduino Pin 9**
   - This is the PWM control signal
5. (Optional) Connect the **WHITE wire** from the pump to **Arduino Pin 8**
   - This provides speed feedback if the pump has a tachometer

### Step 4: Arduino Power
6. Power your Arduino separately via USB or VIN pin
   - **Never connect 24V to Arduino power pins!**
   - Arduino runs on 5V, not 24V

## Visual Wiring Check

Before powering on, verify:
- [ ] Red wire goes to +24V (NOT to Arduino!)
- [ ] Black wire goes to Power Supply GND
- [ ] Arduino GND connected to Power Supply GND (common ground)
- [ ] Green wire goes to Arduino Pin 9
- [ ] White wire goes to Arduino Pin 8 (optional)
- [ ] Arduino is powered separately (USB or 7-12V to VIN)

## Alternative Setup: With External ESC

If your pump requires an external ESC (Electronic Speed Controller):

```
    24V POWER SUPPLY          ESC               BRUSHLESS PUMP
    ┌──────────────┐    ┌─────────────┐    ┌──────────────────┐
    │              │    │             │    │                  │
    │ +24V    GND  │───▶│ V+    V-    │    │                  │
    └──────────────┘    │             │    │  Red, Black,     │
                        │  Motor Out  │───▶│  Green wires     │
                        │             │    │  (3-phase)       │
                        │  Signal GND │    │                  │
                        └─┬─────┬─────┘    └──────────────────┘
                          │     │
    ┌─────────────────────┘     │
    │                            │
    │  ARDUINO                   │
    │  ┌──────────────┐          │
    │  │ Pin 9   GND  │──────────┘
    │  └──────────────┘
    └─────────────────────────────┐
                                  │
    GND (Common Ground) ──────────┘
```

### ESC Connection Table

| ESC Terminal | Connects To | Notes |
|--------------|-------------|-------|
| V+ (Power In) | 24V Power Supply + | |
| V- (Power In) | 24V Power Supply - | |
| Motor Output | Pump Red, Black, Green | 3-phase motor wires |
| Signal Input | Arduino Pin 9 | PWM control |
| Signal GND | Arduino GND | Common ground |

## Important Notes

⚠️ **Critical Safety Reminders:**
1. **Common Ground is Essential** - Without it, the PWM signal has no reference
2. **Never Apply 24V to Arduino** - This will instantly damage your board
3. **Test at Low Speed First** - Start with 20-30% PWM to ensure safe operation
4. **Check Current Rating** - Ensure power supply can handle pump's current draw
5. **Use Proper Wire Gauge** - For high current applications (pump motor)

## Testing the Connection

1. Upload the `simple_pump_control.ino` sketch to your Arduino
2. Open Serial Monitor (9600 baud)
3. Power on the 24V supply
4. The pump should gradually ramp up and down
5. Monitor serial output for speed percentages

## Troubleshooting

**No Response from Pump:**
- Check common ground connection (most common issue)
- Verify 24V is present at pump red/black wires
- Ensure green wire is connected to Pin 9
- Try increasing PWM value manually

**Pump Runs Full Speed Always:**
- Green wire might not be control signal
- May need external ESC
- Check if pump expects 0-10V analog instead of PWM

**Erratic Operation:**
- Check all connections are secure
- Add 100µF capacitor across 24V supply terminals
- Verify adequate power supply current rating
- Check for voltage drops under load
