#include "SimpleHumanoidController.h"

SimpleHumanoidController::SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm,
                                                   double dt,
                                                   const mc_rtc::Configuration & config)
: mc_control::MCController(rm, dt)
{
  jointIndex = robot().jointIndexByName(jointName);

  config_.load(config);
  solver().addConstraintSet(contactConstraint);
  solver().addConstraintSet(kinematicsConstraint);
  solver().addTask(postureTask);
  solver().setContacts({{}});

  mc_rtc::log::success("SimpleHumanoidController init done ");
}

bool SimpleHumanoidController::run()
{
  if(std::abs(postureTask->posture()[jointIndex][0] - robot().mbc().q[jointIndex][0]) < 0.05)
  {
    switch_target();
  }
  return mc_control::MCController::run();
}

void SimpleHumanoidController::reset(const mc_control::ControllerResetData & reset_data)
{
  mc_control::MCController::reset(reset_data);
}

void SimpleHumanoidController::switch_target()
{
  if(goingLeft)
  {
    postureTask->target({{jointName, robot().qu()[jointIndex]}});
  }
  else
  {
    postureTask->target({{jointName, robot().ql()[jointIndex]}});
  }
  goingLeft = !goingLeft;
}

CONTROLLER_CONSTRUCTOR("SimpleHumanoidController", SimpleHumanoidController)
