#include "FastLED.h"
#define DEBUG
//___________________________________PINS_________________________________
#define WS28_PIN 5
#define LED_PIN 2
#define Relais_PIN 13
#define Zuluft_PIN 4
#define Abluft_PIN 3
#define NTC_ambient A5
#define NTC_Netzteil A3
#define Voltage_PIN A4
#define Taster_PIN A0
//__________________________________VALUES______________________________
#define NUM_LEDS 9
#define NUM_Filter 10                 // minimal 3 

float BattMin = 3.5;                  // min Zellspnnung
float KFZmin = 11.5;
float BatteryOffset = .5;             // in mV
int BatteryScale  = 950;
int schwelle_temp_Ambient = 35;       // Lüftung einschalt temeperatur Ambiente
int schwelle_temp_Netzteil = 32;      // Lüftung einschalt temperatur netzteil
int time_LED_an = 1100;
int time_LED_aus = 70;              // Ein und Aus Zeit der grün 5mm LED
int Faktor_temp_LED = 7;
int Faktor_Bat_LED = 250;

//___________________________________Setup_______________________________
CRGB leds[NUM_LEDS];

int temp_Ambient = 0;
int temp_Netzteil = 0;
float voltage = 0;
int voltage_in = 0;
int voltage_filter[NUM_Filter];
int temp_ambient_filter[NUM_Filter];
int temp_Netzteil_filter[NUM_Filter];
int cells = 0;
float cell_V = 3.8;
float V_cell = 0;
char Luft_min = 100;
char inV = 50;                // Faktor Spannungsteiler
int Abluft = 0;
bool Zuluft = false;
char Battcheck = 0;
long current_time;
long set_time1 = 0;
long set_time2 = 0;
float cell_Vdiff = 0;
int i = 0;
int GRB_Temp_Netz[3];
int GRB_Temp_Ambi[3];
int GRB_Relais[3];
int GRB_VBat[3];
int BattLow_Abschaltung = 0;
int cell = 0;
bool Battery = false;
bool BatteryLow = false;
bool relais = false;
bool grLED = true;
bool Battery_init = false;
bool Taster = false;
bool Taster_alt = false;
bool KFZ = false;

