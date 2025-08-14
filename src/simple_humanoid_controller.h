#pragma once

#include <mc_control/fsm/Controller.h>
#include <mc_control/mc_controller.h>
#include "api.h"

struct TargetPose
{
  std::string frame;
  sva::PTransformd pose;
};

struct SimpleHumanoidController_DLLAPI SimpleHumanoidController : public mc_control::fsm::Controller
{
  SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config);
  bool run() override;
  void reset(const mc_control::ControllerResetData & reset_data) override;

private:
  void validate_and_store_target_pose_configs(const mc_rtc::Configuration & config);
  void initialize_target_poses();

  std::map<std::string, mc_rtc::Configuration> pose_configs_;
};