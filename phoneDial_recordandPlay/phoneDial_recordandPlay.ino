/////////////////////
// telephonics telephone lamp! 
// its a lamp made from an old household touch tone telephone. 
// pressing the 9 button turns the light on, 1 turns it off.
// hold the redial button and play a rhythm with the * key and it records and loops
//
// code made in febuary 2023 by peter chu and william kennedy
// record and play code used from ralf schreiber's http://www.ralfschreiber.com/share/pulseandpatterns.html
// which was intitally from MrGrok https://bitbucket.org/mrgrok/mrgroks-arduino-sketches/src/560b538c648e/?at=master
//
// TODO: 
// put ring function back into millis mode (dont use delay() )
// make it so the record function will hold the notes if you hold (currently it only records correctly if you play staccato
//
/////////////////////////////////////////
#define col2 4
#define row2 5
#define row3 6
#define col2b 7
#define col1 8
#define row4 9
#define col3 10
#define row1 11
#define redial 12
#define ring1 2
#define ring2 3
#define lamp 13

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 50;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED
boolean ringoffFlag = false;

/// record and play variables
boolean flag;
boolean morseBtn;
boolean modeBtn;
//const byte motor = 0; //ATtiny85

const byte sampleLength = 1; // 1 ms
const byte maxSamples = 100;

boolean inRecordMode = false;
boolean wasInRecordMode = false;

// buffers to record state/duration combinations
boolean states[maxSamples];
int durations[maxSamples];
int currentSampleCycles;

short idxPlayback = 0;
short idxRecord = 0;
/////

void resetForRecording() {
  memset(states, 0, sizeof(states));
  memset(durations, 0, sizeof(durations));
  idxRecord = 0; // reset record idx just to make playback start point obvious
  idxPlayback = 0;
  currentSampleCycles = 0;
}

void setup() {
  // sending pulses OUT high out on the columns
  // listening IN on rows to see what buttons are pushed
  pinMode(col2, OUTPUT);
  pinMode(row2, INPUT_PULLUP);
  pinMode(row3, INPUT_PULLUP);
  pinMode(col1, OUTPUT);
  pinMode(row4, INPUT_PULLUP);
  pinMode(col3, OUTPUT);
  pinMode(row1, INPUT_PULLUP);
  // SSR on to turn lamp on and off
  pinMode(lamp, OUTPUT);
  // H-bridge needs 2 signals, inverted to ring the bell
  pinMode(ring1, OUTPUT);
  pinMode(ring2, OUTPUT);
  // redial button for ringer
  pinMode(redial, INPUT_PULLUP);
  digitalWrite(lamp, HIGH);
  Serial.begin(9600);
}

void loop() {
  //    delay(10);
  if (!digitalRead(row1)) {
    Serial.println("row 1 is pressed");
  }
  digitalWrite(col1, LOW);
  if (!digitalRead(row1)) {
    Serial.println("Button 1,1");
    digitalWrite(lamp, HIGH);
  } else if (!digitalRead(row2)) {
    Serial.println("Button 1,2");
  } else if (!digitalRead(row3)) {
    Serial.println("Button 1,3");
  } else if (!digitalRead(row4)) {
    Serial.println("Button 1,4");
  }
  digitalWrite(col1, HIGH);

  //    delay(1);
  digitalWrite(col2, LOW);
  if (!digitalRead(row1)) {
    //    Serial.println("Button 2,1");
  } else if (!digitalRead(row2)) {
    //    Serial.println("Button 2,2");
  } else if (!digitalRead(row3)) {
    //    Serial.println("Button 2,3");
  } else if (!digitalRead(row4)) {
    //    Serial.println("Button 2,4");
  }
  digitalWrite(col2, HIGH);

  //  delay(10);
  digitalWrite(col3, LOW);
  if (!digitalRead(row1)) {
    Serial.println("Button 3,1");
  } else if (!digitalRead(row2)) {
    Serial.println("Button 3,2");
  } else if (!digitalRead(row3)) {
    Serial.println("Button 3,3");
    digitalWrite(lamp, LOW);
  } else if (!digitalRead(row4)) {
    Serial.println("Button 3,4");
    flag = true;
  }
  digitalWrite(col3, HIGH);

  //  if (!digitalRead(redial)) {
  //    ring();
  //  }
  //  else { //keep it so there shouldnt be any current flowing when the thing isnt ringing
  //    ringoff();
  //  }

  if (flag) {
    morseBtn = true;
    ring();
  } else {
    morseBtn = false;
    ringoff();
  }
  flag = false;
  //  if (digitalRead(redial)) {
  //    pushedRedialOnce = true;
  //  } else
  //   }
  //  if (pushedRedial) {
  inRecordMode = !digitalRead(redial);
  if (inRecordMode == true) {
    //    Serial.print("in record mode true");
    if (!wasInRecordMode) {
      resetForRecording();
    }
    recordLoop();
  } else {
    // continue playing loop
    playbackLoop();
  }
  wasInRecordMode = inRecordMode; // record prev state for next iteration so we know whether to reset the record arr index
}

