/// simple telephone lamp code 
// "9" key lights up lamp
// "1" key turns it off
// '"REDIAL" key rings the phone ringer

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
  //  delay(10);
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

  //  delay(10);
  digitalWrite(col2, LOW);
  if (!digitalRead(row1)) {
    Serial.println("Button 2,1");
  } else if (!digitalRead(row2)) {
    Serial.println("Button 2,2");
  } else if (!digitalRead(row3)) {
    Serial.println("Button 2,3");
  } else if (!digitalRead(row4)) {
    Serial.println("Button 2,4");
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
  }
  digitalWrite(col3, HIGH);
  if (!digitalRead(redial)) {
    digitalWrite(ring1, LOW);
    digitalWrite(ring2, HIGH);
    delay(50);
    digitalWrite(ring1, HIGH);
    digitalWrite(ring2, LOW);
    delay(50);
  }
  else { //keep it so there shouldnt be any current flowing when the thing isnt ringing
      digitalWrite(ring1, LOW);
      digitalWrite(ring2, LOW);
  }

}
