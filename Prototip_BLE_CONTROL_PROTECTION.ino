int motor1pin1 = 4;
int motor1pin2 = 5;
int motor2pin1 = 6;
int motor2pin2 = 7;
int ENA = 10;
int ENB = 9;

void setup() {
  Serial.begin(9600);

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void loop() {
  if (Serial.available()) {
    char Dir = Serial.read();

    if (Dir == 'F') {  // Forward
      Stop();
      delay(100);
      Forward();
      
    }

    if (Dir == 'B') {  // Backward
      Stop();
      delay(100);
      Back();
    }

    if (Dir == 'L') {  // Left turn
      Stop();
      delay(100);
      Left();
    }

    if (Dir == 'R') {  // Right turn
      Stop();
      delay(100);
      Right();
    }

    if (Dir == 'S') {  // Stop
      Stop();
    }

    if (Dir == 'G') {  
      analogWrite(ENA, 255);
      analogWrite(ENB, 255);
    }

      if (Dir == 'A') {  
      analogWrite(ENA, 155);
      analogWrite(ENB, 155);
    }

    if (Dir == 'Z') {  
      analogWrite(ENA, 100);
      analogWrite(ENB, 100);
    }

  }
}

void Forward(){
    digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
  }

  void Back(){
    digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
  }

  void Left(){
    digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
  }

  void Right(){
    digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
  }

  void Stop(){
    digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
  }

