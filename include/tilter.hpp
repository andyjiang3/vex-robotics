#ifndef _TILTER_H_
#define _TILTER_H_

void move_tilter(int sp);
int get_tilt_pos();
void tilt_reset();
void set_tilt_tare();
void moveTilter(int en);
void moveTilter2();

bool isTilting();
void tiltAsync(int sp);
void tilt(int sp);
void tiltTask(void* parameter);

void tilterOp();

#endif
