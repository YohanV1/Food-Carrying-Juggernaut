#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define IN1 18
#define IN2 19
#define IN3 16
#define IN4 17
#define IR_SensorL 4 
#define IR_SensorR 2

char auth[] = "5slX7DfhSZf2MfI_5HTg_OXDcBhAa6Lu";
char ssid[] = "Drill Press Cafe";
char pass[] = "password";

bool stopActionPerformed = false;

int x=50, y=50, speed, line_toggle;
int IR_R;
int IR_L;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Blynk.begin(auth, ssid, pass);
  Serial.println("Connected to Blynk!");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IR_SensorL, INPUT);
  pinMode(IR_SensorR, INPUT); 

}

BLYNK_WRITE(V0) {
  x = param[0].asInt();
}

BLYNK_WRITE(V1) {
  y = param[0].asInt();
}

BLYNK_WRITE(V2) {
  speed = param.asInt();
}

BLYNK_WRITE(V3) {
  line_toggle = param[0].asInt();
}

void rc() {
  if (y > 70) {
    carForward();
  } else if (y < 30) {
    carBackward();
  } else if (x < 30) {
    carLeft();
  } else if (x > 70) {
    carRight();
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
  }
}

void loop() {
  Blynk.run();
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.print(y);
  Serial.print(", Speed: ");
  Serial.print(speed);
  Serial.print(", line_toggle: ");
  Serial.println(line_toggle);
  if (line_toggle == 0) {
    if (!stopActionPerformed) {
      carStop(); 
      stopActionPerformed = true;
    }
    lineFollower();
  } else {
    stopActionPerformed = false; 
    rc();
  }
  
  } // https://www.youtube.com/watch?v=t7k9D1jDEtk

void carForward() {
  // analogWrite(ENA, Speed);
  // analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  // analogWrite(ENA, Speed);
  // analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  // analogWrite(ENA, Speed);
  // analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  // analogWrite(ENA, Speed);
  // analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void lineFollower()
{
  IR_R = digitalRead(IR_SensorR); 
  IR_L = digitalRead(IR_SensorL);  
 
  if (IR_L == 0 && IR_R == 0)
  {
    Serial.println("straight");
    carForward()
  }
  else if(IR_R == 0 && IR_L == 1)
  {
    Serial.println("turn right");
    carRight()
  }
  else if(IR_R==1 && IR_L == 0)
  {
    Serial.println("turn left");
    carLeft()
  }
  /*
  if(IR_L==0)
  {               
  Serial.println("L = white");   
  }
  else 
  {
  Serial.println("L = black");  
  }
 
  if(IR_R==0)
  {               
  Serial.println("R = white");   
  }
  else 
  {
  Serial.println("R = black"); } */
  
delay(100);

}
