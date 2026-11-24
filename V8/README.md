# V8 — interleaved boost (48–96 V)
Two 470 uH phases, 50 kHz, nominal 180 degree phase shift, same 192/288 W load as V7. VDMOS and thermal parameters inherited. Separate heat sinks assumed. Current limits 10 A per phase, not rated hardware. No hardware isolation; any thermal or electrical trip blocks both PWMs. Optional phase shedding is OFF by default. When enabled in Config it uses 1.5/2.5 A total reference hysteresis and 20 ms dwell. Passive diode paths remain.
 
## Run
Start with V8_OPEN_LOOP.cir (no DLL); check V(carrier1), V(carrier2), V(gate1), V(gate2), V(il1), V(il2), V(itotal), V(out), V(tj1), V(tj2). Verify carrier phase and startup. V8_CLOSED_LOOP_TEMPLATE.cir is explicitly a template, not a runnable closed-loop model until the QSPICE DLL instance is inserted. Create a C++ block in QSPICE, name interleaved_ctrl, all 18 analog pins, compile interleaved_ctrl.cpp, and copy the generated instance line into the template. Keep files together. Port order: vin out il1 il2 ref enable tj1 tj2 duty1 duty2 iref1 iref2 state fault ref_cmd phases limit1 limit2. First 8 inputs, remaining 10 outputs. Use QSPICE-generated syntax rather than guessing it. No native graphical QSCH in this initial V8. 
## Control
Common voltage PI, independent current PI, redistribution of total reference respecting individual thermal limits. ADC 12 bit, IIR alpha .5, PWM 1000 steps, one sample delay, 20 us sampling. Temperature is absolute degrees C, 1 V per C. Derating 110–140 C, both phases latch off at 140 C. No automatic degraded operation after faults. Current samples are simultaneous, not phase-synchronized; solver rollback limitations inherited. Gains provisional; stability not proven.
## Experiments
Change test_normal.inc ambient of phase 1 to 125 C or 145 C for artificial derating/trip. Change load resistance and test profile for low-load shedding. Compare ripple peak-to-peak on identical steady windows (40–50 and 90–100 ms) vs V7, equal mean load power and output voltage. Do not claim ripple cancellation or efficiency until measured. Signed MOS terminal power includes stored energy; average integer cycles. 100 ms is not thermal equilibrium. 
## Tests
g++ -std=c++11 test_interleaved.cpp -o test_interleaved && ./test_interleaved
Source test supplied; not executed in this session. QSPICE compilation and simulation pending. Not for real hardware.
