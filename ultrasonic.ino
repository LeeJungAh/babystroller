int ultrasonic = 10;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  long dt = 0;
  int dist = 0;

  pinMode(ultrasonic, OUTPUT);
  digitalWrite(ultrasonic, HIGH);
  digitalWrite(ultrasonic,LOW);

  pinMode(ultrasonic, INPUT);

  while(digitalRead(ultrasonic) == LOW);
  dt = micros();
  while(digitalRead(ultrasonic) == HIGH);
  dt = micros() - dt;

  dist = (333.5 + 0.60714)*((float)dt / 10000 / 2);

  Serial.println(dist);

  delay(200);
}

