#include <IRremote.h>

#define DEBUG
#define DECODE_SAMSUNG 1

#define IR_RECEIVE_PIN A3
#define IR_SEND_PIN 3

#define REPEAT_DELAY 300

unsigned long inCode[] = { 0xF, 0xB, 0x7 }; // Samsung TV
unsigned long outCode[] = { 0x39, 0x3B, 0x33 }; // Samsung Home Theater

#define SAMSUNG_OUT_ADDRESS 0x5343

#if defined(DEBUG)
  char *buttons[] = { "MUTE", "VOL_DOWN", "VOL_UP" };
#endif

int codesCount = sizeof(inCode) / sizeof(inCode[0]);

void setup() {
  #if defined(DEBUG)
    Serial.begin(115200);
  #endif

  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(IR_SEND_PIN);
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long command = IrReceiver.decodedIRData.command;

    #if defined(DEBUG)
      Serial.print("input command: ");
      Serial.println(command);
    #endif

    for (int i = 0; i < codesCount; i++) {
      if (command == inCode[i]) {
        IrReceiver.stop();
        IrSender.sendSamsung(SAMSUNG_OUT_ADDRESS, outCode[i], 0);
        IrReceiver.start();

        #if defined(DEBUG)
          Serial.print("command sent: ");
          Serial.println(buttons[i]);
        #endif

        delay(REPEAT_DELAY);
      }
    }

    IrReceiver.resume();
  }
}
