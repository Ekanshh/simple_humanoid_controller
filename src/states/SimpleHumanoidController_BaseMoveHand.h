#pragma once

#include <mc_control/fsm/State.h>
#include <mc_tasks/TransformTask.h>
#include "../simple_humanoid_controller.h"

struct SimpleHumanoidController_BaseMoveHand : mc_control::fsm::State
{
public:
  void configure(const mc_rtc::Configuration & config) override;
  void start(mc_control::fsm::Controller & ctl) override;
  bool run(mc_control::fsm::Controller & ctl) override;
  void teardown(mc_control::fsm::Controller & ctl) override;

private:
  double weight_ = 1000.0;
  double stiffness_ = 10.0;
  double damping_ = 20.0;
  double eval_threshold_ = 0.5;
  double speed_threshold_ = 1e-4;
  std::string hand_frame_id_;
  std::string hand_pose_key_;
  Eigen::Vector3d target_pos_;
  Eigen::Quaterniond target_quat_;
  sva::PTransformd target_;
  std::string transform_task_name_;

  std::shared_ptr<mc_tasks::TransformTask> transform_task_;

  bool task_completed_ = false;
};