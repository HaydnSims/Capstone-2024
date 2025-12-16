# Troubleshooting Guide - Arduino Pump Control

This guide covers common issues and solutions when controlling a brushless pump with Arduino.

## Table of Contents
1. [No Response from Pump](#no-response-from-pump)
2. [Pump Runs Full Speed Always](#pump-runs-full-speed-always)
3. [Erratic or Stuttering Operation](#erratic-or-stuttering-operation)
4. [Arduino Issues](#arduino-issues)
5. [Power Supply Issues](#power-supply-issues)
6. [Code/Upload Issues](#codeupload-issues)
7. [Advanced Diagnostics](#advanced-diagnostics)

---

## No Response from Pump

### Symptom
Pump does not run at all, no sound, no movement.

### Diagnostic Steps

#### 1. Check Power Supply
```
Test: Use multimeter to measure voltage at pump red/black wires
Expected: 24V DC (±10%)
```

**If no voltage:**
- Is power supply plugged in and turned on?
- Check fuse in power supply
- Test power supply without load
- Verify power cord connections

**If voltage present:** Continue to next step

#### 2. Check Common Ground ⚠️ MOST COMMON ISSUE
```
Test: Verify continuity between Arduino GND and Power Supply GND
Expected: 0 ohms (direct connection)
```

**Why this matters:**
- PWM signal needs a reference voltage
- Without common ground, signal has no meaning
- This is the #1 cause of "no response" issues

**Solution:**
- Connect a wire from any Arduino GND pin to Power Supply negative terminal
- Verify connection is solid (not loose)

#### 3. Check Control Signal
```
Test: Measure voltage at Arduino Pin 9 while code is running
Expected: Fluctuating voltage (0-5V) as PWM changes
```

**If no signal:**
- Is correct code uploaded?
- Is Arduino powered on (LED should be lit)?
- Try re-uploading the sketch
- Test with different Arduino pin and update code

**If signal present:** Continue to next step

#### 4. Verify Wire Connections
- Green wire properly connected to Pin 9?
- No broken wires or loose connections?
- Try gently wiggling wires while system is running

#### 5. Check if Pump Needs ESC
Some brushless motors need an external Electronic Speed Controller.

**Signs you need an ESC:**
- Pump labeled "brushless without integrated electronics"
- Three thick wires from pump (motor phase wires)
- No response despite correct wiring

**Solution:** See WIRING_DIAGRAM.md for ESC setup

---

## Pump Runs Full Speed Always

### Symptom
Pump runs at maximum speed regardless of PWM signal value.

### Possible Causes & Solutions

#### 1. Wrong Control Wire
**Problem:** Green wire may not be the control input

**Test:**
- Try connecting white wire to Pin 9 instead
- Or try other wire combinations (safely!)

**Note:** Some pumps use different color conventions

#### 2. Wrong Control Type
**Problem:** Pump expects analog voltage (0-10V), not PWM

**Solution:**
- Add RC low-pass filter to convert PWM to analog
- Use DAC (Digital-to-Analog Converter) module
- Or use analog output if available on your Arduino variant

**Simple RC Filter Circuit:**
```
Arduino Pin 9 --[1kΩ resistor]--+--[100µF capacitor]-- GND
                                 |
                                 +-- To pump green wire
```

#### 3. Pump Has No Speed Control
**Problem:** Some pumps are simple on/off only

**Test:**
- Check pump datasheet/specifications
- Look for "variable speed" or "PWM control" in specs

**Solution:**
- If pump is on/off only, use digitalWrite() instead of analogWrite()
- Control via relay if switching high current

#### 4. Wrong PWM Frequency
**Problem:** Pump expects specific PWM frequency

**Test:** Try different frequencies (see code below)

**Solution:** Adjust Timer1 prescaler in code:
```cpp
// In setup():
// For ~4kHz (prescaler = 8):
TCCR1B = (TCCR1B & 0b11111000) | 0x02;

// For ~490Hz (prescaler = 64):
TCCR1B = (TCCR1B & 0b11111000) | 0x03;

// For ~122Hz (prescaler = 256):
TCCR1B = (TCCR1B & 0b11111000) | 0x04;
```

---

## Erratic or Stuttering Operation

### Symptom
Pump speed varies unexpectedly, stutters, or has jerky motion.

### Diagnostic Steps

#### 1. Inadequate Power Supply
**Problem:** Power supply can't provide enough current

**Test:**
- Measure voltage at pump terminals under load
- Should stay at 24V (±1V) even when running

**If voltage drops significantly:**
- Power supply is undersized
- Check pump's current rating
- Upgrade to higher capacity supply

**Current Guidelines:**
```
Small pump:  2-3A supply minimum
Medium pump: 5A supply recommended
Large pump:  10A+ supply required
```

#### 2. Electrical Noise
**Problem:** Motor creates electrical interference

**Solutions:**

**Add Capacitor Across Power Supply:**
```
Place 100-470µF electrolytic capacitor
+ terminal to Power Supply +24V
- terminal to Power Supply GND
```

**Separate Signal and Power Wires:**
- Keep PWM control wires away from power wires
- Use twisted pair for control signals if possible

**Add Ferrite Beads:**
- Clip ferrite bead on pump power wires
- Reduces high-frequency noise

#### 3. Loose Connections
**Problem:** Intermittent electrical contact

**Solution:**
- Check all wire connections are tight
- Re-seat all connections
- Consider soldering for permanent installation
- Use screw terminals for power connections

#### 4. Code Issues
**Problem:** Too-fast speed changes in code

**Solution:**
- Implement gradual ramping (see pump_control.ino)
- Add delay between speed changes
- Smooth transitions prevent mechanical stress

---

## Arduino Issues

### Arduino Resets When Pump Starts

**Cause:** Voltage dip from pump startup current

**Solutions:**
1. Power Arduino from separate supply (not 24V system)
2. Add bulk capacitor to Arduino power input (100µF)
3. Use USB power for Arduino instead of VIN

### Serial Monitor Shows Garbage Characters

**Cause:** Wrong baud rate selected

**Solution:**
- Set Serial Monitor baud rate to 9600
- Match baud rate in code: `Serial.begin(9600);`

### Code Won't Upload

**Possible Causes & Solutions:**

1. **Wrong Board Selected:**
   - Tools > Board > Select correct Arduino model

2. **Wrong Port Selected:**
   - Tools > Port > Select correct COM port
   - Windows: COM3, COM4, etc.
   - Mac/Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.

3. **Arduino Not Recognized:**
   - Install/update USB drivers
   - Try different USB cable
   - Try different USB port

4. **Bootloader Issue:**
   - Upload to Arduino while holding reset button
   - Release reset when "Uploading..." appears

---

## Power Supply Issues

### Power Supply Shuts Off

**Cause:** Overcurrent protection triggered

**Solutions:**
1. Check pump current requirements
2. Ensure pump isn't mechanically bound/stuck
3. Upgrade to higher-current supply
4. Look for short circuits in wiring

### Voltage Drops Under Load

**Cause:** Inadequate supply or thin wires

**Solutions:**
1. Use thicker wire gauge (see PARTS_LIST.md)
2. Shorten power wire length
3. Upgrade power supply capacity
4. Add bulk capacitance

---

## Code/Upload Issues

### Compilation Errors

**Error: 'tachISR' was not declared**
**Solution:** Ensure ISR is defined before attachInterrupt()

**Error: Timer1 errors**
**Solution:** Some boards don't support Timer1 modification - remove or comment out TCCR1B lines

### Runtime Issues

**Pump doesn't ramp smoothly**
**Solution:** Increase RAMP_DELAY value in code

**Serial commands don't work**
**Solution:** 
- Check baud rate matches (9600)
- Send with newline character (enabled in Serial Monitor)
- Trim whitespace in command processing

---

## Advanced Diagnostics

### Using Oscilloscope

If you have access to an oscilloscope:

1. **Check PWM Signal at Pin 9:**
   - Should see square wave
   - Frequency: ~490Hz to 4kHz
   - Voltage: 0V to 5V
   - Duty cycle changes with speed

2. **Check Signal at Pump:**
   - Verify PWM reaches green wire
   - Same characteristics as Pin 9
   - If different, check for wiring issues

### Using Logic Analyzer

1. **Monitor PWM output:**
   - Capture PWM waveform
   - Verify duty cycle matches code
   - Check for glitches or interruptions

### Multimeter Testing

1. **DC Voltage Mode:**
   - Measure 24V at pump power wires: Should be ~24V
   - Measure Arduino Pin 9 (averaged): Should vary 0-5V with PWM

2. **Continuity Mode:**
   - Check GND connections: Should beep
   - Check for shorts: Should NOT beep between power and signal

3. **Current Mode (if available):**
   - Measure pump current draw
   - Compare to specifications
   - High current = mechanical problem or short

---

## When to Seek Professional Help

Contact pump manufacturer or electrical engineer if:

- Pump overheats quickly (>1 minute at low speed)
- Burning smell from pump or ESC
- Physical damage to pump or wires
- Current draw exceeds specifications significantly
- Multiple troubleshooting attempts unsuccessful

## Safety Warnings

⚠️ **STOP IMMEDIATELY if you observe:**
- Smoke from any component
- Burning smell
- Sparks or arcing
- Very hot components (too hot to touch)
- Unusual sounds (grinding, scraping)

🔴 **Disconnect power before:**
- Modifying any wiring
- Touching any connections
- Moving components
- Troubleshooting physical issues

## Getting Additional Help

### Information to Provide When Asking for Help:

1. **Pump Information:**
   - Model number
   - Specifications (voltage, current)
   - Wire colors and count

2. **Your Setup:**
   - Arduino model
   - Power supply specifications
   - Wiring diagram or photos
   - Code being used

3. **Symptoms:**
   - Exact behavior observed
   - What works / what doesn't
   - Error messages
   - Test results from this guide

4. **What You've Tried:**
   - List troubleshooting steps completed
   - Results of each test

### Resources:
- Arduino Forums: https://forum.arduino.cc
- Arduino Stack Exchange
- Electronics Stack Exchange
- Reddit: r/arduino, r/AskElectronics

---

## Success Indicators

Your system is working correctly when:

✅ Pump speed responds to PWM changes  
✅ Operation is smooth and consistent  
✅ No excessive heat or noise  
✅ Serial Monitor shows expected output  
✅ Power supply voltage stays stable  
✅ No error messages or warnings  
✅ System can run for extended periods  

If all indicators are positive, congratulations - your system is operating correctly!
