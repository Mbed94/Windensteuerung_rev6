void showIntro (void)
{
  lcd.setCursor(2, 0);
  lcd.print("Schleppwinde");
  lcd.setCursor(5, 1);
  lcd.print(" V1.0");
  delay(1200);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("manufactured by");
  lcd.setCursor(6, 1);
  lcd.print("...");
  delay(1200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("* Till Wagner ");
  lcd.setCursor(0, 1);
  lcd.print("* Tim Sternagel");
  delay(1200);
  lcd.clear();
  lcd.setCursor(0, 0);

  for (int q = 0; q < 6; q++)
  {
    lcd.print("## All rights ##");
    lcd.setCursor(0, 1);
    lcd.print("##  reserved  ##");
    delay(200);
    lcd.clear();
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.clear();
    delay(90);
  }

  delay(300);
}




void Drehzahlvorwahl_und_bestaetigen(void)
{
  while ((digitalRead(13) == 1) && (f == 0) && (x == 0))
  {
    if (d == 0 && f == 0)
    {
      lcd.setCursor(0, 0);
      lcd.print("Drehzahl einst.");
      lcd.setCursor(0, 2);
      lcd.print("und bestaetigen");
      delay (2000);
      lcd.clear();
      d = 1;
    }

    poti = analogRead(A0);
    potimap = map(poti, 0, 1023, 40, 130);
    potianzeige = map(potimap, 40, 130, 10, 99);

    lcd.setCursor(0, 0);
    lcd.print("Spindeldrehzahl:");
    lcd.setCursor(6, 1);
    lcd.print(potianzeige);
    lcd.setCursor(8, 1);
    lcd.print("%");

    if ((digitalRead(13) == 0) && (f == 0))
    {
      f = 1;
    }
  }

  if (x == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("### Achtung ###");
    lcd.setCursor(0, 1);
    lcd.print("! System aktiv !");
    delay(2500);
    x = 1;
    lcd.clear();
  }
}


void hochfahren(void)
{
  if (millis() > VERZH + myTimer ) {
    myTimer = millis();

    if (power < potimap)
    {
      power++;
    }
  }
}

void herunterfahren(void)
{
  if (millis() > VERZR + myTimer ) {
    myTimer = millis();

    if (power > 30)
    {
      power--;
    }
  }
}



void Spurservo_fahren(void) {

  if (u == 1 && o == 0) {

    static int i = 20;

    if (millis() > (FUEHRGESCHW / power) + myTimer2 ) {
      myTimer2 = millis();

      Spur.write(i);
      if (power > 30) {
        i++;
      }
      //Serial.println(i);

      if (i == 160) {
        o = 1;
        u = 0;
        i = 20;
      }

    }
  }



  if (u == 0 && o == 1) {

    static int s = 160;

    if (millis() > (FUEHRGESCHW / power) + myTimer2 ) {
      myTimer2 = millis();

      Spur.write(s);

      if (power > 30) {
        s--;
      }
      //Serial.println(s);

      if (s == 20) {
        o = 0;
        u = 1;
        s = 160;
      }

    }
  }
}


void Spindeldrehzahl_anzeigen(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Spindeldrehzahl:");
  lcd.setCursor(6, 1);
  lcd.print(potianzeige);
  lcd.setCursor(8, 1);
  lcd.print("%");
}


void UBat_anzeigen(void)
{
  spg = analogRead(A1);
  spg = map(spg, 0, 1024, 0, 5);
  lcd.setCursor(0, 0);
  lcd.print("   Spannung:");
  lcd.setCursor(6, 1);
  lcd.print(spg);
}

void BetrZeit_erstellen(void)
{
  if (millis() > 1000 + myTimer4 )
  {
    myTimer4 = millis();
    Sek++;
    //Serial.println(Sek);
    if (Sek == 60)
    {
      Min++;
      Sek = 0;
    }

    if (Min == 60)
    {
      Stu++;
      Min = 0;
    }
  }
}

void BetrZeit_anzeigen(void)
{
  lcd.setCursor(0, 0);
  lcd.print(" Betriebszeit:");
  lcd.setCursor(1, 1);
  lcd.print("h:");
  lcd.setCursor(3, 1);
  lcd.print(Stu);
  lcd.setCursor(6, 1);
  lcd.print("m:");
  lcd.print(Min);
  lcd.setCursor(11, 1);
  lcd.print("s:");
  lcd.print(Sek);
}

void Startanzahl_anzeigen (void)
{

  startanzahl = EEPROMReadInt(0);
  
  cleardisp++;
  if(cleardisp == 1)
  {
  lcd.clear();
  }
  
  lcd.setCursor(0, 0);
  lcd.print(" Starts gesamt:");
  lcd.setCursor(7, 1);
  lcd.print(startanzahl);
}

void Startzaehler_Status(void)
{
  if (startzaehler_aktiv == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Startzaehler:");
    lcd.setCursor(5, 1);
    lcd.print("aktiv");
  }

  else
  {
    lcd.setCursor(0, 0);
    lcd.print("  Startzaehler:");
    lcd.setCursor(4, 1);
    lcd.print("inaktiv");
  }
}



void Fusstaster_und_Poti_einlesen(void)
{
  fusstaster = digitalRead(13);
  poti = analogRead(A0);
}

void Anzeigeinfos_Zaehlvariable_und_Anzeigen_darstellen(void)
{
  if (millis() > (LCDDAUER + myTimer3)) {
    myTimer3 = millis();
    if (z > 3)
    {
      z = 0;
    }
    z++;

    //Serial.println(z);
    lcd.clear();
  }

  if (z == 1) {
    Spindeldrehzahl_anzeigen();
  }

  if (z == 2) {
    UBat_anzeigen();
  }

  if (z == 3) {
    BetrZeit_anzeigen();
  }

  if (z == 4) {
    Startzaehler_Status();
  }
}



//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
