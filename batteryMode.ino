#include <Sparki.h>
#define NO_MAG
void setup() {
  // put your setup code here, to run once:
  sparki.servo(3);
}
int count = 0;
int left_vals[55];
int center_vals[55];
int right_vals[55];
int left = 0;
int center = 0;
int right = 0;
long int avg_left = 0;
long int avg_center = 0;
long int avg_right = 0;
float battery=100;
bool charging=false;
bool is_dead=false;
bool is_bright=false;
bool is_full=false;
int cm2;
int cm3;
bool c = false;
bool l = false;
bool r = false;
void loop() {
  // put your main code here, to run repeatedly:
  sparki.clearLCD(); // wipe the screen
  left   = sparki.lightLeft();   // measure the left light sensor
  center = sparki.lightCenter(); // measure the center light sensor
  right  = sparki.lightRight();  // measure the right light sensor
  bool is_obstacle;
  bool is_obstacle_L =  false;
  bool is_obstacle_R = false;
  
  if(count !=55)
  {
      calibrate(left,center,right);
  }
  if (count == 55)
  {
      if(avg_left > 950 && avg_center > 950 && avg_right > 950)
      {
        left = left - 200;
        center = center - 200;
        right = right - 200;
        is_bright = true;
        Serial.println("bright");
        Serial.println(avg_left);
        Serial.println(avg_center);
        Serial.println(avg_right);
      }
      is_obstacle = avoid_obstacles();
      if(is_obstacle == true && charging == false && is_dead == false)
      {
        sparki.moveStop();
        delay(100);
        sparki.servo(SERVO_LEFT);
        delay(500);
        cm2 = sparki.ping();
        if(cm2 != -1)
        {
          if(cm2 < 10)
          {
            is_obstacle_L = true;
            sparki.beep(13);
          }
          else
          {
            is_obstacle_L = false;
          }
        }
        sparki.servo(SERVO_RIGHT);
        delay(500);
        cm3 = sparki.ping();
        if(cm3 != -1)
        {
          if(cm3 < 10)
          {
            is_obstacle_R = true;
            sparki.beep(13);
          }
          else
          {
            is_obstacle_R = false;
          }
        }
        if(is_obstacle_L == true && is_obstacle_R == true)
        {
          sparki.moveLeft(180);
        }
        if(is_obstacle_L == false && is_obstacle_R == true)
        {
          sparki.moveLeft(90);
        }
        if(is_obstacle_L == true && is_obstacle_R == false)
        {
          sparki.moveRight(90);
        }
        if(is_obstacle_L == false && is_obstacle_R == false)
        {
          int turn = random(2);
          if(turn == 0)
          {
            sparki.moveRight(90);
          }
          if(turn == 1)
          {
            sparki.moveLeft(90);
          }
        }
        sparki.servo(3);
        delay(100);            
      }

      
      else if(battery > 60 && charging == false)
      {
        full_charge();
        sparki.moveForward();
        sparki.print(left);
        sparki.print(",");
        sparki.print(center);
        sparki.print(",");
        sparki.println(right);
        sparki.println(" ");
        sparki.print(avg_left);
        sparki.print(",");
        sparki.print(avg_center);
        sparki.print(",");
        sparki.println(avg_right);
        sparki.RGB(RGB_GREEN);
      }
      else
      {
        if(is_dead == true && charging == false)
        {
          sparki.moveStop();
        }
        look_for_light();
        sparki.print(left);
        sparki.print(",");
        sparki.print(center);
        sparki.print(",");
        sparki.println(right);
        sparki.println(" ");
        sparki.print(avg_left);
        sparki.print(",");
        sparki.print(avg_center);
        sparki.print(",");
        sparki.println(avg_right);
      }
      is_obstacle = false;
      //is_bright = false;
      if(!charging)
      {
        if(battery>0)
        {
          battery=battery-0.5;
        }
      }
      if(charging)
      {
        if(battery < 100)
        {
          battery++;  
        }
        if(battery >= 100)
        {
          charging = false;
        }
      }
      if(battery>0)
      {
        sparki.print("Battery: ");
        sparki.println(battery);
      }
      else
      {
        sparki.println("LOW BATTERY");
        sparki.print("Battery: ");
        sparki.println(battery);
        sparki.beep(); // beep!
      }
  
  if(battery <=2)
  {
    sparki.moveStop();
    is_dead = true;
  }
  else if(battery !=0)
  {
    is_dead = false;
  }
  }
  sparki.updateLCD();
  delay(100); // wait 0.1 seconds
  
}

