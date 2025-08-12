#pragma once

#include <mc_control/fsm/Controller.h>
#include <mc_control/mc_controller.h>

#include "api.h"

struct SimpleHumanoidController_DLLAPI SimpleHumanoidController : public mc_control::fsm::Controller
{
  SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config);

  bool run() override;

  void reset(const mc_control::ControllerResetData & reset_data) override;

private:
  std::string left_hand_frame_id_ = "LeftGripper";
  std::string right_hand_frame_id_ = "RightGripper";
  std::string left_hand_initial_pose_key_ = "LeftHandInitialPose";
  std::string right_hand_initial_pose_key_ = "RightHandInitialPose";

  void read_and_store_left_hand_initial_pose();
  void read_and_store_right_hand_initial_pose();
};