(1001)
(Bearbeiten)
(  Hersteller: ChinaCnc)
(  model: 3040T)
(  description: Generic 3-axis)
(T5 D=3.175 CR=0 - ZMIN=-1.5 - Schaftfrser)
G90 G94
G17
G21
(-Attention- Property Safe Retracts is set to Clearance Height.)
(Ensure the clearance height will clear the part and or fixtures.)
(Raise the Z-axis to a safe height before starting the program.)
(When using Fusion 360 for Personal Use, the feedrate of)
(rapid moves is reduced to match the feedrate of cutting)
(moves, which can increase machining time. Unrestricted rapid)
(moves are available with a Fusion 360 Subscription.)

(2D-Kontur1)
M0
(MANUAL TOOL CHANGE TO T5)
S6000 M3
G17 G90 G94
G54
G0 X0.318 Y-7.54
Z15
G1 Z4.7 F1000
Z0.335 F333.3
Z-1.183
G19 G3 Y-7.222 Z-1.5 J0.318 K0 F1000
G1 Y-6.905
G17 G3 X0 Y-6.588 I-0.317 J0
G2 X0 Y6.588 I0 J6.588
X0 Y-6.588 I0 J-6.588
G3 X-0.317 Y-6.905 I0 J-0.318
G1 Y-7.222
G19 G2 Y-7.54 Z-1.183 J0 K0.317
G1 Z15
G17
M5
M30
