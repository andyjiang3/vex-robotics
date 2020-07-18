#ifndef _LIFT_H_
#define _LIFT_H_

void move_lift(int sp);

void set_lift_pid(int target);
void lift_pid();
void lift_reset();
void set_lift_tare();

void lowTower(void* parameter);
void highTower(void* parameter);
int get_lift_pos();

void liftOp();


#endif