int average_data(int c)
{
  avg_left = avg_left + left_vals[c];
  avg_center = avg_center + center_vals[c];
  avg_right = avg_right + right_vals[c];
}
int collectdata(int l,int c,int r){
    if(count < 55){
    left_vals[count] = l;
    center_vals[count] = c;
    right_vals[count] = r;
    sparki.print("Left   ");
    sparki.println(left_vals[count]); 
    Serial.print(left_vals[count]);
    Serial.print(",");
    sparki.print("Center ");
    sparki.println(center_vals[count]);
    Serial.print(center_vals[count]);
    Serial.print(",");
    sparki.print("Right  ");
    sparki.println(right_vals[count]);
    Serial.print(right_vals[count]);
    Serial.print(",");
    Serial.println(count);
    average_data(count);
    count++;
    }
    delay(50); 
}

int calibrate(int left,int center,int right)
{
  if(count != 55) //scans for 55 
  {
    sparki.moveLeft();
  }
  collectdata(left,center,right);
  if(count == 55)
  {
    avg_left = avg_left/55;
    avg_center = avg_center/55;
    avg_right = avg_right/55;
    Serial.println("avgs: ");
    Serial.print(avg_left);
    Serial.print(",");
    Serial.print(avg_center);
    Serial.print(",");
    Serial.println(avg_right);   
    sparki.moveStop();
    sparki.moveLeft(27);
  }
  
}
void look_for_light()
{
  if ( (center > left) && (center > right) ) // if the center light is the strongest
  {  
    sparki.moveForward(); // move forward
    c = true;
   // delay(500);
  }
  if ( (left > center) && (left > right) )  // if the left light is the strongest
  {   
    sparki.moveLeft(); // turn left
    l = true;
  }
  if ( (right > center) && (right > left) )  // if the right light is the strongest
  {  
    sparki.moveRight(); // turn right
    r = true;
  }
 if(l == true && r == true && c == false)
 {
    sparki.moveForward();
    l = false;
    r = false;
 }
 c = false;
  if(is_bright == true)
  {
    if((right > 800) && (center > 800) && (left > 800))
    {
      sparki.moveStop();
      if(!charging)
      {
         charge();
      }
    }
    else
    {
      charging=false;
      sparki.RGB(RGB_RED);
    }
  }
  if(is_bright == false)
  {
    if ( (right > 999) && (center > 999) && (left > 999) && is_bright == false )
    {
      sparki.moveStop();
      if(!charging)
      {
        charge();
      }
    }
   else
    {
      charging=false;
      sparki.RGB(RGB_RED);
    }
  }
}
void full_charge()
{
    if ( (center > left) && (center > right) ){  // if the center light is the strongest
    sparki.moveForward();
  }

  if ( (left > center) && (left > right) ){    // if the left light is the strongest
    sparki.moveRight();
  }

  if ( (right > center) && (right > left) ){   // if the right light is the strongest
    sparki.moveLeft();
  }
    sparki.moveForward();
  if(is_bright == true)
  {
    if((right > 805) && (center > 805) && (left > 805) && charging == false && is_full == false)
    {
      sparki.moveStop();
      if(!charging)
      {
         charge();
      }
    }
    else
    {
      charging=false;
      //sparki.RGB(RGB_RED);
    }
  }
  if(is_bright == false)
  {
    if ( (right > 999) && (center > 999) && (left > 999) && is_bright == false && charging == false && is_full == false )
    {
      sparki.moveStop();
      if(!charging)
      {
        charge();
      }
    }
   else
    {
      charging=false;
      //sparki.RGB(RGB_RED);
    }
  }
  delay(100);
}
bool avoid_obstacles()
{
  int cm = sparki.ping();
  sparki.print("Distance: ");
  sparki.print(cm);
  sparki.println(" cm");

  if(cm != -1)
  {
    if(cm < 9)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  delay(100);
}
void charge()
{
  sparki.moveStop();
  charging=true;
  sparki.RGB(RGB_GREEN);
  if(battery >= 100)
  {
    charging=false;
    is_full=true;
    sparki.beep();
  }
  else if(battery < 60)
  {
    is_full=false;
  }
}


