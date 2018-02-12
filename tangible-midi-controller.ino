#define DEBUG 1

//int keys[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//int oldKeyState[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//int keyCount = 12;
int keys[] = {4, 5, 6, 7, 9, 10, 11, 12, 13};
int oldKeyState[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int keyCount = 9;
int baseNote = 48; // C4

void setup() {
  #if DEBUG == 1
    Serial.begin(31250);
  #else
    Serial.begin(9600);
  #endif

  for (int i = 0; i < keyCount; i++) {
    pinMode(keys[i], INPUT_PULLUP);
  }
}

void loop() {
  /**
    1. Keys
  */
  byte thisCommand;
  byte thisNote;
  byte thisVolume = 127;
  int curKeyState[keyCount];

  for (int i = 0; i < keyCount; i++) {
    curKeyState[i] = digitalRead(keys[i]);

    if (curKeyState[i] != oldKeyState[i]) {
      thisNote = baseNote + i;

      if (curKeyState[i] == LOW) {
        thisCommand = 0x90; // noteOn
      } else {
        thisCommand = 0x80; // noteOff
      }

      sendMidiMessage(thisCommand, thisNote, thisVolume);
      oldKeyState[i] = curKeyState[i];
    }
  }

  /**
    2. Pitch Bend
  */
  int threshold = 10;
  int bendReading = analogRead(A0);

  if (bendReading > threshold) {
    // midi high byte
    byte msb = highByte(bendReading << 1);

    // midi low byte
    byte lsb = lowByte(bendReading);
    bitWrite(lsb, 7, 0);

    sendMidiMessage(0xE0, lsb, msb);
  }

  /**
    3. DEBUG OUTPUT
  */

  #if DEBUG == 0
    Serial.println();
    Serial.println();
    for (int i = 0; i < keyCount; i++) {
      Serial.print(i);
      Serial.print("\t");
    }
    Serial.print("ANALOG \t");
    Serial.println();

    for (int k = 0; k < keyCount; k++) {
      Serial.print(digitalRead(keys[k]));
      Serial.print("\t");
    }
    Serial.print(bendReading);
    Serial.print("\t");
  #endif
}

void sendMidiMessage(byte cmd, byte data1, byte data2) {
  #if DEBUG == 1
    Serial.write(cmd);
    Serial.write(data1);
    Serial.write(data2);
  #endif
}





