/*The motion of the gamepad is only dependent on two dimensions so using the third dimension 'z' would be redundant. 
The computation time of this function can be reduced by ~ 33% when neglecting the third dimension
and since this is a function that will be called continually, the total computation time of our program will be reduced
significantly. One additional point is that we only have two available analog ports left for use that I'm aware of on our arduino 
and I'm not really sure how we could free up another analog port.
The function will be most efficient if we do not pass a return value, but instead mutate a global variable in the call function.
I don't think it would be wise to make the function have a return parameter because this would mean initializing a variable every 
time the function is called. Since the function is being called all the time, we can lower our computation time for the program as 
a whole. 
Lastly, the global variable type I feel we should use is Boolean because for all practical purposes, our game only cares if the gamepad 
has been tilted beyond a certain threshold. This means that the computation of whether or not the gamepad is being tilted will occur
in the tiltDirection() function.*/

#include<math.h>
#include<stdio.h>
#define A_X  3 //or whatever port the x dimension is hooked up to
#define A_Y  4 //or whatever port the y dimension is hooked up to

/*Add these global variables to the main code. */
int x_val=0,y_val=0,x_bias,y_bias,tiltLimit=20; //Tilt limit is arbitrary and can be changed to whatever threshold we think is best
bool x_neg=0,x_pos=0,y_neg=0,y_pos=0;

/*Add this stuff in the setup code to the main setup code. We can hard code the bias values once the casing
has been constructed and the acceleromter has been secured to the gamepad. For now we need all this setup 
code for testing purposes.*/

void setup() {

  pinMode(A_X,INPUT);

  pinMode(A_Y,INPUT);

  Serial.begin(9600);

  for (int i=0;i<50;i++){     // We take 50 readings and find the average bias value from them
    x_val+=analogRead(A_X);delay(2);    //Delay of 2 milli because of potential noise issues from readings
    y_val+=analogRead(A_Y);delay(2);    //Same reason
  }
  x_bias = x_val/50;

  Serial.print(" X_Bias: ");
  Serial.print(x_bias);

  y_bias = y_val/50;

  Serial.print(" Y_Bias: ");
  Serial.print(y_bias);
  Serial.print("\n");
  
  x_val = 0;
  y_val = 0;
}

void loop(){
  tiltDirection();
  Serial.print("X_POS: ");
  Serial.print(x_pos);
  Serial.print(" X_NEG: ");
  Serial.print(x_neg);
  Serial.print(" Y_POS: ");
  Serial.print(y_pos);
  Serial.print(" Y_NEG: ");
  Serial.print(y_neg);
  Serial.print("\n");
  if(x_pos==true && x_neg==false && y_pos==false && y_neg==false){
    Serial.print("Tilting in the positive x direction.\n");
  }
  else if(x_pos==false && x_neg==true && y_pos==false && y_neg==false){
    Serial.print("Tilting in the negative x direction.\n");
  }
  else if(x_pos==false && x_neg==false && y_pos==true && y_neg==false){
    Serial.print("Tilting in the positive y direction.\n");
  }
  else if(x_pos==false && x_neg==false && y_pos==false && y_neg==true){
    Serial.print("Tilting in the negative y direction.\n");
  }
  else if(x_pos==false && x_neg==false && y_pos==false && y_neg==false){
    Serial.print("Not tilted past the threshold limit.\n");
  }
  else{
    Serial.print("Error!!!\n");
  }
  delay(1000);
}

void tiltDirection() {
    x_val = analogRead(A_X);delay(2); //Delayed by 2 milli in case of noise disturbance
    y_val = analogRead(A_Y);

    if(abs(x_val-x_bias) >= abs(y_val-y_bias)){ //If it's tilting more in the x direction than the y direction or if it's tilted equally in both directions (perfectly diagonal)
      y_neg=0;
      y_pos=0;
      if(x_val-x_bias > tiltLimit){
        x_neg=0;
        x_pos=1;
      }
      else if(x_val-x_bias < (-1)*tiltLimit){
        x_pos=0;
        x_neg=1;
      }
      else{
        x_pos=0;
        x_neg=0;
      }
    }
    else if(abs(y_val-y_bias) > abs(x_val-x_bias)){   //If it's tilting more in the y direction than the x direction
      x_neg=0;
      x_pos=0;
      
      if(y_val-y_bias > tiltLimit){
        y_neg=0;
        y_pos=1;
      }
      else if(y_val-y_bias < (-1)*tiltLimit){
        y_pos=0;
        y_neg=1;
      }
      else{
        y_pos=0;
        y_neg=0;
      }
    }
}
