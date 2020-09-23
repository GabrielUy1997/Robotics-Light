#include <Sparki.h>
#define NO_MAG
//NO_MAG to save memory since the magnetometer is not being used
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
void loop() {
  // put your main code here, to run repeatedly:
  sparki.clearLCD(); // wipe the screen
  left   = sparki.lightLeft();   // measure the left light sensor
  center = sparki.lightCenter(); // measure the center light sensor
  right  = sparki.lightRight();  // measure the right light sensor
  bool is_obstacle;
  if(count !=55)
  {
      calibrate(left,center,right);
  }
  if (count == 55)
  {
      is_obstacle = avoid_obstacles();
      if(is_obstacle == true)
      {
        sparki.moveBackward();
        delay(1000);
      }
      else
      {
        look_for_light();
        sparki.print(left);
        sparki.print(",");
        sparki.print(center);
        sparki.print(",");
        sparki.println(right);
      }
      is_obstacle = false;
  }
  
  sparki.updateLCD();
  delay(100); // wait 0.1 seconds
}
//averages the data that was collected and stored when calbrating
int average_data(int c)
{
  avg_left = avg_left + left_vals[c];
  avg_center = avg_center + center_vals[c];
  avg_right = avg_right + right_vals[c];
}
//stores the values from the sensors into array to be used in the averaging of data
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
    delay(100);
}
//makes robot rotate left for 5 seconds while it is collecting light data from the room
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
  }
}
//function that is used to make the robot move towards a source of light
void look_for_light()
{
  if ( (center > left) && (center > right) ) // if the center light is the strongest
  {  
    sparki.moveForward(); // move forward
  }
  if ( (left > center) && (left > right) )  // if the left light is the strongest
  {   
    sparki.moveLeft(); // turn left
  }
  if ( (right > center) && (right > left) )  // if the right light is the strongest
  {  
    sparki.moveRight(); // turn right
  }
}
//function used to avoid obstacles when moving
bool avoid_obstacles()
{
  int cm = sparki.ping();
  sparki.print("Distance: ");
  sparki.print(cm);
  sparki.println(" cm");

  if(cm != -1)
  {
    if(cm < 10)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}
