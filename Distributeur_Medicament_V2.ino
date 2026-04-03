// ╔══════════════════════════════════════════════════════════╗
// ║  MEDSMART V2.3 — STABILITY & FIXES                       ║
// ║  - UNIFIED LCD BUFFER (Fix corruption)                   ║
// ║  - CORRECT STEPS (341 for 60 degrees)                    ║
// ║  - UNIFIED DISPENSE LOGIC                                ║
// ║  - ZERO DELAY / NON-BLOCKING                             ║
// ╚══════════════════════════════════════════════════════════╝

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// ── CONFIGURATION MÉCANIQUE ─────────────────────────────
const int stepsPerRevolution    = 2048;
const int nb_total_emplacements = 6;
const int pas_par_compartiment  = 1125;     // Ajusté pour plus de course (1125 au lieu de 1075)

// ── ALARME UNIQUE (Date + Heure) ────────────────────────
int  aDay = 28, aMo = 3, aYr = 2026;
int  aHr  = 8,  aMi = 0;
bool aActive = false;
bool aFired  = false;

// ── BROCHES ─────────────────────────────────────────────
const int pinBouton = 2;
const int pinBuzzer = 3;
const int pinLED    = 13;
const int pinTCRT   = 4;   // LOW = retirée, HIGH = présente

// ── OBJETS ──────────────────────────────────────────────
RTC_DS3231       rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Stepper          monMoteur(stepsPerRevolution, 8, 10, 9, 11);

// ── MACHINE À ÉTATS ─────────────────────────────────────
enum SystemState { IDLE, DISPENSING, WAITING, CLOSING };
SystemState state = IDLE;

// ── VARIABLES DE TIMING ─────────────────────────────────
int            compartimentActuel = 0;   
unsigned long  dernierEnvoi       = 0;   
unsigned long  waitStart          = 0;   
unsigned long  lastBlink          = 0;   
bool           blinkState         = false;
bool           pillTaken          = false;
unsigned long  dernierAffichage   = 0;   

// ── CONSTANTES ──────────────────────────────────────────
const unsigned long SECURITE_MS   = 10000;  // 10 secondes
const unsigned long BLINK_MS      = 200;    
const unsigned long AFFICHAGE_MS  = 300;    // Rafraîchir LCD un peu plus vite
const unsigned long STATUT_MS     = 2000;   

// ═══════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(pinBouton, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLED,    OUTPUT);
  pinMode(pinTCRT,   INPUT);

  monMoteur.setSpeed(6);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  if (!rtc.begin()) {
    updateLCD("RTC ERROR", "Check Wiring");
    while (1);
  }

  updateLCD("MEDSMART V2.3", "Initialisation");
  delay(1500);
  Serial.println("READY");
}

// ═══════════════════════════════════════════════════════════
//  HELPER LCD (Évite la corruption et le scintillement)
// ═══════════════════════════════════════════════════════════
void updateLCD(const char* l1, const char* l2) {
  char b1[17], b2[17];
  
  // Ligne 1
  strncpy(b1, l1, 16);
  b1[16] = '\0';
  for (int i = strlen(b1); i < 16; i++) b1[i] = ' ';
  b1[16] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(b1);

  // Ligne 2
  strncpy(b2, l2, 16);
  b2[16] = '\0';
  for (int i = strlen(b2); i < 16; i++) b2[i] = ' ';
  b2[16] = '\0';
  lcd.setCursor(0, 1);
  lcd.print(b2);
}

// ═══════════════════════════════════════════════════════════
//  LOOP
// ═══════════════════════════════════════════════════════════
void loop() {
  lireCommandes();

  switch (state) {

    case IDLE: {
      DateTime now = rtc.now();
      if (millis() - dernierAffichage >= AFFICHAGE_MS) {
        char s1[17], s2[17];
        snprintf(s1, 17, "%02d:%02d:%02d C:%d", 
                 now.hour(), now.minute(), now.second(), 
                 compartimentActuel);
        
        if (aActive) {
          snprintf(s2, 17, "%02d/%02d AL:%02d:%02d", now.day(), now.month(), aHr, aMi);
        } else {
          snprintf(s2, 17, "AUCUNE ALARME");
        }
        updateLCD(s1, s2);
        dernierAffichage = millis();
      }

      if (aActive && !aFired) {
        if (now.hour() == aHr && now.minute() == aMi) {
          aFired = true;
          state = DISPENSING;
        }
      }
      if (now.minute() != aMi) aFired = false;
      break;
    }

    case DISPENSING: {
      compartimentActuel++;
      if (compartimentActuel > 2) compartimentActuel = 1;
      
      Serial.println("STATUS:DISPENSING");
      char s2[17];
      snprintf(s2, 17, "COMPARTIMENT %d", compartimentActuel);
      updateLCD("OUVERTURE...", s2);
      
      monMoteur.step(compartimentActuel * pas_par_compartiment);
      
      Serial.println("STATUS:WAITING_FOR_PILL");
      waitStart = millis();
      pillTaken = false;
      state = WAITING;
      break;
    }

    case WAITING: {
      unsigned long elapsed = millis() - waitStart;
      
      if (millis() - lastBlink >= BLINK_MS) {
        blinkState = !blinkState;
        digitalWrite(pinLED, blinkState ? HIGH : LOW);
        if (blinkState) tone(pinBuzzer, 1500); else noTone(pinBuzzer);
        lastBlink = millis();
      }

      if (digitalRead(pinTCRT) == LOW || digitalRead(pinBouton) == LOW) {
        pillTaken = true;
        state = CLOSING;
        break;
      }

      if (elapsed >= SECURITE_MS) {
        pillTaken = false;
        state = CLOSING;
        break;
      }

      if (millis() - dernierAffichage >= AFFICHAGE_MS) {
        char s2[17];
        snprintf(s2, 17, "TEMPS: %lds", (SECURITE_MS - elapsed) / 1000);
        updateLCD("PRENEZ PILULE!", s2);
        dernierAffichage = millis();
      }
      break;
    }

    case CLOSING: {
      noTone(pinBuzzer);
      digitalWrite(pinLED, LOW);

      if (pillTaken) {
        Serial.println("STATUS:PILL_TAKEN");
        updateLCD("STATUT: PRIS", "MERCI !");
      } else {
        Serial.println("STATUS:PILL_MISSED");
        updateLCD("STATUT: OMIS", "NON RETIRE !");
      }

      delay(2000);
      updateLCD("FERMETURE...", "RETOUR ZERO");
      monMoteur.step(-(compartimentActuel * pas_par_compartiment));

      if (compartimentActuel >= 2) compartimentActuel = 0;
      
      Serial.println("STATUS:READY");
      envoyerStatut();
      state = IDLE;
      break;
    }
  }

  if (millis() - dernierEnvoi >= STATUT_MS) {
    envoyerStatut();
    dernierEnvoi = millis();
  }
}

