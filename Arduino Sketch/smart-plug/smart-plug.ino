#include <Filters.h>
#include <SoftwareSerial.h>

#define CURRENT_PIN A0
#define VOLTAGE_PIN A1
#define RX_PIN 2
#define TX_PIN 3

SoftwareSerial bluetooth(RX_PIN, TX_PIN);
RunningStatistics inputStatsVoltage, inputStatsCurrent;


float frequency = 50;
float windowLength = 40.0 / frequency;

float voltageSlope = 1.54;
float volts;

float currentSlope = 0.0728;
float ampers;

unsigned long printPeriodForMeasurement = 1000;
unsigned long previousMillisForMeasurement = 0;


unsigned long printPeriodForBluetooth = 2000;
unsigned long previousMillisForBluetooth = 0;

void setup() {
  Serial.begin(9600);

  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  bluetooth.begin(9600);

  inputStatsVoltage.setWindowSecs(windowLength);
  inputStatsCurrent.setWindowSecs(windowLength);
}

void loop() {
  inputStatsVoltage.input(analogRead(VOLTAGE_PIN) * 0.72);
  inputStatsCurrent.input(analogRead(CURRENT_PIN) * 0.64);

  if (bluetooth.available()) {
    Serial.println("Reading data");
    Serial.println(bluetooth.read());
  }


  if ((unsigned long)(millis() - previousMillisForMeasurement) >= printPeriodForMeasurement) {
    previousMillisForMeasurement = millis();

    volts = voltageSlope * inputStatsVoltage.sigma();
    ampers = currentSlope * inputStatsCurrent.sigma();

    Serial.print("\tVoltage: ");
    Serial.print(volts);
    Serial.print("\t Ampers: ");
    Serial.println(ampers);
  }
  if ((unsigned long)(millis() - previousMillisForBluetooth) >= printPeriodForBluetooth) {
    previousMillisForBluetooth = millis();

    Serial.println("Sending data via bluetooth");

    bluetooth.println("V" + String(volts));
    bluetooth.println("A" + String(ampers));
  }
}
