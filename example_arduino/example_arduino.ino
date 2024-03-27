/*

This example shows how to use DRV8662 piezo driver.
This code was tested using an ESP32.

*/

#define DRV8662_ENABLE_PIN 32
#define DRV8662_PWM_PN 33

#define NUM_PWM_BITS 9
#define PWM_FREQUENCY 100 * 1000


#define FULL_RANGE ((1 << NUM_PWM_BITS) - 1)
#define HALF_RANGE (FULL_RANGE / 2)

void setup() {
  Serial.begin(115200);
  Serial.println("You can update the frequency with the serial monitor");

  ledcSetup(0, 100 * 1000, NUM_PWM_BITS);
  ledcAttachPin(DRV8662_PWM_PN, 0);
  pinMode(DRV8662_ENABLE_PIN, OUTPUT);

  digitalWrite(DRV8662_ENABLE_PIN, LOW);
  ledcWrite(0, HALF_RANGE);
  delay(100);

  digitalWrite(DRV8662_ENABLE_PIN, HIGH);
  delay(100);
}

int setFrequency = 120;

void drivePiezo(
  float frequency,
  int durationMs
) {

  ledcWrite(0, HALF_RANGE);
  time_t startUs = micros();
  time_t deltaTus = 0;
  do {
    deltaTus = micros() - startUs;

    int pwm =
        HALF_RANGE +
        sin(deltaTus * frequency / 1000000.0f * TWO_PI) * HALF_RANGE;
        ledcWrite(0, pwm);
        delayMicroseconds(10);

  } while (deltaTus < durationMs * 1000);

  ledcWrite(0, HALF_RANGE);
}

void readFrequencyFromSerial();

void loop() {
  readFrequencyFromSerial();

  drivePiezo(
    setFrequency,
    50
  );

  delay(500);
  readFrequencyFromSerial();

  drivePiezo(
    setFrequency,
    100
  );

  delay(500);
  readFrequencyFromSerial();

  drivePiezo(
    setFrequency,
    200
  );

  delay(500);
  readFrequencyFromSerial();

  drivePiezo(
    setFrequency,
    500
  );

  delay(500);
}



void readFrequencyFromSerial() {

  int temp = Serial.parseInt();
  if (temp > 0) {
    setFrequency = temp;
  }
  Serial.print("Using frequency: ");
  Serial.println(setFrequency);
}