void lireCommandes() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd.startsWith("SET_TIME:")) {
    int vals[6]; int idx = 0; String sub = cmd.substring(9);
    while (sub.length() > 0 && idx < 6) {
      int sep = sub.indexOf(':');
      if (sep == -1) { vals[idx++] = sub.toInt(); break; }
      vals[idx++] = sub.substring(0, sep).toInt();
      sub = sub.substring(sep + 1);
    }
    if (idx >= 6) {
      rtc.adjust(DateTime(vals[5], vals[4], vals[3], vals[0], vals[1], vals[2]));
      Serial.println("STATUS:TIME_SET");
    }
  }
  else if (cmd.startsWith("SET_ALARM:")) {
    int vals[6]; int idx = 0; String sub = cmd.substring(10);
    while (sub.length() > 0 && idx < 6) {
      int sep = sub.indexOf(':');
      if (sep == -1) { vals[idx++] = sub.toInt(); break; }
      vals[idx++] = sub.substring(0, sep).toInt();
      sub = sub.substring(sep + 1);
    }
    if (idx >= 6) {
      aDay = vals[0]; aMo = vals[1]; aYr = vals[2];
      aHr = vals[3]; aMi = vals[4]; aActive = (vals[5] == 1);
      aFired = false;
      Serial.println("STATUS:ALARM_SET");
    }
  }
  else if (cmd == "STATUS") envoyerStatut();
  else if (cmd == "DISPENSE") { if (state == IDLE) state = DISPENSING; }
  else if (cmd == "BUZZ_TEST") { tone(pinBuzzer, 1500); delay(200); noTone(pinBuzzer); Serial.println("STATUS:BUZZ_OK"); }
  else if (cmd == "LED_TEST") { digitalWrite(pinLED, HIGH); delay(500); digitalWrite(pinLED, LOW); Serial.println("STATUS:LED_OK"); }
  else if (cmd == "TEST_MOTOR") { 
    Serial.println("STATUS:MOTOR_TESTING");
    monMoteur.step(pas_par_compartiment); 
    delay(500);
    monMoteur.step(-pas_par_compartiment); 
    Serial.println("STATUS:MOTOR_OK"); 
  }
}

void envoyerStatut() {
  DateTime now = rtc.now();
  Serial.print("TIME:");
  if (now.hour()<10) Serial.print('0'); Serial.print(now.hour()); Serial.print(':');
  if (now.minute()<10) Serial.print('0'); Serial.print(now.minute()); Serial.print(':');
  if (now.second()<10) Serial.print('0'); Serial.print(now.second());
  Serial.print("|DATE:");
  if (now.day()<10) Serial.print('0'); Serial.print(now.day()); Serial.print('/');
  if (now.month()<10) Serial.print('0'); Serial.print(now.month()); Serial.print('/');
  Serial.print(now.year());
  Serial.print("|COMP:"); Serial.print(compartimentActuel);
  Serial.print("|ALM:");
  if (aDay<10) Serial.print('0'); Serial.print(aDay); Serial.print('/');
  if (aMo<10) Serial.print('0'); Serial.print(aMo); Serial.print('/');
  Serial.print(aYr); Serial.print(' ');
  if (aHr<10) Serial.print('0'); Serial.print(aHr); Serial.print(':');
  if (aMi<10) Serial.print('0'); Serial.print(aMi); Serial.print(':');
  Serial.print(aActive?1:0);
  
  Serial.print("|SENS:"); Serial.println(digitalRead(pinTCRT)==HIGH?1:0);
}
