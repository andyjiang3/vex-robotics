#include "main.h"

#define MAX 127


/*
Drive Code 
By: Andy Jiang

*/

/*===================================================================================*/
//DEFINE
Motor leftf(LEFT_FRONT, MOTOR_GEARSET_6, 1, MOTOR_ENCODER_DEGREES);
Motor leftb(LEFT_BACK, MOTOR_GEARSET_6, 1, MOTOR_ENCODER_DEGREES);
Motor rightf(RIGHT_FRONT, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_DEGREES);
Motor rightb(RIGHT_BACK, MOTOR_GEARSET_6, 0, MOTOR_ENCODER_DEGREES);
Imu imu(IMU);

double scale = 1.0;



//friction coefficient
const double fc = 1;
static int driveMode = 1;
static int driveTarget = 0;
static int turnTarget = 0;
static int maxSpeed = MAX;
static int slant = 0;



/*===================================================================================*/
//BASIC DRIVE CONTROL

void left(int sp) {
  leftf.move(sp);
  leftb.move(sp);
}

void right(int sp) {
  rightf.move(sp);
  rightb.move(sp);
}

void leftVel(int vel) {
  leftf.move_velocity(vel);
  leftb.move_velocity(vel);
}

void rightVel(int vel) {
  rightf.move_velocity(vel);
  rightb.move_velocity(vel);
}

void turnDrive(int degree) {
  //clockwise is positive
  int sp = 45;
  int direction = abs(degree)/degree;
  if (mirror) {    //blue = -sp     red = sp
    sp = -sp;
  }
  if (sp < 0) {
    while (imu.get_rotation() > -degree) {     //-108   rotation: getting more neg
      left(direction*sp);
      right(-direction*sp);
    }
  } else {
    while (imu.get_rotation() < degree) {
      left(direction*sp);       //sp is postive
      right(-direction*sp);
    }
  }
}

void turnDrive3(int degree, int turnOther) {
  //clockwise is positive
  int sp = 45 * turnOther;
  int direction = abs(degree)/degree;
  if (mirror) {    //blue = -sp     red = sp
    sp = -sp;
  }
  if (sp < 0) {
    while (imu.get_rotation() > -degree) {     //-108   rotation: getting more neg
      left(direction*sp);
      right(-direction*sp);
    }
  } else {
    while (imu.get_rotation() < degree) {
      left(direction*sp);       //sp is postive
      right(-direction*sp);
    }
  }
}

void turnDrive2(int time) {
  int sp = 100;
  if (mirror) {
    sp = -sp;
  }
  leftf.move(-sp);
  leftb.move(-sp);
  rightf.move(sp);
  rightb.move(sp);
  delay(time);
  left(0);
  right(0);
}




/*===================================================================================*/
//BASIC FUNC

void reset() {
  maxSpeed = MAX;
  slant = 0;
  driveTarget = 0;
  turnTarget = 0;

  //set zero position to current position
  leftf.tare_position();
  leftb.tare_position();
  rightf.tare_position();
  rightb.tare_position();

  left(0);
  right(0);

}

int drivePos() {
  return (leftf.get_position() + rightf.get_position())/2;
}

int drivePosLeft() {
  return leftb.get_position();
}

int get_rightVel() {
  return rightf.get_actual_velocity();
}


/*===================================================================================*/
//SLEW CONTROL: Adjust to certain speed through steps, control current to motor. Prevent fluctuation.

const int accel_step = 9;     //acceleration - amount to increase each step.
const int deccel_step = 256;  // decceleration - no decel slew
static int leftSpeed = 0;
static int rightSpeed = 0;

void leftSlew(int leftTarget) {
  int step;

  //initial
  //if current speed less than target, acc by steps of 9.
  if (abs(leftSpeed) < abs(leftTarget)) {
    step = accel_step;
  } else {
    step = deccel_step;
  }

  //add step if target is greater. opposite for less.
  if (leftTarget > leftSpeed + step) {
    leftSpeed += step;
  } else if (leftTarget < leftSpeed + step) {
    leftSpeed -= step;
  } else {
    leftSpeed = leftTarget;
  }

  left(leftSpeed);

}

//Slew rate
void rightSlew(int rightTarget) {
  int step;

  if (abs(rightSpeed) < abs(rightTarget)) {
    step = accel_step;
  } else {
    step = deccel_step;
  }

  if (rightTarget > rightSpeed + step) {
    rightSpeed += step;
  } else if (rightTarget < rightSpeed + step) {
    rightSpeed -= step;
  } else {
    rightSpeed = rightTarget;
  }

  right(rightSpeed);

}

/*===================================================================================*/
//SLOP CORRECTITON


void slop(int sp) {
  driveMode = 2;
  if (sp < 0) {
    right(-30);
    delay(100);
  }
  driveMode = 1;
}

/*===================================================================================*/
//BASIC AUTO FUNC

