#include <SPI.h>
#include <LoRa.h>

#define PIN_LED	13
#define LED_MILLIS 500

#define PIN_LORA_CS	8
#define PIN_LORA_RST	4
#define PIN_LORA_IRQ	7

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver v3");

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  LoRa.setPins(PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_IRQ);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1) {
  	digitalWrite(PIN_LED, HIGH);
	delay(100);
  	digitalWrite(PIN_LED, LOW);
	delay(100);
    }
  }
  Serial.println("LORA register dump:");
  LoRa.dumpRegisters(Serial);
  Serial.println("Setup done");
}

int nextblink = 0;
bool blinkstate = true;

void blink() {
	if (blinkstate) {
  	  digitalWrite(PIN_LED, HIGH);
	  blinkstate = false;
	} else {
  	  digitalWrite(PIN_LED, LOW);
	  blinkstate = true;
	} 
}

void loop() {
  int now = millis();
  if (now > nextblink) {
	blink();
	nextblink = now + LED_MILLIS;
  } 

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("RCV");

    Serial.print(" millis:");
    Serial.print(millis());

    Serial.print(" rssi:");
    Serial.print(LoRa.packetRssi());

    Serial.print(" snr:");
    Serial.print(LoRa.packetSnr());

    Serial.print(" fe:");
    Serial.print(LoRa.packetFrequencyError());

    Serial.print(" size:");
    Serial.print(packetSize);

    Serial.print(" data:");
    while (LoRa.available()) {
        byte val = LoRa.read();
        Serial.print(" ");
        if (val < 16) { Serial.print("0"); }
        Serial.print(String(val,HEX));
	blink();
    }
    Serial.println("");
  }
}
