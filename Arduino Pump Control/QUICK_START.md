# Quick Start Guide - Arduino Pump Control

This guide will help you get your 4-wire brushless pump running with Arduino in under 30 minutes.

## Before You Begin

⚠️ **Safety First:**
- Work in a well-ventilated area
- Wear safety glasses
- Never connect 24V to Arduino pins
- Ensure all connections are secure before powering on
- Have a way to quickly disconnect power (emergency shutoff)

## What You Need

✅ Arduino board (Uno, Nano, or Mega)  
✅ 24V DC power supply (rated for your pump's current)  
✅ Your 4-wire brushless pump  
✅ Jumper wires or connection wire  
✅ USB cable to program Arduino  
✅ Computer with Arduino IDE installed  

Optional: Multimeter for testing

## Step 1: Install Arduino IDE (5 minutes)

1. Download Arduino IDE from https://www.arduino.cc/en/software
2. Install the software on your computer
3. Open Arduino IDE

## Step 2: Wire the Connections (10 minutes)

**⚠️ DO NOT POWER ANYTHING ON YET!**

### Connection Order:

1. **Arduino to Computer:**
   - Connect Arduino to your computer via USB (this powers the Arduino)

2. **Power Supply Setup:**
   - Connect RED wire from pump to +24V terminal
   - Connect BLACK wire from pump to GND terminal
   - **DO NOT TURN ON THE POWER SUPPLY YET**

3. **Common Ground (CRITICAL!):**
   - Use a jumper wire to connect Arduino GND to Power Supply GND
   - This is the most important connection!

4. **Control Signal:**
   - Connect GREEN wire from pump to Arduino Pin 9

5. **Optional Speed Feedback:**
   - Connect WHITE wire from pump to Arduino Pin 8 (skip if unsure)

### Visual Check:
```
✓ Red wire    → 24V Power Supply (+)
✓ Black wire  → 24V Power Supply (-)
✓ Green wire  → Arduino Pin 9
✓ White wire  → Arduino Pin 8 (optional)
✓ Wire from Arduino GND → Power Supply (-)
✓ Arduino connected to computer via USB
```

## Step 3: Upload the Code (5 minutes)

1. Open Arduino IDE
2. Copy the code from `simple_pump_control.ino`
3. In Arduino IDE, go to **Tools > Board** and select your Arduino model
4. Go to **Tools > Port** and select the correct COM port
5. Click the **Upload** button (→ arrow icon)
6. Wait for "Done uploading" message

## Step 4: Test the System (10 minutes)

1. **Open Serial Monitor:**
   - In Arduino IDE, click the magnifying glass icon (top right)
   - Set baud rate to 9600 at the bottom

2. **Power On:**
   - Turn on your 24V power supply
   - You should see messages in the Serial Monitor

3. **First Test:**
   - The pump should start ramping up slowly
   - Serial Monitor shows speed percentages
   - Listen/watch for pump operation

4. **If Nothing Happens:**
   - Check all connections
   - Verify common ground is connected
   - Check that 24V is present at pump red/black wires with multimeter
   - Review troubleshooting section below

## Step 5: Next Steps

Once basic operation is confirmed:

1. **Experiment with different speeds:**
   - Upload `pump_control.ino` for serial control
   - Type speed values (0-100) in Serial Monitor

2. **Add manual control:**
   - Connect a potentiometer (see `advanced_pump_control.ino`)
   - Physical dial to control speed

3. **Monitor performance:**
   - Check if white wire provides speed feedback
   - Monitor RPM in Serial Monitor

## Troubleshooting

### Pump Doesn't Run At All

**Check 1: Power Supply**
- Is 24V supply turned on?
- Use multimeter to verify 24V at pump red/black wires
- Solution: Verify power connections

**Check 2: Common Ground**
- Is Arduino GND connected to Power Supply GND?
- This is the #1 cause of "no response"
- Solution: Double-check ground connection

**Check 3: Control Signal**
- Is green wire connected to Pin 9?
- Is correct sketch uploaded to Arduino?
- Solution: Re-check wiring and re-upload code

### Pump Runs at Full Speed Always

**Possible Causes:**
- Green wire might not be control signal
- Pump may require external ESC
- Different control protocol needed

**Solutions:**
- Try swapping green and white wires
- Consider external ESC (see WIRING_DIAGRAM.md)
- Consult pump manufacturer

### Erratic Operation / Pump Stutters

**Possible Causes:**
- Inadequate power supply current
- Loose connections
- Electrical noise

**Solutions:**
- Verify power supply can handle pump current
- Check all connections are tight
- Add 100µF capacitor across 24V supply terminals
- Keep PWM wires away from power wires

### Arduino Resets When Pump Starts

**Cause:**
- Voltage drop from power supply
- Noise from motor affecting Arduino

**Solutions:**
- Use separate power supply for Arduino (not from 24V supply)
- Add filtering capacitor to power supply
- Ensure USB cable is providing good power

### Getting Help

If you're still having issues:
1. Take photos of your wiring
2. Note what IS working (LEDs, Serial Monitor messages, etc.)
3. Note what ISN'T working
4. Check the detailed troubleshooting in WIRING_DIAGRAM.md
5. Post to Arduino forums with:
   - Your pump specifications
   - Photos of wiring
   - Description of behavior

## Safety Reminders

- 🔴 **Never connect 24V to Arduino pins** - Use only for pump power
- 🟡 **Always connect common ground** - Required for PWM to work
- 🟢 **Start with low speeds** - Test at 20-30% before full speed
- 🔵 **Monitor temperature** - Pump and ESC (if used) shouldn't get too hot
- ⚫ **Have emergency shutoff** - Be able to quickly cut power if needed

## What to Expect

**Normal Operation:**
- Pump makes humming/whirring sound
- Speed increases smoothly as PWM increases
- No excessive vibration
- Moderate warmth (not hot) after running

**Abnormal - Stop Immediately:**
- Burning smell
- Excessive heat
- Loud grinding noises
- Smoke
- Sparks

## Success Checklist

- [ ] Arduino IDE installed
- [ ] Wiring completed and verified
- [ ] Common ground connected
- [ ] Code uploaded successfully
- [ ] Serial Monitor shows messages
- [ ] Pump responds to PWM signals
- [ ] System operates smoothly
- [ ] No overheating

## Next Learning Resources

- **pump_control.ino** - Serial command control
- **advanced_pump_control.ino** - Multiple control modes
- **WIRING_DIAGRAM.md** - Detailed wiring schematics
- **PARTS_LIST.md** - Shopping guide for additional components

Congratulations! You now have Arduino control of your brushless pump! 🎉
