#include <IRremote.h>

#define DEBUG
#define DECODE_SAMSUNG 1

#define IR_RECEIVE_PIN A3
#define IR_SEND_PIN 3

unsigned long inCode[] = { 0xF, 0xB, 0x7 };
unsigned long outCode[] = { 0x39, 0x3B, 0x33 };

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
  unsigned long command = 0;

  if (IrReceiver.decode()) {
    command = IrReceiver.decodedIRData.command;
    #if defined(DEBUG)
      Serial.println(command);
    #endif

    for (int i = 0; i < codesCount; i++) {
      if (command == inCode[i]) {
        #if defined(DEBUG)
          Serial.println(buttons[i]);
        #endif

        IrReceiver.stop();
        IrSender.sendSamsung(0x5343, outCode[i], 0);
        IrReceiver.start();

        command = 0;
        delay(333);
      }
    }

    IrReceiver.resume();
  }
}
