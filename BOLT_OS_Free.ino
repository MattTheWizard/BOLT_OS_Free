////////////////////////////////////////////
// BOLT OS 1.0 Free edition
// 
// Make (almost) any electric foam dart blaster select fire
//
// This is the free version with the following features:
// * Reliable and fast code for super snappy response
// * Support for a 2 position selector switch
// * Choose between single fire and full auto
//
// There is also a premium version with even more awesome features:
// * Semi / Semi Binary / 2 rnd Burst / 3 round Burst/ 5 round Burst / Full auto support
// * Complete freedom in combination of firemodes (Yes, you can have double burst, or semi and binary semi)
// * Rate of fire reduction, 0 - 5 (get super snappy semi auto with a fast motor, but don't go through a mag in 1 second on full auto)
// * pusher crash detection (to make sure you don't accidentally burn out your pusher motor).
// * Easily adjustable hardware settings via the selector switch and rev trigger. No other buttons, LCD or laptop needed!

// Interested? Check out the premium version on my youtube here: 
// Matt the foam blaster wizard
// https://www.youtube.com/channel/UCnzgV6HDINqco_RPyLQLHTg 
///////////////////////////////////////////

#include <Bounce2.h>

// set constants
const int triggerPin = A2;
const int pusherPin = A4;
const int fireModePin = A5;
const int mosfetPin = A3;

// set select fire variables
int fireMode = 1; // (the value set here is the default mode, it will change immediately based on the firemode pin settings) 1 = Semi, 2 = Full auto

// set the bounce object
Bounce pusher = Bounce();
Bounce trigger = Bounce();
Bounce fireModeSelector = Bounce();

//// Functions ////

void performShotCycle() {
  
  digitalWrite(mosfetPin, HIGH); // start pushing

  while(true) {  
    pusher.update();
    if(pusher.fell()) {
      break;
    }
  }
  
  digitalWrite(mosfetPin, LOW); // stop pushing
}

//// main part ////

void setup() {
  pinMode(mosfetPin, OUTPUT);
  digitalWrite(mosfetPin, LOW); 
  
  pinMode(triggerPin, INPUT_PULLUP);
  trigger.attach(triggerPin);
  trigger.interval(5);

  pinMode(pusherPin, INPUT_PULLUP);
  pusher.attach(pusherPin);
  pusher.interval(5);

  pinMode(fireModePin,INPUT_PULLUP);
  fireModeSelector.attach(fireModePin);
  fireModeSelector.interval(5);
}

void loop() { 

  // Select fire mode
  fireModeSelector.update();
  
  if(fireModeSelector.read() == LOW) {
    fireMode = 1;
  } else if (fireModeSelector.read() == HIGH) {
    fireMode = 2;
  }

  // check for trigger press
  trigger.update();

  // Shoot Dart
  if(trigger.fell()) {
    if(fireMode == 1) {
       performShotCycle();
   } else if(fireMode == 2) {
        while(trigger.read() == LOW) {
           performShotCycle(); 
           trigger.update();
        }
    }
  }
}
