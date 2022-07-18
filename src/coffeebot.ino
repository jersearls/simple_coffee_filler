// set pin positions
#define SOLENOID 3
int FLOAT_SWITCH = D6;

// declare variables
volatile int count = 0;
int floatState;
bool pumpFault = false;
String pumpDuration = "15";
int pumpDurationInt;

// inital setup
void setup()
{
  // set pins
  pinMode(SOLENOID, OUTPUT);
  pinMode(FLOAT_SWITCH, INPUT_PULLUP);

  // set state
  digitalWrite(SOLENOID, LOW);

  // cloud variables
  Particle.variable("Float State", floatState);
  Particle.variable("Pump Fault?", pumpFault);
  Particle.variable("Pump Duration (in seconds)", pumpDuration);

  // cloud functions
  Particle.function("Pump Reset", PumpReset);
  Particle.function("Set Pump Duration", SetPumpDuration);
}

// loop
void loop()
{
  pumpDurationInt = pumpDuration.toInt();
  floatState = digitalRead(FLOAT_SWITCH);

  if (floatState)
  {
    digitalWrite(SOLENOID, LOW); // Switch Solenoid OFF
    count = 0;
    Particle.publish("Info", "Solenoid OFF");
  }
  else if (count > pumpDurationInt)
  {
    digitalWrite(SOLENOID, LOW);
    pumpFault = true;
    count = 0;
    Particle.publish("Info", "Solenoid OFF");
    Particle.publish("Filling Error", "Check Water!");
  }
  else if (!pumpFault && !floatState)
  {
    digitalWrite(SOLENOID, HIGH); // Switch Solenoid OFF
    Particle.publish("Info", "Solenoid ON");
    count += 1;
  }
  delay(1000);
}

// cloud functions
int PumpReset(String message)
{
  Particle.publish("Info", "Resetting Pump");
  pumpFault = false;
  return 0;
}

int SetPumpDuration(String message)
{
  Particle.publish("Info", "Setting Pump Duration");
  pumpDuration = message;
  return 0;
}
