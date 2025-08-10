#pragma once

#include <mc_control/mc_controller.h>
#include <mc_rbdyn/Robot.h>
#include <mc_solver/CollisionsConstraint.h>
#include <mc_tasks/CoMTask.h>
#include <mc_tasks/LookAtFrameTask.h>
#include <mc_tasks/SurfaceTransformTask.h>
#include "api.h"

struct SimpleHumanoidController_DLLAPI SimpleHumanoidController : public mc_control::MCController
{
  SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config);

  bool run() override;

  void reset(const mc_control::ControllerResetData & reset_data) override;

  void switch_target();

private:
  mc_rtc::Configuration config_;
  std::shared_ptr<mc_solver::CollisionsConstraint> selfCollisionConstraint;
  std::shared_ptr<mc_tasks::CoMTask> comTask;
  std::shared_ptr<mc_tasks::SurfaceTransformTask> handTask;
  std::shared_ptr<mc_tasks::LookAtFrameTask> lookAtFrameTask;
  bool goToTarget;

  Eigen::Vector3d target_trans;
  Eigen::Quaterniond target_quat;
  Eigen::Matrix3d target_rot;

  Eigen::Vector3d initial_trans;
  Eigen::Quaterniond initial_quat;
  Eigen::Matrix3d initial_rot;

  double tol;
};
