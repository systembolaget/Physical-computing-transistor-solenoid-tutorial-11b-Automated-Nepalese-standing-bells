// Tutorial 11b. Automated Nepalese standing bells

// Main parts: Adafruit Metro Mini, TDS-10A solenoids, 9V battery,
// Infineon Technologies IRFZ44N MOSFET, Nepalese standing bells

// Variables that remain constant
const byte pinSolenoidSmall = 5; // Digital output pin to solenoid
const byte pinSolenoidLarge = 6;

// Variables that can change
unsigned int bellSmallInterval = 0; // Time to pass until next strike
unsigned int bellLargeInterval = 0;
unsigned long timeNowBellSmall = 0; // Timestamp that updates each loop() iteration
unsigned long timeNowBellLarge = 0;

void setup()
{
  // Initialise the solenoids' transistor gate pins
  pinMode(pinSolenoidSmall, OUTPUT);
  pinMode(pinSolenoidLarge, OUTPUT);

  // Generate a new random number sequence with every start
  randomSeed(analogRead(4));

  Serial.begin(9600);
}

void loop()
{
  // A call to these functions will cause random bell strokes.
  // Because of millis() timing of everything, many more bells
  // could be added, as no delay() interferes with the timing
  strikeBellSmall();
  strikeBellLarge();
}

void strikeBellSmall()
{
  // A call to this function will cause the bell's solenoid to
  // strike when it is time to do so. The interval between strokes
  // is generated randomly after each stroke. To begin with, we
  // set the bell's solenoid striker to a "ready to strike"
  // (if it is time) state. A static variable only "lives" inside
  // the function in which it is declared. Its value will also
  // "survive" each new function call (unlike a local variable)
  // and the bell "remembers" its last state
  static byte state = 0;

  switch ( state )
  {
    // If ready to strike
    case 0 :

      // Check if it is time to do so
      if (millis() - timeNowBellSmall >= bellSmallInterval) // Check if it is time to strike
      {
        // And energise the bell's solenoid
        digitalWrite(pinSolenoidSmall, HIGH);

        // Record the current time
        timeNowBellSmall = millis();

        // Then set the state to "started to strike"
        state = 1;
      }

      // Then jump back to loop()
      break;

    // If started to strike
    case 1 :

      // Check if the stroke (= 100 milliseconds) is complete
      if (millis() - timeNowBellSmall >= 100)
      {
        // And de-energise the bell's solenoid
        digitalWrite(pinSolenoidSmall, LOW);
        Serial.println("Small");

        // Record the current time
        timeNowBellSmall = millis();

        // Then set the state to "ready to strike"
        state = 0;

        // Finally, generate a new random strike interval
        bellSmallInterval = random(7654, 34567);
      }
  }
}

void strikeBellLarge()
{
  static byte state = 0;

  switch ( state )
  {
    case 0 :
      if (millis() - timeNowBellLarge >= bellLargeInterval)
      {
        digitalWrite(pinSolenoidLarge, HIGH);
        Serial.println("Large");
        timeNowBellLarge = millis();
        state = 1;
      }
      break;

    case 1 :
      if (millis() - timeNowBellLarge >= 100)
      {
        digitalWrite(pinSolenoidLarge, LOW);
        timeNowBellLarge = millis();
        state = 0;
        bellLargeInterval = random(3456, 23456);
      }
  }
}

/* These functions are shorter, but require code-blocking delay()
  void strikeBellSmall()
  {
  if (millis() - timeNowBellSmall >= bellSmallInterval)
  {
    timeNowBellSmall = millis();
    digitalWrite(solenoidSmall, HIGH);
    delay(110);
    digitalWrite(solenoidSmall, LOW);
    bellSmallInterval = random(17000, 31000);
  }
  }

  void strikeBellLarge()
  {
  if (millis() - timeNowBellLarge >= bellLargeInterval)
  {
    timeNowBellLarge = millis();
    digitalWrite(solenoidLarge, HIGH);
    delay(110);
    digitalWrite(solenoidLarge, LOW);
    bellLargeInterval = random(18000, 44000);
  }
  }
*/
