#include "main.h"

#define MAX 127
/*
Tilter Code
By: Andy Jiang

*/

/*===================================================================================*/
//DEFINE
Motor tilter(TILTER, MOTOR_GEARSET_36, 1, MOTOR_ENCODER_DEGREES);

static int tiltTarget = 0;
static int maxSpeed = MAX;


/*===================================================================================*/
//BASIC TILTER CONTROL

void move_tilter(int sp) {
  tilter.move(sp);
}

void moveTilter(int en) {

  move_tilter(127);
  if (en > 0) {
    while(get_tilt_pos() < en) {
      if (get_tilt_pos() > 1200) {
        move_tilter(90);
      } else {
        move_tilter(127);
      }
    }
  } else {
    while(get_tilt_pos() > en) {
      move_tilter(-127);
      delay(20);
    }
  }

  move_tilter(0);


}

void moveTilter2() {

  if (get_tilt_pos() > 400 && get_tilt_pos() < 600) {
    move_tilter(95);
  } else if (get_tilt_pos() >= 600 && get_tilt_pos() < 765) {
    move_tilter(70);
  } else if (get_tilt_pos() >= 765) {
    move_tilter(0);
  } else {
    move_tilter(127);
  }

}


int get_tilt_pos() {
  return tilter.get_position();
}

void tilt_reset() {
  maxSpeed = MAX;
  tiltTarget = 0;
  move_tilter(0);
  delay(450);
  tilter.tare_position();
}

void set_tilt_tare() {
  tilter.tare_position();
}

/*===================================================================================*/
//PID
/*
bool isTilting() {
  static int count = 0;
  static int last = 0;
  static int lastTarget = 0;

  int curr = tilter.get_position();

  int target = tiltTarget;


  if (abs(last-curr) < 3) {
    count++;
  } else {
    count = 0;
  }

  if(target != lastTarget) {
    count = 0;
  }

  lastTarget = target;
  last = curr;

  if (count > 4) {
    return false;
  } else {
    return true;
  }

}

void tiltAsync(int sp) {
  tiltTarget = sp;
}

void tilt(int sp) {
  tiltAsync(sp);
  delay(450);
  while(isTilting()) delay(20);
  if (sp < 0) {
    tilt_reset();
  }
}

void tiltTask(void* parameter) {
  int prevError = 0;

  while(1) {
    delay(20);

    //make sure its in drive mode


    int sp = tiltTarget;

    //not using integral
    double kp = 1.2;
    double kd = 0;

    int sv = tilter.get_position();

    //speed
    int error = sp-sv;
    int derivative = error - prevError;
    prevError = error;    //set to previous erorr
    int speed = error*kp + derivative*kd;

    //cap at max speed
    if(speed > maxSpeed) {
      speed = maxSpeed;
    }

    //for reverse direction
    if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }

    move_tilter(speed);

  }
}

*/

/*===================================================================================*/
//OPERATOR CONTROL

void tilterOp() {

  if (master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_R2)) {
    moveTilter2();
  } else if (master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_R2)) {

      move_tilter(-127);

  } else {
    move_tilter(0);
  }

}
