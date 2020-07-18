#include "main.h"
#include <chrono>
//using namespace std::chrono;


void bigTower() {

  intakeAsync();
  delay(535);
  intakeStopAsync();
  delay(850);

  //intake row one cubes (4)
  intakeAsync();
  slowDrive(1.9 TL, 0.1 TL);
  delay(700);
  intakeStopAsync();
  reset();


  drive(-0.75 TL);


  turnDrive(109);

  lineCubeAsync();
  delay(410);
  intakeStopAsync();


  slowDrive(1.05 TL, 0.1 TL);
  moveTilter(765);
  drive(0.2 TL);

  drive(-0.3 TL);

  /*
  //line up with corner
  slowDrive(-1.0 TL, -0.1 TL);
  turnDrive(108);

  //move cube to bottom of rollers
  intake2();
  delay(430);
  stopIntake();

  //stack cube (5 cube tower)
  slowDrive(0.75 TL, 0.1 TL);
  moveTilter(1300);

  //drive outwards
  drive(-0.3 TL);
  */


}

void flipOut() {
  moveTilter(140);
  delay(1000);
  moveTilter(-1);

}

void push() {
  drive(-0.8 TL);
  drive(1.1 TL);
}

void skinnyTower() {

  intakeAsync();
  delay(535);
  intakeStopAsync();
  delay(850);

  intakeAsync();
  slowDrive(1.1 TL, 0.1 TL);
  delay(200);
  intakeStopAsync();
  reset();

  turnDrive3(85, -1);

  
}

void skills() {




  intakeAsync();
  delay(535);
  intakeStopAsync();
  delay(850);

  //intake row one cubes (4)
  intakeAsync();
  slowDrive(1.9 TL, 0.1 TL);
  delay(700);
  intakeStopAsync();
  reset();


  drive(-0.75 TL);


  turnDrive(107);

  lineCubeAsync();
  delay(410);
  intakeStopAsync();


  slowDrive(1.05 TL, 0.1 TL);
  moveTilter(765);
  drive(0.2 TL);

  drive(-0.4 TL);

  moveTilter(0);

//  turnDrive(220);


}

void none() {

}


void autonomous() {

  reset();

  Task drive_task(driveTask, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "drive_task");
  Task turn_task(turnTask, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "turn_task");
  Task intake_task(intakeTask,nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "intake_task" );

  //auto start = high_resolution_clock::now();
  switch(auton) {
    case 0:
      bigTower();
      break;
    case 1:
      skinnyTower();
      break;
    case 2:
      push();
      break;
    case 3:
      flipOut();
      break;
    case 4:
      skills();
      break;
    case 5:
      none();
      break;
    }

  /*
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << duration.count() << std::endl;
  */

  drive_task.remove();
  turn_task.remove();
  intake_task.remove();

  reset();

}
