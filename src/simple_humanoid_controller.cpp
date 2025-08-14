#include "simple_humanoid_controller.h"

SimpleHumanoidController::SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm,
                                                   double dt,
                                                   const mc_rtc::Configuration & config)
: mc_control::fsm::Controller(rm, dt, config)
{
}

bool SimpleHumanoidController::run()
{
  return mc_control::fsm::Controller::run();
}

void SimpleHumanoidController::reset(const mc_control::ControllerResetData & reset_data)
{
  mc_control::fsm::Controller::reset(reset_data);

  read_and_store_left_hand_initial_pose();
  read_and_store_right_hand_initial_pose();
}

void SimpleHumanoidController::read_and_store_left_hand_initial_pose()
{
  auto left_hand_initial_pose = robot().surfacePose(left_hand_frame_id_);
  datastore().make<sva::PTransformd>(left_hand_initial_pose_key_, left_hand_initial_pose);
}

void SimpleHumanoidController::read_and_store_right_hand_initial_pose()
{
  auto right_hand_initial_pose = robot().surfacePose(right_hand_frame_id_);
  datastore().make<sva::PTransformd>(right_hand_initial_pose_key_, right_hand_initial_pose);
}
