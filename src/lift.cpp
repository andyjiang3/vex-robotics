#include "main.h"

/*
Lift Code 
By: Andy Jiang

*/

/*===================================================================================*/
//DEFINE
Motor lift(LIFT, MOTOR_GEARSET_18, 1, MOTOR_ENCODER_DEGREES);
bool lifted = false;

bool liftLowTowerGo = false;
bool liftHighTowerGo = false;

int lift_target;

/*===================================================================================*/
//BASIC LIFT CONTROL

void move_lift(int sp) {
  lift.move(sp);
}

void set_lift_tare() {
  lift.tare_position();
}


/*===================================================================================*/
//INFO FUNC

int get_lift_pos() {
  return lift.get_position();
}

void lift_reset() {
  lift.tare_position();
}


/*===================================================================================*/
//MACROS

void lowTower (void * parameter) {
  while (1) {
    if (liftLowTowerGo) {
      int target = 950;

      if (!lifted) {
        set_lift_tare();
        lifted = true;
      }

      if (target > get_lift_pos()) {
        while (target > get_lift_pos()) {

          // early out if user wants control b/c something changed -- change Controller1.ButtonX to desired values
          if (master.get_digital(DIGITAL_R2)) {
            if (master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) {
             // EXIT_BUTTON is your opcontrol button for this function
              liftLowTowerGo = false;
              break;
            }
          }
          lift.move(127);
          delay(10);
        }
        lift.move(0);
      } else {
        while (target < get_lift_pos()) {
          // early out if user wants control b/c something changed -- change Controller1.ButtonX to desired values
          if (master.get_digital(DIGITAL_R2)) {
            if (master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) {
             // EXIT_BUTTON is your opcontrol button for this function
              liftLowTowerGo = false;
              break;
            }
          }
          lift.move(-127);
          delay(10);
        }
        lift.move(0);
      } // if target
      liftLowTowerGo = false;
    } // if GO

    pros::delay(20); // only check lowTowerGo every 20 ms
  } // while (1)
} // lowTower

void highTower (void * parameter) {
  while (1) {
    if (liftHighTowerGo) {
      int target = 1200;

      if (!lifted) {
        set_lift_tare();
        lifted = true;
      }

      if (target > get_lift_pos()) {
        while (target > get_lift_pos()) {

          // early out if user wants control b/c something changed -- change Controller1.ButtonX to desired values
          if (master.get_digital(DIGITAL_R2)) {
            if (master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) {
             // EXIT_BUTTON is your opcontrol button for this function
              liftHighTowerGo = false;
              break;
            }
          }
          lift.move(127);
          delay(10);
        }
        lift.move(0);
      } else {
        while (target < get_lift_pos()) {
          // early out if user wants control b/c something changed -- change Controller1.ButtonX to desired values
          if (master.get_digital(DIGITAL_R2)) {
            if (master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) {
             // EXIT_BUTTON is your opcontrol button for this function
              liftHighTowerGo = false;
              break;
            }
          }
          lift.move(-127);
          delay(10);
        }
        lift.move(0);
      } // if target
      liftHighTowerGo = false;
    } // if GO

    pros::delay(20); // only check lowTowerGo every 20 ms
  } // while (1)
} // lowTower








/*===================================================================================*/
//PID

void set_lift_pid(int target) {
  lift_target = target;
}

void lift_pid() {
  while (true) {
    move_lift((lift_target-lift.get_position())*0.5);
  }
}






/*===================================================================================*/
//MODIFIERS



/*===================================================================================*/
//OPERATOR CONTROL
bool trayUp;

void liftOp() {


  if (master.get_digital(DIGITAL_R2)) {
    if (master.get_digital(DIGITAL_L1)) {

      lift.set_brake_mode(MOTOR_BRAKE_HOLD);
      move_lift(127);


    } else if (master.get_digital(DIGITAL_L2)) {


        lifted = false;
        lift.set_brake_mode(MOTOR_BRAKE_COAST);
        move_lift(-127);

    } else {
      lift.set_brake_mode(MOTOR_BRAKE_HOLD);
      move_lift(0);

    }
  }


  if (master.get_digital(DIGITAL_RIGHT)) {
    lift.set_brake_mode(MOTOR_BRAKE_HOLD);
    liftHighTowerGo = true;

  } else if (master.get_digital(DIGITAL_Y)) {
    lift.set_brake_mode(MOTOR_BRAKE_HOLD);
    liftLowTowerGo = true;

  }


}
