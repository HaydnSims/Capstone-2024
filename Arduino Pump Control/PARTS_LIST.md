# Parts List and Shopping Guide

## Required Components

### 1. Arduino Board
**Options:**
- Arduino Uno (Recommended for beginners) - ~$25
- Arduino Nano (Compact version) - ~$15
- Arduino Mega (More pins if expanding) - ~$40

**Where to Buy:** Arduino.cc, Amazon, Adafruit, SparkFun

### 2. 24V DC Power Supply
**Specifications:**
- Output: 24VDC
- Current: Check your pump's datasheet (typically 2-5A for small pumps)
- Safety: Look for UL/CE certified supplies

**Recommendations:**
- Mean Well LRS-100-24 (100W, 4.5A) - ~$20-30
- Generic 24V 2A adapter - ~$15-20

**Where to Buy:** Amazon, Digi-Key, Mouser, eBay

### 3. Electronic Speed Controller (ESC) - If Needed
**Only required if pump doesn't have built-in driver electronics**

**Specifications:**
- Input voltage: 24V DC compatible
- Type: Brushless DC motor ESC
- Current rating: Match or exceed pump requirements

**Recommendations:**
- BLDC ESC rated for 24V - ~$15-30
- Check for PWM input capability (1-2 kHz)

**Where to Buy:** HobbyKing, Amazon, Banggood (drone/RC suppliers)

### 4. Jumper Wires / Connection Wire
**Specifications:**
- Male-to-Male jumper wires for breadboard
- 22-24 AWG solid wire for Arduino connections
- 16-18 AWG stranded wire for power connections (depending on current)

**Quantity:** 
- Pack of jumper wires (~40 pieces)
- Few feet of 18 AWG wire

**Cost:** ~$5-10

**Where to Buy:** Amazon, Adafruit, SparkFun

### 5. Breadboard (Optional but Recommended)
**For prototyping:**
- 830 point breadboard - ~$5-8
- Helps with temporary connections before soldering

**Where to Buy:** Amazon, Adafruit, SparkFun

### 6. USB Cable
**For Arduino Programming:**
- USB A to B cable (for Uno/Mega)
- USB A to Mini-B (for Nano)
- Usually included with Arduino

### 7. Multimeter (Highly Recommended)
**For troubleshooting:**
- Digital multimeter (DMM) - ~$15-30
- Essential for checking voltages and continuity

**Where to Buy:** Amazon, Harbor Freight, hardware stores

## Optional Components

### 8. Potentiometer (For Manual Speed Control)
**Specifications:**
- 10kΩ linear taper
- Can be any value 1kΩ-100kΩ

**Cost:** ~$1-2

**Where to Buy:** Amazon, Adafruit, SparkFun, Digi-Key

### 9. Emergency Stop Button
**Specifications:**
- Normally closed (NC) push button
- Or normally open (NO) - requires code adjustment

**Cost:** ~$2-5

**Where to Buy:** Amazon, Adafruit, SparkFun

### 10. Power Filtering Capacitor
**For stable operation:**
- 100-470µF electrolytic capacitor
- Voltage rating: 35V or higher
- Helps reduce electrical noise

**Cost:** ~$0.50-2

**Where to Buy:** Digi-Key, Mouser, Amazon

### 11. Heat Sinks (If ESC gets hot)
**For thermal management:**
- Small aluminum heat sinks
- Thermal adhesive tape

**Cost:** ~$5-10

### 12. Project Enclosure
**For permanent installation:**
- Plastic project box
- Size depends on your layout

**Cost:** ~$5-15

**Where to Buy:** Amazon, Adafruit, Hammond Manufacturing

## Wire Gauge Reference

| Current Draw | Recommended Wire Gauge | Notes |
|--------------|------------------------|-------|
| < 1A | 22-24 AWG | Signal wires, Arduino connections |
| 1-3A | 18-20 AWG | Small pumps, low power |
| 3-5A | 16-18 AWG | Medium pumps |
| 5-10A | 14-16 AWG | Large pumps, high power |

## Total Cost Estimate

**Basic Setup (Arduino + Power Supply):**
- Minimum: $40-50
- Recommended: $60-80 (includes multimeter, extra components)

**With ESC (if needed):**
- Add $15-30

**Full Setup with Optional Components:**
- $80-120 (includes everything for a robust system)

## Tools Needed

### Required:
- None (if using breadboard/jumper wires)

### Recommended:
- Wire strippers - ~$10
- Small screwdriver set - ~$10
- Soldering iron (for permanent connections) - ~$20-40
- Solder and flux - ~$5-10

## Safety Equipment

- Safety glasses (when working with power)
- ESD wrist strap (optional, protects Arduino) - ~$5

## Where to Buy - Quick Reference

### Online Retailers:
1. **Amazon** - Fast shipping, good for power supplies and basic components
2. **Adafruit** - Quality components, great tutorials
3. **SparkFun** - Similar to Adafruit
4. **Digi-Key** - Huge selection, technical components
5. **Mouser Electronics** - Similar to Digi-Key
6. **AliExpress/Banggood** - Cheaper but slower shipping (2-4 weeks)

### Local Options:
- RadioShack (if still available in your area)
- Micro Center (if available)
- Local electronics stores

## Pump Compatibility Notes

Your pump specifications:
- ✅ 24VDC - Power supply must match
- ✅ Brushless motor - ESC may be needed
- ✅ 4-wire configuration - Red/Black (power), Green/White (control/feedback)
- ⚠️ "Without integrated electronics" - Likely needs external ESC

**Important:** Before ordering an ESC, try connecting the green wire directly to Arduino Pin 9 with the basic sketch. Some pumps labeled "without integrated electronics" actually have simple PWM control built-in.

## Next Steps After Shopping

1. Start with the `simple_pump_control.ino` sketch
2. Test at low speeds (20-30%) first
3. Monitor temperature of pump and ESC (if used)
4. Gradually increase speed as you verify proper operation
5. Add optional components as needed (potentiometer, emergency stop, etc.)
