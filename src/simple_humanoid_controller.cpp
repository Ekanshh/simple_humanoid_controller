#include "simple_humanoid_controller.h"

SimpleHumanoidController::SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm,
                                                   double dt,
                                                   const mc_rtc::Configuration & config)
: mc_control::fsm::Controller(rm, dt, config)
{
  validate_and_store_target_pose_configs(config);
}

bool SimpleHumanoidController::run()
{
  return mc_control::fsm::Controller::run();
}

void SimpleHumanoidController::reset(const mc_control::ControllerResetData & reset_data)
{
  mc_control::fsm::Controller::reset(reset_data);

  initialize_target_poses();
}

void SimpleHumanoidController::validate_and_store_target_pose_configs(const mc_rtc::Configuration & config)
{
  if(!(config.has("target_poses")))
  {
    mc_rtc::log::error_and_throw("No 'target_poses' found in configuration");
  }

  const auto & pose_configs_tmp = config("target_poses");

  for(const auto & pose_name : pose_configs_tmp.keys())
  {
    const auto & pose_config = pose_configs_tmp(pose_name);

    if(!pose_config.has("frame"))
    {
      mc_rtc::log::error_and_throw("Target pose '{}' is missing required key 'frame'", pose_name);
    }

    const auto frame = pose_config("frame");
    if(!robot().hasFrame(frame))
    {
      mc_rtc::log::error_and_throw("Frame '{}' not found for pose '{}'", frame, pose_name);
    }

    const bool has_translation_rotation = pose_config.has("translation") && pose_config.has("rotation");
    const bool use_initial = pose_config.has("useInitial") && pose_config("useInitial", false);
    if(!has_translation_rotation && !use_initial)
    {
      mc_rtc::log::error_and_throw("Target pose '{}' must have either 'useInitial' true or 'translation' + 'rotation'",
                                   pose_name);
    }

    datastore().make<TargetPose>(pose_name);
    pose_configs_[pose_name] = pose_config;
  }
}

void SimpleHumanoidController::initialize_target_poses()
{
  for(const auto & [pose_name, pose_config] : pose_configs_)
  {
    sva::PTransformd pose;
    const std::string frame = pose_config("frame");
    const bool use_initial = pose_config("useInitial", false);

    if(use_initial)
    {
      pose = robot().frame(frame).position();
    }
    else
    {
      const Eigen::Vector3d translation = pose_config("translation");
      const Eigen::Quaterniond rotation = pose_config("rotation");

      pose = sva::PTransformd(rotation, translation);
    }

    datastore().get<TargetPose>(pose_name) = TargetPose{frame, pose};
  }
}
