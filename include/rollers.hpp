#ifndef _ROLLERS_H_
#define _ROLLERS_H_

void move_rollers(int sp);
void intake();
void stopIntake();
void intake2();

int get_rollers_pos();

void intakeAsync();
void lineCubeAsync();
void intakeStopAsync();

void intakeTask(void* parameter);
void indexCube(void * parameter);

void rollersOp();

#endif
