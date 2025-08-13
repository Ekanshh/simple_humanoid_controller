#include "SimpleHumanoidController_BaseMoveHand.h"

void SimpleHumanoidController_BaseMoveHand::configure(const mc_rtc::Configuration & config)
{
  config("weight", weight_);
  config("stiffness", stiffness_);
  config("damping", damping_);
  config("eval_threshold", eval_threshold_);
  config("speed_threshold", speed_threshold_);
  config("hand_frame_id", hand_frame_id_);
  config("hand_pose_key", hand_pose_key_);
  config("target_pos", target_pos_);
  config("target_quat", target_quat_);
  config("transform_task_name", transform_task_name_);
}

void SimpleHumanoidController_BaseMoveHand::start(mc_control::fsm::Controller & ctl)
{
  if(ctl.datastore().has(hand_pose_key_))
  {
    target_ = ctl.datastore().get<sva::PTransformd>(hand_pose_key_);
  }
  else
  {
    target_ = sva::PTransformd(target_quat_, target_pos_);
  }

  transform_task_ = std::make_shared<mc_tasks::TransformTask>(hand_frame_id_, ctl.robots(), 0);
  transform_task_->name(transform_task_name_);
  transform_task_->weight(weight_);
  transform_task_->stiffness(stiffness_);
  transform_task_->damping(damping_);
  transform_task_->target(target_);

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

  double eval_norm = transform_task_->eval().norm();
  double speed_norm = transform_task_->speed().norm();

  bool eval_condition = eval_norm < eval_threshold_;
  bool speed_condition = speed_norm < speed_threshold_;

  if(eval_condition && speed_condition)
  {
    task_completed_ = true;
    mc_rtc::log::info("Completed {} (eval AND speed)", transform_task_name_);
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
