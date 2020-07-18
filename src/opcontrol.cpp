#include "main.h"

Controller master(CONTROLLER_MASTER);

void opcontrol() {
	//Task tilt2_task(tiltTask, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "tilt2_task");
	bool trayUp = false;

	while (true) {

		driveOp();
    rollersOp();
    liftOp();
		tilterOp();

		if(master.get_digital(DIGITAL_DOWN)) {
			move_tilter(-127);
			move_lift(-127);
		}

		if(master.get_digital(DIGITAL_LEFT) && !competition::is_connected()) {
			autonomous();
		}

		delay(10);

	}
}
