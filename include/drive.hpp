#ifndef _DRIVE_H_
#define _DRIVE_H_


void left(int sp);
void right(int sp);
void leftVel(int vel);
void rightVel(int vel);

int get_rightVel();
void reset();
int drivePos();
void autoRollers(int time);
void turnDrive(int degree);
void turnDrive2(int time);
void turnDrive3(int degree, int turnOther);

void leftSlew(int leftTarget);
void rightSlew(int rightTarget);

void slop(int sp);

bool isDriving();
void driveAsync(int sp);
void turnAsync(int sp);
void drive(int sp);
void turn(int sp);
void slowDrive(int sp, int dp);

void setSlant(int s);
void setSpeed(int sp);
void setBrakeMode(int mode);
void setCurrent(int current);

void driveTask(void* parameter);
void turnTask(void* parameter);
int drivePosLeft();

void driveOp();

#endif