//feedback from drive
bool isDriving() {
  static int count = 0;
  static int last = 0;
  static int lastTarget = 0;

  int curr = leftb.get_position();

  int target = turnTarget;

  if (driveMode == 1) {
    target = driveTarget;
  }

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

void driveAsync(int sp) {
  reset();
  driveTarget = sp;
  driveMode = 1;
}

void turnAsync(int sp) {
    if (mirror) {
      sp = -sp;
    }
  reset();
  turnTarget = sp;
  driveMode = 0;
}

void drive(int sp) {
  driveAsync(sp);
  delay(450);
  while(isDriving()) delay(20);
}

void turn(int sp) {
  turnAsync(sp);
  delay(450);
  while(isDriving()) delay(20);
}

//dp when to slow drive
void slowDrive(int sp, int dp) {
  driveAsync(sp);


  if (sp > 0) {
    while(drivePos() < dp) delay(20);
  } else {
    while(drivePos() > dp) delay(20);
  }

  setSpeed(43);
  while(isDriving()) delay(20);
}



/*===================================================================================*/
//MODIFIERS

void setSpeed(int sp) {
  maxSpeed = sp;
}

void setSlant(int s) {
  if (mirror) {
    s = -s;
  }

  slant = s;
}

void setBrakeMode(int mode) {
  motor_brake_mode_e_t brakeMode;
  switch(mode){
    case 0:
      brakeMode = MOTOR_BRAKE_COAST;
      break;
    case 1:
      brakeMode = MOTOR_BRAKE_BRAKE;
      break;
    case 2:
      brakeMode = MOTOR_BRAKE_HOLD;
      break;
   }

   leftf.set_brake_mode(brakeMode);
   leftb.set_brake_mode(brakeMode);
   rightf.set_brake_mode(brakeMode);
   rightb.set_brake_mode(brakeMode);

}

void setCurrent(int current) {
  rightf.set_current_limit(current);
  rightb.set_current_limit(current);
  leftf.set_current_limit(current);
  rightb.set_current_limit(current);
}


/*===================================================================================*/
//PID
void driveTask(void* parameter) {
  int prevError = 0;

  while(1) {
    delay(20);

    //make sure its in drive mode
    if (driveMode != 1) {
      continue;
    }

    int sp = driveTarget;

    //not using integral
    double kp = 0.08;  //.053
    double kd = 0.01;    //.02

    int sv = leftb.get_position();

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

    left(speed - slant);
    right(speed + slant);

  }
}

void turnTask(void* parameter) {
  int prevError;

  while(1) {
    delay(20);

    if (driveMode != 0) {
      continue;
    }

    int sp = turnTarget;


    if(sp > 0)
      sp *= 2.35;
    else
      sp *= 2.3;

    double kp = .9*fc;
    double kd = 3.5/fc;

    int sv = (leftf.get_position() - rightf.get_position())/2;
    int error = sp-sv;
    int derivative = error - prevError;
    prevError = error;
    int speed = error*kp + derivative*kd;

    if(speed > maxSpeed) {
      speed = maxSpeed;
    }

    if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }

    leftSlew(speed);
    rightSlew(-speed);


  }
}
/*===================================================================================*/
//OPERATOR CONTROL
void driveOp() {
  /*
  int leftJoy = ((master.get_analog(ANALOG_LEFT_Y))*pow(abs((master.get_analog(ANALOG_LEFT_Y))), 2)/10000)/4.5;
  int rightJoy = ((master.get_analog(ANALOG_RIGHT_Y))*pow(abs((master.get_analog(ANALOG_RIGHT_Y))), 2)/10000)/4.5;
  */

  /*
  int leftJoy = (master.get_analog(ANALOG_LEFT_Y)) * .78740157;
  int rightJoy = (master.get_analog(ANALOG_RIGHT_Y)) * .78740157;



  int leftJoy = ((master.get_analog(ANALOG_LEFT_Y)) * abs(master.get_analog(ANALOG_LEFT_Y)))/150;
  int rightJoy = ((master.get_analog(ANALOG_RIGHT_Y)) * abs(master.get_analog(ANALOG_RIGHT_Y)))/150;
  */
  //(((master.get_analog(ANALOG_LEFT_Y)) * abs(master.get_analog(ANALOG_LEFT_Y)))/130)*2
  //(((master.get_analog(ANALOG_RIGHT_Y)) * abs(master.get_analog(ANALOG_RIGHT_Y)))/130)*2

  //(master.get_analog(ANALOG_LEFT_Y)) * 2.2

  if (master.get_digital(DIGITAL_UP)) {
    if (scale == 1.0) {
      scale = 0.5;
      master.rumble("-");
    } else {
      scale = 1.0;
      master.rumble("-");
    }
  }

  setCurrent(2500);
  setBrakeMode(0);

  int leftJoy = master.get_analog(ANALOG_LEFT_Y) * scale;
  int rightJoy = master.get_analog(ANALOG_RIGHT_Y) * scale;



  left(leftJoy);
  right(rightJoy);
}
