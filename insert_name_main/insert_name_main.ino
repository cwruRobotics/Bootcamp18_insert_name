#define PIN       7
#define LAG       10
#define TRIG      8
#define ECHO      9
#define TOO_CLOSE     50
#define EPSILON     5
#define INTERVAL    50
#define TOUCH       6

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

enum STATES
{
  STOP=0,
  GOFORWARD=1,
  GOLEFT=2,
  GORIGHT=3,
  GOBACK=4
};
  
uint16_t dist = 0;
int16_t error = 0;
uint8_t command = FORWARD;
uint8_t state =  GOFORWARD;
bool last_touch = HIGH;



uint16_t poll_ultrasonic(uint8_t);
uint16_t poll_green();
void send_start_cmd(void);
void send_cmd(uint8_t);
void send_end_cmd(void);
long time = 0;

void setup(){
  pinMode(TOUCH,INPUT_PULLUP);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(PIN, OUTPUT);
  Serial.begin(115200);
}

void loop(){
  dist = 0; //poll_ultrasonic(TRIG, ECHO);
  int errorToClose= 50;
  int errorToFaaar = 50;
  if(digitalRead(TOUCH)==LOW && last_touch == HIGH)
  {
    last_touch = LOW;
    if(state == GOLEFT){
      state = GORIGHT;
    }else if (state == GORIGHT){
      state = GOLEFT;
    }else{
      state = GOLEFT;
    }
  }else{
    last_touch = digitalRead(TOUCH);
  }
  
  if(state == GORIGHT)
  {
    long start_t = millis();
    int elapsed_t = 0;
    while(elapsed_t < 4000){
      elapsed_t = millis() - start_t;
      send_cmd(BACKWARD);
      send_end_cmd();
      delay(10);
    
    }
    start_t=millis();
    elapsed_t =0;
    while(elapsed_t < 4000)
    {
      elapsed_t = millis() - start_t;
      send_cmd(FORWARD_LEFT);
      send_end_cmd();
      delay(10);
    }
    
  }

  if(state == GOLEFT)
  {
    long start_t = millis();
    int elapsed_t = 0;
    while(elapsed_t < 4000){
      elapsed_t = millis() - start_t;
      send_cmd(BACKWARD);
      send_end_cmd();
      delay(10);
    
    }
    start_t=millis();
    elapsed_t =0;
    while(elapsed_t<4000)
    {
      elapsed_t = millis() - start_t;
      send_cmd(FORWARD_RIGHT);
      send_end_cmd();
      delay(10);
    }
  }
  
  if (state == GOFORWARD){
    command = FORWARD;
    if(errorToFaaar < dist){
      command = FORWARD_RIGHT;
      
    }
    else{
      command = FORWARD;
    }
  
    if(errorToClose > dist){
      command = FORWARD_LEFT;
    }
    else{
      
      command=FORWARD;
    }
    send_cmd(command);
    send_end_cmd();
    send_cmd(command);
    send_end_cmd();
    send_cmd(command);
    send_end_cmd();
    send_cmd(command);
    send_end_cmd();
 }
 
  // if( (millis() - time) >  INTERVAL){
  //   send_cmd(command);
  //   delay(10);
  //   send_cmd(command);
  //   time = millis();
  // }
  
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
  Serial.print("SENDING CMD");
  Serial.println(num);
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
