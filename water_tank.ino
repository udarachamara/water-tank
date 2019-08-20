
double water_level = 0;
float water_sensor = 0;
float intervals_reads[2] = {0, 0};
boolean system_error = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (system_error) {
    digitalWrite(5, LOW);
    systemErrorIndicator();
  } else {
    water_sensor = analogRead(A0);
    //Serial.println(water_sensor);

    if (water_sensor > 50) {
      if(checkSensorValueForTimeIntervals()){
        onWaterMachine();
      }
        
    } else {
      tankFullIndicate();
      offWaterMachine();
    }
  }

}

void tankFullIndicate() {
  digitalWrite(2, HIGH); // turn on blue light
  digitalWrite(3, LOW); // turn off red light blink
}

void tankLowIndicate() {
  digitalWrite(2, LOW); // turn off blue light
}


// this function is for blink red led while water filling in to tank
void waterFillingIndicate() {
  for (int i = 0 ; i < 400 ; i++) {
    analogWrite(3, 200);
    delay(300);
    analogWrite(3, 0);
    delay(300);
    water_level++;

        if(hasSystemError()){
          break;
        }
  }
}

void onWaterMachine() {
  digitalWrite(5, HIGH);
  tankLowIndicate();
  waterFillingIndicate();
}

void offWaterMachine() {
  digitalWrite(5, LOW);
  tankFullIndicate();
}

void systemErrorIndicator() {
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  delay(300);
}

boolean checkSensorValueForTimeIntervals() { // validate sensor value some time intervals
  intervals_reads[0] = analogRead(A0);
  delay(1000);
  intervals_reads[1] = analogRead(A0);
  delay(1000);

  if (intervals_reads[0] > 50 && intervals_reads[1] > 50 ) {
    return true;
  } else {
    return false;
  }
}

boolean hasSystemError() { // check whether sensor conecter broken or not
  float temp1 = analogRead(A0);
  float temp2 = analogRead(A0);
  if (water_level > 15 && water_level < 18) {
    if (temp1 > 50 && temp2 > 50) {
      system_error = true;
      digitalWrite(5, LOW);
      return true;
    } else {
      return false;
    }
  }
}
