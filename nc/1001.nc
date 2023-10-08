(1001)
(Bearbeiten)
(  Hersteller: ChinaCnc)
(  model: 3040T)
(  description: Generic 3-axis)
(T14 D=1.5 CR=0 - ZMIN=-1.5 - Schaftfrser)
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

(2D-Kontur21)
M0
(MANUAL TOOL CHANGE TO T14)
S6000 M3
G17 G90 G94
G54
G0 X-1.3 Y-4.85
Z15
G1 Z5 F600
Z1 F300
Z-1.35
X-1.307 Z-1.396 F400
X-1.329 Z-1.438
X-1.362 Z-1.471
X-1.404 Z-1.493
X-1.45 Z-1.5
X-1.6
X-1.646 Y-4.857
X-1.688 Y-4.879
X-1.721 Y-4.912
X-1.743 Y-4.954
X-1.75 Y-5
Y-9.449 F600
G2 X-4.931 Y-13.932 I-4.75 J0
G3 X-7.107 Y-17 I1.074 J-3.068
G1 Y-73
G3 X-4.931 Y-76.068 I3.25 J0
G2 X-1.75 Y-80.551 I-1.569 J-4.483
G1 Y-85
G2 X-4.5 Y-87.75 I-2.75 J0
G1 X-45.5
G2 X-48.25 Y-85 I0 J2.75
G1 Y-80.551
G2 X-45.069 Y-76.068 I4.75 J0
G3 X-42.893 Y-73 I-1.074 J3.068
G1 Y-17
G3 X-45.069 Y-13.932 I-3.25 J0
G2 X-48.25 Y-9.449 I1.569 J4.483
G1 Y-5
G2 X-45.5 Y-2.25 I2.75 J0
G1 X-4.5
G2 X-1.75 Y-5 I0 J-2.75
G1 X-1.743 Y-5.046
X-1.721 Y-5.088
X-1.688 Y-5.121
X-1.646 Y-5.143
X-1.6 Y-5.15
X-1.45
X-1.404 Z-1.493
X-1.362 Z-1.471
X-1.329 Z-1.438
X-1.307 Z-1.396
X-1.3 Z-1.35
Z15
M5
M30
