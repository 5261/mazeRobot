#define alphaSensor 0
#define midLefSensor 3
#define midRgtSensor 5
#define lefWingSensor 4
#define rgtWingSensor 2

#define lefWheel 6
#define rgtWheel 7

#define forSpeed 130
#define sopSpeed 200
#define bacSpeed 270

#define n 7
const int cmd[n] = { -1, +1, 0, +1, -1, -1, +1 };

int i = 0;

int gain(int pin, int times = 100, int bound = 80){
  int cnt = 0;
  
  for(int i = 0; i < times; i++){
    if(digitalRead(pin)){
      cnt++;
    }
  }
  
  return cnt >= bound;
}

 
void pulse(int pin, int interval){
  digitalWrite(pin, 1);
  delayMicroseconds(interval);
  digitalWrite(pin, 0);
}

void forward(){
  pulse(lefWheel, forSpeed);
  delay(1);
  pulse(rgtWheel, forSpeed);
}

void bacward(){
  pulse(lefWheel, bacSpeed);
  delay(1);
  pulse(rgtWheel, bacSpeed);
}

void turnLef(){
  pulse(lefWheel, sopSpeed);
  delay(1);
  pulse(rgtWheel, forSpeed);
}

void turnRgt(){
  pulse(lefWheel, forSpeed);
  delay(1);
  pulse(rgtWheel, sopSpeed);
}

void pause(){
  pulse(lefWheel, sopSpeed);
  delay(1);
  pulse(rgtWheel, sopSpeed);

  while(1);
}

void inter(int t){
  pulse(lefWheel, sopSpeed);
  delay(1);
  pulse(rgtWheel, sopSpeed);

  delay(t * 1000);
}

void setup(){
  pinMode(lefWheel, OUTPUT);
  pinMode(rgtWheel, OUTPUT);

  pinMode(midLefSensor, INPUT);
  pinMode(midRgtSensor, INPUT);
  pinMode(lefWingSensor, INPUT);
  pinMode(rgtWingSensor, INPUT);
  
  Serial.begin(9600);
}

void loop(){
  if(i == n){
    Serial.println("loop twice");
    forward();
    while(1);
  }
  
  forward();
  
  while(1){
    if(!gain(midLefSensor)){
      turnRgt();
      while(!gain(midLefSensor));
      forward();
    }
  
    if(!gain(midRgtSensor)){
      turnLef();
      while(!gain(midRgtSensor));
      forward();
    }
    
    if(gain(lefWingSensor) || gain(rgtWingSensor)){
      Serial.println(i);
      Serial.println("is");
      Serial.println(cmd[i]);
      
      if(cmd[i] == -1){
        turnLef();
        delay(500);
        // while(!gain(alphaSensor));
        // inter(1);
        
        while(!gain(midLefSensor) || !gain(midRgtSensor));
        
        forward();
      } else if(cmd[i] == +1){
        turnRgt();
        delay(500);
        // while(!gain(alphaSensor));
        // inter(2);
        
        while(!gain(midLefSensor) || !gain(midRgtSensor));
        
        forward();
      } else{
        // inter(3);
        
        forward();
  
        while(gain(lefWingSensor) || gain(rgtWingSensor));
      }

      i++;

      Serial.println("i++");
      Serial.println(i);
      
      if(i == n){
        Serial.println("charging");
        forward();
          
        if(!gain(midLefSensor) && gain(midRgtSensor)){
          turnRgt();
          while(!gain(midLefSensor));
          forward();
        }
        
        if(!gain(midRgtSensor) && gain(midLefSensor)){
          turnLef();
          while(!gain(midRgtSensor));
          forward();
        }

        forward();
        Serial.println("Hoooooooooooo-----------!");
        while(1);
        // delay(100);
        }
      }
    }
 }
