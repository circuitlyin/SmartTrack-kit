int RMotor_1 = 2;//2
int RMotor_2 = 3;//3

int LMotor_1 = 4;//4
int LMotor_2 = 5;//5

int REnable = 10;//10
int LEnable = 11;//11

int motor_speed = 90;//adjust the speed upto max 260  min 60

void setup() {

  Serial.begin(9600);

  pinMode(RMotor_1, OUTPUT);
  pinMode(RMotor_2, OUTPUT);

  pinMode(LMotor_1, OUTPUT);
  pinMode(LMotor_2, OUTPUT);

  pinMode(REnable, OUTPUT);
  pinMode(LEnable, OUTPUT);

  pinMode(7, INPUT);//7
  pinMode(8, INPUT);//8

  analogWrite(REnable, motor_speed);
  analogWrite(LEnable, motor_speed);

  Serial.println("Robot Started");
}

void loop() {

  int ldrright = digitalRead(7);
  int ldrleft  = digitalRead(8);

  Serial.print("Left: ");
  Serial.print(ldrleft);

  Serial.print(" Right: ");
  Serial.println(ldrright);

  if (ldrright == 0 && ldrleft == 0) {

    Serial.println("FORWARD");
    move_forward();
  }

  else if (ldrright == 0 && ldrleft == 1) {

    Serial.println("RIGHT");
    turn_right();
  }

  else if (ldrright == 1 && ldrleft == 0) {

    Serial.println("LEFT");
    turn_left();
  }

  else {

    Serial.println("STOP");
    move_stop();
  }

  delay(100);
}

void move_forward() {

  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);

  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

void turn_right() {

  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, LOW);

  digitalWrite(LMotor_1, HIGH);
  digitalWrite(LMotor_2, LOW);
}

void turn_left() {

  digitalWrite(RMotor_1, HIGH);
  digitalWrite(RMotor_2, LOW);

  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, LOW);
}

void move_stop() {

  digitalWrite(RMotor_1, LOW);
  digitalWrite(RMotor_2, LOW);

  digitalWrite(LMotor_1, LOW);
  digitalWrite(LMotor_2, LOW);
}