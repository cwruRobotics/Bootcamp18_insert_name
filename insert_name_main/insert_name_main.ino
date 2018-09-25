#define PIN       7
#define LAG       10
#define TRIG      8
#define ECHO      9
#define TOO_CLOSE     50
#define EPSILON     5
#define INTERVAL    100

#define S2        10
#define S3        11
#define FREQ      12


enum COMMANDS {
  NONE      = 4,
  FORWARD     = 10,
  TURBO       = 22,
  FORWARD_LEFT  = 28,
  FORWARD_RIGHT   = 34,
  BACKWARD    = 40,
  BACK_LEFT     = 52,
  BACK_RIGHT    = 46
};

uint16_t dist = 0;
int16_t error = 0;
uint8_t command = NONE;

uint16_t poll_ultrasonic(uint8_t);
uint16_t poll_green();
void send_start_cmd(void);
void send_cmd(uint8_t);
void send_end_cmd(void);
long time = 0;

void setup(){
  Serial.begin(115200);
}

void loop(){
  dist = poll_ultrasonic(TRIG, ECHO);
  int errorToClose= 50;
  int errorToFaaar = 50;
  if(errorToFaaar < dist){
    
    command = FORWARD_RIGHT;

  }
  else{
    command = NONE;
  }

if(errorToClose > dist){
  command = FORWARD_LEFT;
}
else{
  command=NONE;
}
  if( (millis() - time) >  INTERVAL){
    send_cmd(command);
    delay(10);
    send_cmd(command);
    time = millis();
  }
  
}


/////////////////////////////////////////////////////

void send_end_cmd(){
  send_cmd(4);
}

void send_start_cmd(){
  // digitalWrite(PIN, LOW);

  digitalWrite(PIN, HIGH);
  delayMicroseconds(1500 - LAG);
  digitalWrite(PIN, LOW);
  delayMicroseconds(500 - LAG);

  digitalWrite(PIN, HIGH);
  delayMicroseconds(1500 - LAG);
  digitalWrite(PIN, LOW);
  delayMicroseconds(500 - LAG);

  digitalWrite(PIN, HIGH);
  delayMicroseconds(1500 - LAG);
  digitalWrite(PIN, LOW);
  delayMicroseconds(500 - LAG);

  digitalWrite(PIN, HIGH);
  delayMicroseconds(1500 - LAG);
  digitalWrite(PIN, LOW);
  delayMicroseconds(500 - LAG);
}

void send_cmd(uint8_t num){
  send_start_cmd();
  for(uint8_t i = 0; i<num ; i++){
    digitalWrite(PIN, HIGH);
    delayMicroseconds(500 - LAG);
    digitalWrite(PIN, LOW);
    delayMicroseconds(500 - LAG);
  }
}

uint16_t poll_ultrasonic(uint8_t trig, uint8_t echo){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long temp = pulseIn(echo, HIGH, 100000);
  return (uint16_t)(temp / 58);
}

uint16_t poll_green(){
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  return pulseIn(FREQ, HIGH, 200);
}
