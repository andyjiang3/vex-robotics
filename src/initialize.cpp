#include "main.h"

int auton = 0; //which auton is selected
bool mirror = false;


void autonScreen(void* parameter) {
  delay(200);

  const int autoCount = 6;
  const char *autoNames[autoCount] = {
    "big tower",
    "skinny tower",
    "push",
    "flip out",
    "skills",
    "none"
  };


  lcd::initialize();
  lcd::set_text(0, "Select an Auton");
  lcd::print(2, "%s", autoNames[auton]);
  lcd::print(4, "%s", mirror ? "blue" : "red");

  imu.reset();
  std::size_t iter = 0;
  auto time = pros::millis();

  while (imu.is_calibrating()) {
  lcd::print(5, "IMU is calibrating (%u)", iter);
  //lcd::print(6, "status: %d", imu.get_status());
  delay(10);
  iter += 10;
  }
  lcd::print(5, "IMU done calibrating (took %u ms)", pros::millis() - time);


  while(true) {

    if(master.get_digital(DIGITAL_A)) {
      auton++;
      if (auton == autoCount) {
        auton = 0;
      }
        lcd::print(2, "%s", autoNames[auton]);
        while(master.get_digital(DIGITAL_A)) delay(20);
     delay(300);

    }

    if(master.get_digital(DIGITAL_B)) {
      mirror = !mirror;
      lcd::print(4, "%s", mirror ? "blue" : "red");
      while(master.get_digital(DIGITAL_B)) delay(20);
      delay(300);
    }


    //lcd::print(5, "%d", get_tilt_pos());
    lcd::print(6, "%d", get_rollers_pos());





    //pros::lcd::print(6, "heading: %05.2f", imu.get_heading());
		pros::lcd::print(7, "degrees: %05.2f deg", imu.get_rotation());
		auto gyro = imu.get_gyro_rate();
		pros::lcd::print(8, "gyro:{%03.2f,%03.2f,%03.2f}", gyro.x, gyro.y, gyro.z);



    delay(50);
  }

}

void initialize() {
  Task lcd_task(autonScreen, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "lcd_task");
  Task lT_task(lowTower, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "lT_task");
  Task hT_task(highTower, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "hT_task");
  //Task rT_task(indexCube, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "rT_task");
}

void disabled() {}

void competition_initialize() {}
