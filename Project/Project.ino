#define A      8
#define A_bar    A4
#define B        9
#define B_bar    A5 //motor pins 

#define BCD0 0
#define BCD1 1
#define BCD2 4
//#include <Stepper.h>
                      //A+ A- B+  B-
//Stepper myStepper(128, A, A_bar, B, B_bar);

int bcd[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
int up_orders[8]= {-1,-1,-1,-1,-1,-1,-1,-1};
int down_orders[8]= {-1,-1,-1,-1,-1,-1,-1,-1};
int Keypad_orders[8]= {-1,-1,-1,-1,-1,-1,-1,-1};

int angle;
int pwm;
int count = 0 ;
int floor_number = 0;
int CurrentFloor = 0;
bool moving = false;
void setup()
{
  Serial.begin(9600);
  //myStepper.setSpeed(60);

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(A, OUTPUT);
  pinMode(A_bar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(B_bar, OUTPUT); //motor
  pinMode(A0, OUTPUT);

  //KEYPAD
  pinMode(BCD0, OUTPUT);
  pinMode(BCD1, OUTPUT);
  pinMode(BCD2, OUTPUT);

  //PUSH BUTTONS
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  //Initialize Servos
  for(int i = 7; i > -1; i--)
  {
    int angle = 0;
    pwm = (angle * 11) + 500;    // Convert angle to microseconds
    digitalWrite(A0, 1);
    digitalWrite(A1, bcd[i][0]);
    digitalWrite(A2, bcd[i][1]);
    digitalWrite(A3, bcd[i][2]);
    delayMicroseconds(pwm);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A0, LOW);
    myDelay(50);
  }
  attachInterrupt(digitalPinToInterrupt(2), readFromKeypad, HIGH) ;
  attachInterrupt(digitalPinToInterrupt(3), readFromButtons, HIGH) ;
}

void loop ()
{
  if(up_orders[7] == 1) //No up for 7th floor
    up_orders[7] == -1;
  if(down_orders[0] == 1) //No down for Ground floor
    down_orders[0] == -1;
  if(floor_number != CurrentFloor)
    {
      goToLevel(floor_number);
      if(up_orders[CurrentFloor] == 1 || down_orders[CurrentFloor] == 1 || Keypad_orders[CurrentFloor] == 1)
      {
        door(CurrentFloor);
        up_orders[CurrentFloor] == -1;
        down_orders[CurrentFloor] == -1;
        Keypad_orders[CurrentFloor] == -1;
      }
      if(!moving)
      {
        for(int i = 0; i < 8; i++)
        {
          if(up_orders[i] == 1 || down_orders[i] == 1 || Keypad_orders[i] == 1)
          {
            floor_number = i;
            //Serial.print("7asal ");
            //Serial.println(i);
            break;             
          }
        }
      }
    }
   else
   {
        if(up_orders[CurrentFloor] == 1 || down_orders[CurrentFloor] == 1 || Keypad_orders[CurrentFloor] == 1)
        {
          //Serial.print("7asal ");
          //Serial.println("ssssssss");
          door(CurrentFloor);
          up_orders[CurrentFloor] = -1;
          down_orders[CurrentFloor] = -1 ;
          Keypad_orders[CurrentFloor] = -1;
        }
        
   }
    //STEPPER
    /*myStepper.step(128);
    delay(500);
    myStepper.step(-128);
     delay(500); */

  //SERVO
  /*for (angle = 0; angle <= 180; angle += 5)  {
     servoPulse(servo, angle);  }
    for (angle = 180; angle >= 0; angle -= 5)  {
     servoPulse(servo, angle);  }*/

  /* for (int i = 0; i < 16 ; i ++) // motor steps 64
    {
     mystep();
    }*/

  // delay(1000);
  //  Serial.print("works");

  /*  for (int i = 0; i < 8 ; i ++) /////////
    {
      door(i);
    }*/


}

void door(int x) {
  for (angle = 0; angle <= 180; angle += 5)  {
    pwm = (angle * 11) + 500;    // Convert angle to microseconds
    digitalWrite(A0, HIGH);
    digitalWrite(A1, bcd[x][0]);
    digitalWrite(A2, bcd[x][1]);
    digitalWrite(A3, bcd[x][2]);
    delayMicroseconds(pwm);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    myDelay(50);            ;
  }
  for (angle = 180; angle >= 0; angle -= 5)  {
    pwm = (angle * 11) + 500;    // Convert angle to microseconds
    digitalWrite(A0, HIGH);
    digitalWrite(A1, bcd[x][0]);
    digitalWrite(A2, bcd[x][1]);
    digitalWrite(A3, bcd[x][2]);
    delayMicroseconds(pwm);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    myDelay(50);
  }
  up_orders[x] == -1;
  down_orders[x] == -1;
  Keypad_orders[x] == -1;

}
//Interrupt 3
void readFromButtons() {
    int floor_number_3,input;
   if (digitalRead(3) == HIGH){
    int a = digitalRead(10); //msb
    int b = digitalRead(11);
    int c = digitalRead(12);
    int d = digitalRead(13);  //LSB
    input = (a * 8) + (b * 4) + (c*2) + d;
  
    if(input == 15)
    input = 0;
  
    if (input%2 ==0  )// order tale3
    {
      floor_number_3 = input/2;
      up_orders[floor_number_3] = 1;
    }
    else  //order nazel
    { 
      floor_number_3=(input /2) + 1;
      down_orders[floor_number_3] = 1;
    }
          //Serial.println(input);
    Serial.println(floor_number_3);
    if(!moving)
      {
        floor_number = floor_number_3; //Call the elevator
      }
    }
    
  }

//Interrupt 2
void readFromKeypad()
{
  if (digitalRead(2) == HIGH) {
    int a = digitalRead(5); //msb
    int b = digitalRead(6);
    int c = digitalRead(7); //lsb
    floor_number = (a * 4) + (b * 2) + (c);
    Keypad_orders[floor_number] = 1;
  }
}
void goToLevel(int level)
{
    moving = true;
    int sign = (level > CurrentFloor)? 1:-1;
    int Iterations = level - CurrentFloor;
    for(int x = 0; x < abs(Iterations); x++)
    {
      //myStepper.step(sign*128);
      if(sign == 1)
      {
        for(int y = 0; y < (128/4); y++)
          mystep();
      }
      else if(sign == -1)
      {
        for(int y = (128/4); y > 0; y--)
           mybackstep();
      }
      CurrentFloor = CurrentFloor + sign;
      showonBCD(CurrentFloor);
      if(sign == 1)
      {
          if(up_orders[CurrentFloor] == 1)
          {
            door(CurrentFloor);
            up_orders[CurrentFloor] = -1;
          }
      }
      else if(sign == -1)
      {
        if(down_orders[CurrentFloor] == 1)
          {
            door(CurrentFloor);         
            down_orders[CurrentFloor] = -1;
          }
      }
      if(Keypad_orders[CurrentFloor] == 1)
      {
        door(CurrentFloor);         
        Keypad_orders[CurrentFloor] = -1;                
      }
      myDelay(500);
    }
    if(CurrentFloor == 7)
      {
        door(CurrentFloor);
        down_orders[CurrentFloor] = -1;
      }
    
    if(CurrentFloor == 0)
      {
        door(CurrentFloor);
        up_orders[CurrentFloor] = -1;
      }
    moving = false;
  
}
void showonBCD(int x)
{
  digitalWrite(BCD2, bcd[x][0]); //msb
  digitalWrite(BCD1, bcd[x][1]);
  digitalWrite(BCD0, bcd[x][2]); //lsb
}

void servoPulse (int servo, int angle)
{
  pwm = (angle * 11) + 500;    // Convert angle to microseconds
  digitalWrite(servo, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(servo, LOW);
  delay(50);                   // Refresh cycle of servo
}

void myDelay(int d) //delay milliseconds
{
  unsigned long curr = millis();
  while ((millis() - curr) < d)
  {
  }
}


void mystep() //4 forward motor steps
{
  digitalWrite(A, HIGH);
  digitalWrite(A_bar, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(B_bar, LOW);
  myDelay (5);

  digitalWrite(A, LOW);
  digitalWrite(A_bar, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(B_bar, LOW);
  myDelay (5);

  digitalWrite(A, LOW);
  digitalWrite(A_bar, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, LOW);
  myDelay (5);

  digitalWrite(A, HIGH);
  digitalWrite(A_bar, LOW);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, HIGH);
  myDelay (5);


}

void mybackstep() //4 reverse motor steps
{
  digitalWrite(A, LOW);
  digitalWrite(A_bar, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, HIGH);
  myDelay (5);

  digitalWrite(A, HIGH);
  digitalWrite(A_bar, LOW);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, HIGH);
  myDelay (5);

  digitalWrite(A, LOW);
  digitalWrite(A_bar, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(B_bar, LOW);
  myDelay (5);

  digitalWrite(A, HIGH);
  digitalWrite(A_bar, LOW);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, LOW);
  myDelay (5);
}