void recordLoop() {
  //  Serial.println("record loop");
  boolean state = morseBtn;
  //  if (state) { // give feedback to person recording the loop
  //    ring();
  //  } else (ringoff());

  if (states[idxRecord] == state) {
    // state not changed, add to duration of current state
    durations[idxRecord] += sampleLength;
  } else {
    // state changed, go to next idx and set default duration
    idxRecord++;
    if (idxRecord == maxSamples) {
      idxRecord = 0;  // reset idx if max array size reached
    }
    states[idxRecord] = state;
    durations[idxRecord] = sampleLength;
  }
  delay(sampleLength); // slow the loop to a time constant so we can reproduce the timelyness of the recording
}

void playbackLoop() {
  if (currentSampleCycles == 0 && durations[idxPlayback] != 0) {
    // state changed
    if (idxPlayback > 0 && states[idxPlayback] && idxPlayback < 100 ) {
      ring();
    } else {
      ringoff();
    }
  }
  
  if (idxPlayback == maxSamples) {
    // EOF recorded loop - repeat
    idxPlayback = 0;
    currentSampleCycles = 0;
  } else {
    //  if(durations[idxPlayback]*playbackMultiplier <= currentSampleCycles) // speed ctls deactivated
    if (durations[idxPlayback] * 1 <= currentSampleCycles)
    {
      // EOF current sample in recorder buffer
      idxPlayback++; // move to next sample
      currentSampleCycles = 0; // reset for next sample
    } else {
      // still in same sample, but in next cycle (==sampleLength approx)
      currentSampleCycles++;
    }
  }
  delay(sampleLength); // keep time same as we had for record loop (approximately)
}

void ring () {
  digitalWrite(ring1, LOW);
  digitalWrite(ring2, HIGH);
  delay(interval);
  digitalWrite(ring2, LOW);
  digitalWrite(ring1, HIGH);
  delay(interval);
}

//
//  unsigned long currentMillis = millis();
//  if (currentMillis - previousMillis >= interval) {
//    // save the last time you blinked the LED
//    previousMillis = currentMillis;
//    // if the LED is off turn it on and vice-versa:
//    if (ledState == LOW) {
//      Serial.println("led state == low");
//      ledState = HIGH;
//      digitalWrite(ring1, LOW);
//      digitalWrite(ring2, HIGH);
//    } else {
//      Serial.println("led stte == high");
//      ledState = LOW;
//      digitalWrite(ring1, HIGH);
//      digitalWrite(ring2, LOW);
//    }
//  }
//}

void ringoff() {
  ringoffFlag = true;
  digitalWrite(ring1, LOW);
  digitalWrite(ring2, LOW);
  ringoffFlag = false;
}
