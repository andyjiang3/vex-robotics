#include "main.h"

/*
Rollers Code 
By: Andy Jiang

*/

/*===================================================================================*/
//DEFINE

/*
static int rollerTarget = 0;
static bool rollerTargetUpdate = false;
*/
bool done = false;
bool rollerMacro = false;

static int intakeTarget = 0;
static bool intakeTargetUpdate = false;

Motor right_roller (RIGHT_ROLLER, true);
Motor left_roller (LEFT_ROLLER);


/*===================================================================================*/
//BASIC DRIVE CONTROL


void move_rollers(int sp) {
  right_roller.move(sp);
  left_roller.move(sp);
}

int get_rollers_pos() {
  return (right_roller.get_position() + left_roller.get_position()) / 2;
}

void moveRollers(int en) {
  while (get_rollers_pos() > en) {
    move_rollers(-90);
  }

}

void indexCube() {



    move_rollers(-127);
    delay(160);
    move_rollers(0);



    rollerMacro = false;
    pros::delay(20);

}


void intakeAsync() {
  intakeTarget = 1;
  intakeTargetUpdate = true;
}

void lineCubeAsync() {
  intakeTarget = 2;
  intakeTargetUpdate = true;

}

void intakeStopAsync() {
  intakeTarget = 0;
  intakeTargetUpdate = true;
}

void intake() {
  move_rollers(127);
  while(!intakeTargetUpdate) delay(10);
  move_rollers(0);
}

void lineCube() {
  move_rollers(-80);
  while(!intakeTargetUpdate) delay(10);
  move_rollers(0);
}


void intakeTask(void* parameter) {
  while(1) {
    delay(20);
    intakeTargetUpdate = false;

    switch(intakeTarget) {
      case 1:
        intake();
        break;
      case 2:
        lineCube();
        break;
    }
    intakeTarget = 0;
  }
}


/*
void intake() {
  rollerTarget = 1;
  rollerTargetUpdate = true;
}

void intake2() {
  rollerTarget = 2;
  rollerTargetUpdate = true;
}

void stopIntake() {
  rollerTarget = 0;
  rollerTargetUpdate = true;
}

void intakeCubes() {
  move_rollers(127);
  while(!rollerTargetUpdate) delay(20);
  move_rollers(0);
}

void intakeCubes2() {
  move_rollers(-50);
  while(!rollerTargetUpdate) delay(20);
  move_rollers(0);
}

void intakeTask(void* parameter) {
  while(1) {
    delay(20);
    rollerTargetUpdate = false;

    switch(rollerTarget) {
      case 1:
        intakeCubes();
        break;
      case 2:
        intakeCubes2();
        break;
    }

    rollerTarget = 0;
  }
}
*/

/*===================================================================================*/
//OPERATOR CONTROL

void rollersOp() {

  if (master.get_digital(DIGITAL_R1)) {
    if (master.get_digital(DIGITAL_R2)) {
        move_rollers(-127);
    } else {
        move_rollers(127);
    }
	} else {
		move_rollers(0);
	}

  if (master.get_digital(DIGITAL_X)) {
    indexCube();
  }

}
