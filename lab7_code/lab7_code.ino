#include "thingProperties.h"

const int sensor_pin = 0; // pin A0
int pulse_signal = 0;
// float BPM = 0;
float pulse_period = 0;
int counter = 0;

bool any_peak_detected = false;
bool first_peak_detected = false;

unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;

int upper_threshold = 950;
int lower_threshold = 550;

void setup() {
  // put your setup code here, to run once:

  delay(1500);
  initProperties();

  // Connect to cloud and get info/errors
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Wait for cloud connection
  while (ArduinoCloud.connected() !=1) {

    ArduinoCloud.update();
    delay(500);

  }

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  pulse_signal = analogRead(sensor_pin);
  Serial.println(pulse_signal);
  delay(30);

  if (pulse_signal > upper_threshold && any_peak_detected == false) {

    any_peak_detected = true;

    if (first_peak_detected == false) {

      first_pulse_time = millis();
      first_peak_detected = true;

    } else {

      second_pulse_time = millis();
      pulse_period = second_pulse_time - first_pulse_time;
      first_peak_detected = false;
      BPM = 1/(pulse_period/60000);
      // Serial.println(BPM);


    }

  }

  if (pulse_signal < lower_threshold) {

    any_peak_detected = false;

  }

  counter++;

  if (counter > 200) {
    ArduinoCloud.update();
    Serial.println(BPM);
    counter = 0;

  }

}
