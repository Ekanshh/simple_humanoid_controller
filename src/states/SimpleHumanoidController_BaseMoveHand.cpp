#include "SimpleHumanoidController_BaseMoveHand.h"

void SimpleHumanoidController_BaseMoveHand::configure(const mc_rtc::Configuration & config)
{
  config("weight", weight_);
  config("stiffness", stiffness_);
  config("damping", damping_);
  config("eval_threshold", eval_threshold_);
  config("speed_threshold", speed_threshold_);
  config("target_pose_key", target_pose_key_);
  config("task_name", task_name_);
}

void SimpleHumanoidController_BaseMoveHand::start(mc_control::fsm::Controller & ctl)
{

  if(!(ctl.datastore().has(target_pose_key_)))
  {
    mc_rtc::log::error_and_throw("Target pose key '{}' not found in datastore", target_pose_key_);
  }

  const auto target_pose_ = ctl.datastore().get<TargetPose>(target_pose_key_);

  transform_task_ = std::make_shared<mc_tasks::TransformTask>(target_pose_.frame, ctl.robots(), 0);
  transform_task_->name(task_name_);
  transform_task_->weight(weight_);
  transform_task_->stiffness(stiffness_);
  transform_task_->damping(damping_);
  transform_task_->target(target_pose_.pose);

  ctl.solver().addTask(transform_task_);

  task_completed_ = false;
}

bool SimpleHumanoidController_BaseMoveHand::run(mc_control::fsm::Controller & ctl)
{
  if(task_completed_)
  {
    output("OK");
    return true;
  }

  const auto eval_norm = transform_task_->eval().norm();
  const auto speed_norm = transform_task_->speed().norm();
  const auto eval_condition = eval_norm < eval_threshold_;
  const auto speed_condition = speed_norm < speed_threshold_;

  if(eval_condition && speed_condition)
  {
    task_completed_ = true;
    mc_rtc::log::info("Completed {} (eval AND speed)", task_name_);
    output("OK");
    return true;
  }

  return false;
}

void SimpleHumanoidController_BaseMoveHand::teardown(mc_control::fsm::Controller & ctl)
{
  if(transform_task_)
  {
    ctl.solver().removeTask(transform_task_);
    transform_task_.reset();
  }
}

EXPORT_SINGLE_STATE("SimpleHumanoidController_BaseMoveHand", SimpleHumanoidController_BaseMoveHand)
