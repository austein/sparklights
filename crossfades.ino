// Output
#define REDPIN A6
#define GREENPIN A5
#define BLUEPIN A7

// Color arrays
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 80, 80, 0 };
int purple[3] = { 80, 0, 80 };
int teal[3] = { 0, 80, 80 };
int dimWhite[3] = { 30, 30, 30 };
// etc.

int colors[3][20][3] = {0};
int activeSlot = 1;
int altSlot = 0;
int slotLength[3] = { 6, 6, 6};
int wait[3] = {10, 10, 10};

// Set initial color
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

//int wait = 10;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int loopCount = 60; // How often should DEBUG report?
int j = 0;          // Loop counter for repeat
int loops = 0;

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;

// Set up the LED outputs
void setup()
{
  pinMode(REDPIN, OUTPUT);   // sets the pins as output
  pinMode(GREENPIN, OUTPUT);   
  pinMode(BLUEPIN, OUTPUT); 
  
  colors[1][0][0] = 100;
  colors[1][0][1] = 0;
  colors[1][0][2] = 0;
  
  colors[1][1][0] = 100;
  colors[1][1][1] = 100;
  colors[1][1][2] = 0;
  
  colors[1][2][0] = 0;
  colors[1][2][1] = 100;
  colors[1][2][2] = 0;
  
  colors[1][3][0] = 0;
  colors[1][3][1] = 0;
  colors[1][3][2] = 50;
  
  slotLength[1] = 4;
  
  Spark.function("c", colorMod);
}

// Main program: list the order of crossfades
void loop()
{
  for (int i=0; i<slotLength[activeSlot]; i++)
  {
      crossFade(colors[activeSlot][i]);
  }
}

/* BELOW THIS LINE IS THE MATH -- YOU SHOULDN'T NEED TO CHANGE THIS FOR THE BASICS
* 
* The program works like this:
* Imagine a crossfade that moves the red LED from 0-10, 
*   the green from 0-5, and the blue from 10 to 7, in
*   ten steps.
*   We'd want to count the 10 steps and increase or 
*   decrease color values in evenly stepped increments.
*   Imagine a + indicates raising a value by 1, and a -
*   equals lowering it. Our 10 step fade would look like:
* 
*   1 2 3 4 5 6 7 8 9 10
* R + + + + + + + + + +
* G   +   +   +   +   +
* B     -     -     -
* 
* The red rises from 0 to 10 in ten steps, the green from 
* 0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.
* 
* In the real program, the color percentages are converted to 
* 0-255 values, and there are 1020 steps (255*4).
* 
* To figure out how big a step there should be between one up- or
* down-tick of one of the LED values, we call calculateStep(), 
* which calculates the absolute gap between the start and end values, 
* and then divides that gap by 1020 to determine the size of the step  
* between adjustments in the value.
*/

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(REDPIN, redVal);   // Write current values to LED pins
    analogWrite(GREENPIN, grnVal);      
    analogWrite(BLUEPIN, bluVal); 

    delay(wait[activeSlot]); // Pause for 'wait' milliseconds before resuming the loop
  }
  
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}

int delayMod(String args)
{
    FADESPEED = args.toInt();
   return FADESPEED;
}

// #runs, slot#, speed, #colors, color1, color2, ...
int colorMod(String args)
{
    int sep = args.indexOf("|");
    String start = args.substring(0,sep);
    start.trim();
    
    String starts[4] = {NULL}; // Increase to support more parameters
        splitArgStringToArray(start, starts);
    
    loops = starts[0].toInt();
    altSlot = activeSlot;
    activeSlot = starts[1].toInt();
    wait[activeSlot] = starts[2].toInt();
    slotLength[activeSlot] = starts[3].toInt();
    
    String cols[20] = {NULL};
    String argend = args.substring(sep+1);
    splitArgStringToArray(argend, cols);
    
    int colpos = 0;
    for (int i = 0; i< slotLength[activeSlot]; i++)
    {
        colors[activeSlot][i][0] = cols[colpos].toInt();
        colpos++;
        colors[activeSlot][i][1] = cols[colpos].toInt();
        colpos++;
        colors[activeSlot][i][2] = cols[colpos].toInt();
        colpos++;
    }
    
   return colpos;
}

void splitArgStringToArray(String arguments, String *target){
    int numArgs = 0;
    int beginIdx = 0;
    int idx = arguments.indexOf(",");
    
    while (idx != -1) {
        String arg = arguments.substring(beginIdx, idx);
        arg.trim();
        target[numArgs] = arg;
    
        beginIdx = idx + 1;
        idx = arguments.indexOf(",", beginIdx);
        ++numArgs;
    }
    
    // Single or last parameter
    String lastArg = arguments.substring(beginIdx);
    target[numArgs] = lastArg;
}
