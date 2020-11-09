// Je nachdem, welcher Drehregeler-Eingang (4,7,8) auf LOW gezogen wird, bestimmt den Gang
// Fußschalter (2) startet den Schleppvorgang
// Wird der Fußschalter länger als PEDALZEITEEPROM betätigt, findet ein Schreibvorgang im EEPROM statt

//Langsam = 80, Mittel = 90, Schnell = 130 --> Werte für Regleransteuerung, eingestellt durch Poti


/*Beschaltung:

  LC-Display: siehe AnagVision Word-Dok.

  Regler:           9
  Seilführung:      10
  Fußschalter:      13

  Drehzahl-Poti:    A0
  Spannungseingang: A1

*/

#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define VERZH 0
#define VERZR 5
#define FUEHRGESCHW 1000 // Je kleiner, desto schneller bewegt sich der Führ-Arm der Trommel
#define LCDDAUER 1000 // Anzeigedauer der Daten in ms
#define PEDALZEITEEPROM 5000 // Zeit, die Pedal betätigt sein muss, damit Start als solcher gezählt wird

Servo Regler;
Servo Spur;

float power = 30;
int cleardisp = 0, fusstaster, u, o, poti, potimap, potianzeige, z = 0, f, x = 0, d = 0, spg, Min = 0, Sek = 0, Stu = 0, piepfertig = 10, startzaehler_aktiv, startzaehler_zeigen, schreibzugriff = 0;
unsigned int startanzahl, momentaneStartzahl;
unsigned long long int myTimer = 0, myTimer2 = 0, myTimer3 = 0, myTimer4 = 0, myTimer5 = 0, AnzahlStarts = 0, istzeitholen = 0;

void setup()
{
  lcd.begin(16, 2);

  pinMode(A0, INPUT); // Poti für max. Drehzahl
  pinMode(A1, INPUT); // Eingang für Batteriespannung
  pinMode(13, INPUT); // Fußtaster
  pinMode(6, INPUT); // Windenstartzähler aktiv/inaktiv (Schalter)
  pinMode(7, INPUT); // Startzählstand anzeigen/ausblenden (Taster)
  pinMode(8, OUTPUT); // Piezo-Summer

  pinMode(13, INPUT_PULLUP); // Pullup am Fußtaster-Eingang aktiv schalten
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);


  Regler.attach(9); // Regler an Ausgang 9
  Spur.attach(10); // Führungs-Servo an Ausgang 10

  Regler.write(30);
  Spur.write(20);

  u = 1;
  o = 0;
  f = 0;
  Serial.begin(115200);

  //showIntro();
  Drehzahlvorwahl_und_bestaetigen();
  
  //EEPROMWriteInt(0, 0x0000); // wird nur verwendet, wenn der EEPROM des Startanzahl-Speichers zurückgesetzt werden soll (erste beide Speicherzellen)
  //EEPROMWriteInt(1, 0x0000); // Achtung! Hier bitte den Controller bis zur loop-Schleife laufen lassen, dass Speicher auch auf 0 gesetzt wird!
}


void loop()
{

  //_________________________________________________________

  BetrZeit_erstellen();
  startzaehler_aktiv = digitalRead(6);
  startzaehler_zeigen = digitalRead(7);

  if (startzaehler_zeigen == 0)
  {
    Startanzahl_anzeigen();
  }
  else
  {
    cleardisp = 0;
  }

  if (f == 1) { // Hier startet der Programmlauf

    if (startzaehler_zeigen == 1)
    {
      Anzeigeinfos_Zaehlvariable_und_Anzeigen_darstellen();
    }

    Fusstaster_und_Poti_einlesen();

    potimap = map(poti, 0, 1023, 40, 130);
    potianzeige = map(potimap, 40, 130, 10, 99);

    if (fusstaster == LOW)
    {
      hochfahren();
      istzeitholen++;

      if (istzeitholen == 1)
      {
        myTimer5 = millis();
        schreibzugriff = 1;
      }
    }

    if (millis() > PEDALZEITEEPROM + myTimer5 && istzeitholen != 0 && piepfertig == 0)
    {
      if(schreibzugriff == 1 && (startzaehler_aktiv == 0)){
      momentaneStartzahl = EEPROMReadInt(0); // Momentane Startzahl aus dem EEPROM lesen (als HEX-Wert)
      momentaneStartzahl++; // Momentane Startzahl um "1" erhöhen
      EEPROMWriteInt(0, momentaneStartzahl); // Aktualisierte Startzahl in EEPROM schreiben (als HEX-Wert)
      schreibzugriff = 0;
      }
      Serial.println(momentaneStartzahl);
      digitalWrite(8, HIGH); // Piezo Pieps ausgeben

    }

    if (millis() > (PEDALZEITEEPROM + 50) + myTimer5 && istzeitholen != 0 && piepfertig == 0)
    {
      digitalWrite(8, LOW); // Piezop wieder deaktivieren
      piepfertig = 1;
    }

    if (fusstaster == HIGH)
    {
      herunterfahren();
      istzeitholen = 0;
      piepfertig = 0;
    }

    Regler.write(power);
    //Spurservo_fahren();

  }
}
