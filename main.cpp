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

int gain(int pin, int times = 50, int bound = 10){
  int cnt = 0;
  
  for(int i = 0; i < times; i++){
    if(digitalRead(pin)){
      cnt++;
    }
  }
  
  return cnt >= bound;
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
      Serial.println(cmd[i]);
      
      if(cmd[i] == -1){
        // inter(1);
        
        turnLef();
        // delay(500);

        while(gain(alphaSensor));
        while(!gain(alphaSensor));
        
        // while(!gain(midLefSensor) || !gain(midRgtSensor));
        
        forward();
      } else if(cmd[i] == +1){
        // inter(2);

        turnRgt();
        // delay(500);
        
        while(gain(alphaSensor));
        while(!gain(alphaSensor));
        
        // while(!gain(midLefSensor) || !gain(midRgtSensor));
        
        forward();
      } else{
        // inter(3);
        
        forward();
  
        while(gain(lefWingSensor) || gain(rgtWingSensor));
      }

      i++;
      if(i == n){
        charge();
      }
    }
  }
  
  while(1);
}

void charge(){
  inter(5);
  
  forward();
  
  while(1){
    if(!gain(midLefSensor) && !gain(midRgtSensor)){
      forward();
      while(1);
    }
    
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

    delay(100);
  }
}

void pulse(int pin, int interval){
  digitalWrite(pin, 1);
  delayMicroseconds(interval);
  digitalWrite(pin, 0);
}

void bacward(){
  pulse(lefWheel, bacSpeed);
  delay(1);
  pulse(rgtWheel, bacSpeed);
}

void forward(){
  pulse(lefWheel, forSpeed);
  delay(1);
  pulse(rgtWheel, forSpeed);
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