void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, WS28_PIN, RGB>(leds, NUM_LEDS);                 // WS28 Init

  pinMode(WS28_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(Relais_PIN, OUTPUT);
  pinMode(Zuluft_PIN, OUTPUT);
  pinMode(Abluft_PIN, OUTPUT);
  pinMode(NTC_ambient, INPUT);
  pinMode(NTC_Netzteil, INPUT);
  pinMode(Voltage_PIN, INPUT);
  pinMode(Taster_PIN, INPUT);

  leds[0] = CRGB(255, 255, 255);                                // WS28 show init
  leds[1] = CRGB(255, 255, 255);
  leds[2] = CRGB(255, 255, 255);
  leds[3] = CRGB(255, 255, 255);
  leds[4] = CRGB(255, 255, 255);
  leds[5] = CRGB(255, 255, 255);
  leds[6] = CRGB(255, 255, 255);
  leds[7] = CRGB(255, 255, 255);
  leds[8] = CRGB(255, 255, 255);

  FastLED.show();
  delay(300);
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop() {
  // ____________________________________________ EINLESEN _____________________________________________________
  temp_Ambient = analogRead(NTC_ambient) / 10;
  temp_Netzteil = analogRead(NTC_Netzteil) / 10;
  voltage_in = analogRead(Voltage_PIN);
  int Tast = analogRead(Taster_PIN);

  //_________________________________________________Taster und Umschaltung_____________________________________

  if (Tast > 500) {
    Taster = true;
  } else {
    Taster = false;
  }
  if (Taster != Taster_alt) {
    if (Taster == true) {
      if (KFZ == false) {
        KFZ = true;
      } else {
        KFZ = false;
      }
    }
    Taster_alt = Taster;
  }
  //______________________________________________BATTERIE______________________________________________________

  voltage_filter[i] = voltage_in;                              // Lowpass filter
  i++;
  if (i == NUM_Filter - 1) {
    i = 0;
    if (Battery == true) Battery_init = true;
  }

  for (int j = 0; j < NUM_Filter - 1; j++) {
    voltage_in += voltage_filter[j];
  }
  voltage_in /= NUM_Filter;

  if (voltage_in > 10) {                                       // Batterie erkennung
    Battery = true;
    voltage = voltage_in * 4.887;
    voltage = voltage / 50;
    voltage = voltage - BatteryOffset;
    voltage = voltage / 1000 * BatteryScale;
  } else {
    Battery = false;
    Battery_init = false;
  }

  if (Battery == true && KFZ == false) {        // Batterie Berechnung
    if (KFZ == false) {
      cells = abs(voltage / cell_V);
      V_cell = voltage / cells;
      if (V_cell > 4.3) {
        cells++;
        V_cell = voltage / cells;
      }
    }
    if (cell == 0)cell = cells;
    if (V_cell < BattMin) BatteryLow = true;                      // Bat-leer mit Hysterese
    if (V_cell > BattMin + 0.25) BatteryLow = false;
  } else if (Battery == true && KFZ == true) {
    if (voltage < KFZmin) BatteryLow = true;
    if (voltage > KFZmin + 0.7) BatteryLow = false;
  } else {
    cells = 0;
    cell = 0;
    V_cell = 0;
    voltage = 0;
  }

  if (BatteryLow == true && Battery == true) {
    BattLow_Abschaltung = 255;
  } else {
    BattLow_Abschaltung = 0;
  }

  if (KFZ == false) {
    cell_Vdiff = V_cell - BattMin;
    if (cell_Vdiff < 0) cell_Vdiff = 0;

    GRB_VBat[2] = 0;    // B
    GRB_VBat[0] = Faktor_Bat_LED * cell_Vdiff;
    if (GRB_VBat[0] >= 250 ) GRB_VBat[0] = 250;
    if (GRB_VBat[0] <= 0 ) GRB_VBat[0] = 0;
    GRB_VBat[1] = 250 - GRB_VBat[0];
  } else {
    GRB_VBat[2] = 20;    // B
    GRB_VBat[0] = 0;
    GRB_VBat[1] = 0;
    cells = 5;
  }

  //_______________________________________Abluft Steuerung ________________________________________________________

  temp_Netzteil_filter[i] = temp_Netzteil;                              // Lowpass filter

  for (int j = 0; j < NUM_Filter - 1; j++) {
    temp_Netzteil += temp_Netzteil_filter[j];
  }
  temp_Netzteil /= NUM_Filter;

  if (temp_Netzteil >= schwelle_temp_Netzteil) {
    int temp = temp_Netzteil - schwelle_temp_Netzteil;
    Abluft = 100 + temp * 7;
  } else {
    Abluft = 0;
  }
  if ( temp_Ambient > temp_Netzteil && Zuluft == true) {
    Abluft = 255;
  }
  GRB_Temp_Netz[0] = 0;   //G
  GRB_Temp_Netz[1] = Faktor_temp_LED * (temp_Netzteil - 25);
  if (GRB_Temp_Netz[1] > 250) GRB_Temp_Netz[1] = 250;
  if (GRB_Temp_Netz[1] < 5) GRB_Temp_Netz[1] = 0;
  GRB_Temp_Netz[2] = 250 - GRB_Temp_Netz[1];

  //______________________________________Zuluft Steuerung_________________________________________________________

  temp_ambient_filter[i] = temp_Ambient;                              // Lowpass filter

  for (int j = 0; j < NUM_Filter - 1; j++) {
    temp_Ambient += temp_ambient_filter[j];
  }
  temp_Ambient /= NUM_Filter;

  if (temp_Ambient >= schwelle_temp_Ambient) {
    Zuluft = true;
  }
  if (temp_Ambient <= schwelle_temp_Ambient - 3) {
    Zuluft = false;
  }
  if (temp_Netzteil >= 40) {
    Zuluft = true;
  }

  GRB_Temp_Ambi[0] = 0;
  GRB_Temp_Ambi[1] = Faktor_temp_LED * (temp_Ambient - 25);
  if (GRB_Temp_Ambi[1] > 250) GRB_Temp_Ambi[1] = 250;
  if (GRB_Temp_Ambi[1] < 5) GRB_Temp_Ambi[1] = 0;
  GRB_Temp_Ambi[2] = 250 - GRB_Temp_Ambi[1];

  //___________________________________________RELAIS_______________________________________________________________
  if (Battery == true && BatteryLow == false && Battery_init == true) {
    Battcheck ++;
    if (Battcheck >= 40) Battcheck = 40;
    if (Battcheck > 30) {
      relais = true;
      GRB_Relais[0] = 0;
      GRB_Relais[1] = 0;
      GRB_Relais[2] = 250;
    }
  } else {
    Battcheck --;
    if (Battcheck <= 0) Battcheck = 0;
    if (Battcheck < 5) {
      relais = false;
      GRB_Relais[0] = 80;   // G
      GRB_Relais[1] = 80;   // R
      GRB_Relais[2] = 80;   // B
    }
  }

  //_____________________________________Serial Ausgabe ____________________________________________________________
 #ifdef DEBUG
  if (i == NUM_Filter - 2) {

    Serial.print(temp_Ambient);
    Serial.print("  ");
    Serial.print(temp_Netzteil);
    Serial.print("  ");
    //  Serial.print(voltage_in);
    //  Serial.print("  ");
    //  Serial.print(GRB_VBat[0]);
    //  Serial.print("  ");
    //  Serial.print(GRB_VBat[1]);
    //  Serial.print("  ");
    //  Serial.print(cell_Vdiff);
    //  Serial.print("  ");
      Serial.print(voltage);
      Serial.print("  ");
    //  Serial.print(Abluft);
    //  Serial.print("  ");
    //  Serial.print(Zuluft);
    //  Serial.print("  ");
    //  Serial.print(relais);
    //  Serial.print(" r ");
    Serial.print(cells);
    Serial.print("  ");
    Serial.print(V_cell);
    Serial.println("");
  }
 #endif
  //________________________________________________Ausgabe ____________________________________________________
  LED();
  LUFT();
  RELAIS();
  //________________________________________________Heartbeat LED ______________________________________________
  current_time = millis();
  if (current_time > set_time1 + time_LED_an) {
    set_time1 = current_time;
    set_time2 = current_time;
    digitalWrite(LED_PIN, HIGH);
    grLED = false;
  }
  if (current_time > set_time2 + time_LED_aus) {
    digitalWrite(LED_PIN, LOW);
    grLED = true;
  }

}
//____________________________________________________WS28 Ausgabe____________________________________________
void LED() {

  leds[0] = CRGB(GRB_Temp_Netz[0], GRB_Temp_Netz[1], GRB_Temp_Netz[2]);     // Temp Netzteil
  leds[1] = CRGB(GRB_Temp_Ambi[0], GRB_Temp_Ambi[1], GRB_Temp_Ambi[2]);     // Temp Ambiente
  leds[2] = CRGB(GRB_Relais[0], GRB_Relais[1], GRB_Relais[2]);              // ST Relais
  leds[3] = CRGB(0, BattLow_Abschaltung, 0);
  if (cells > 4) {
    leds[4] = CRGB(GRB_VBat[0], GRB_VBat[1], GRB_VBat[2]);   // V Bat wenn Zelle 5
  } else {
    leds[4] = CRGB(0, 0, 0);
  }
  if (cells > 3) {
    leds[5] = CRGB(GRB_VBat[0], GRB_VBat[1], GRB_VBat[2]);   // V Bat wenn Zelle 4
  } else {
    leds[5] = CRGB(0, 0, 0);
  }
  if (cells > 2) {
    leds[6] = CRGB(GRB_VBat[0], GRB_VBat[1], GRB_VBat[2]);   // V Bat wenn Zelle 3
  } else {
    leds[6] = CRGB(0, 0, 0);
  }
  if (cells > 1) {
    leds[7] = CRGB(GRB_VBat[0], GRB_VBat[1], GRB_VBat[2]);   // V Bat wenn Zelle 2
  } else {
    leds[7] = CRGB(0, 0, 0);
  }
  if (cells > 0) {
    leds[8] = CRGB(GRB_VBat[0], GRB_VBat[1], GRB_VBat[2]);   // V Bat wenn Zelle 1
  } else {
    leds[8] = CRGB(0, 0, 0);
  }
  FastLED.show();
}
//_____________________________________________________LUFT Ausgabe_______________________________
void LUFT()  {
  if (Abluft == 0) {
    analogWrite(Abluft_PIN, 0);
  } else {
    if (Abluft >= 255) Abluft = 255;
    analogWrite(Abluft_PIN, Abluft);
  }
  if (Zuluft == false) {
    digitalWrite(Zuluft_PIN, LOW);
  } else {
    digitalWrite(Zuluft_PIN, HIGH);
  }
}
//__________________________________________________Relais Ausgabe_____________________________
void RELAIS() {
  if (relais == true) {
    digitalWrite(Relais_PIN, HIGH);
  } else {
    digitalWrite(Relais_PIN, LOW);
  }
}
// ____________________________________________ENDE___________________________________________
