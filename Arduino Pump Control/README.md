# Arduino Brushless Pump Control Guide

## Pump Specifications
- **Operative Voltage**: 24VDC
- **Motor Type**: Brushless (for analogue or PWM control)
- **Wires**: 4 wires (Red, Black, Green, White)
- **Electronics**: Brushless without integrated electronics

## Understanding the Wiring

### Wire Identification
Based on the pump specifications and common brushless motor wiring conventions:

- **Red Wire**: Positive power supply (24VDC+)
- **Black Wire**: Ground/Negative (24VDC-)
- **Green Wire**: Control signal (PWM input)
- **White Wire**: Tachometer feedback (optional speed sensor output)

**Note**: Since this is a brushless motor without integrated electronics, it requires an Electronic Speed Controller (ESC) that can handle 24VDC input.

## Wiring Diagram

### Option 1: Direct Connection (If pump has built-in ESC functionality)

```
24V Power Supply:
  [+24V] -----> [Red Wire - Pump]
  [GND]  -----> [Black Wire - Pump]

Arduino:
  [GND]  -----> [Power Supply GND] (Common Ground)
  [Pin 9] ----> [Green Wire - Pump] (PWM Signal)
  [Pin 8] <---- [White Wire - Pump] (Optional: Tachometer feedback)
```

### Option 2: With External ESC (Most Common for Brushless Motors)

```
24V Power Supply:
  [+24V] -----> [ESC Power Input +]
  [GND]  -----> [ESC Power Input -]

ESC to Pump:
  [ESC Motor Output] -----> [Red, Black, Green Wires from Pump]
  
Arduino to ESC:
  [GND]  -----> [ESC Signal Ground]
  [Pin 9] ----> [ESC Signal Input] (PWM Signal)

Note: The white wire may be for speed sensing or a third motor phase
```

## Required Components

1. **Arduino Board** (Uno, Nano, Mega, etc.)
2. **24V DC Power Supply** (capable of handling pump's current requirements)
3. **Electronic Speed Controller (ESC)** (if not built into pump)
   - Must support 24V input
   - Must support brushless motors
   - Look for ESCs rated for BLDC motors with 3-phase output
4. **Common Ground Connection** between Arduino and power supply

## Safety Considerations

⚠️ **IMPORTANT SAFETY NOTES:**

1. **Always connect a common ground** between Arduino and the 24V power supply
2. **Never connect 24V directly to Arduino pins** - this will damage your Arduino
3. **Check pump current requirements** and ensure your power supply can handle it
4. **Use appropriate wire gauge** for the current your pump draws
5. **Test with low PWM values first** (start at 20-30% duty cycle)
6. **Ensure proper ventilation** for the pump and ESC
7. **Add a flyback diode** if controlling directly (for motor protection)

## Control Method

The pump speed is controlled via PWM (Pulse Width Modulation) on the green wire:
- **0% PWM (0 duty cycle)**: Pump OFF
- **20-30% PWM**: Minimum speed (good for testing)
- **100% PWM**: Maximum speed

## Additional Notes

- The white wire is typically a tachometer output that provides speed feedback
- Some pumps may use different color conventions - always verify with manufacturer
- If the pump has only motor windings (no built-in driver), you'll definitely need an external ESC
- PWM frequency should typically be between 1-2 kHz for brushless motor ESCs

## Troubleshooting

**Problem**: Pump doesn't respond to PWM signal
- Check common ground connection
- Verify PWM signal is reaching the control wire (use oscilloscope or multimeter)
- Ensure power supply voltage is correct (24V)
- Try different PWM frequencies

**Problem**: Pump runs at full speed regardless of PWM
- The green wire might not be the control input
- May need an external ESC
- Check if pump expects a different control protocol

**Problem**: Erratic operation
- Ensure adequate power supply current rating
- Check for loose connections
- Verify common ground
- Add filtering capacitors (100µF across power supply)
