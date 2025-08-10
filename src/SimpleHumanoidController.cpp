#include "SimpleHumanoidController.h"

SimpleHumanoidController::SimpleHumanoidController(mc_rbdyn::RobotModulePtr rm,
                                                   double dt,
                                                   const mc_rtc::Configuration & config)
: mc_control::MCController(rm, dt)
{
  config_.load(config);

  // Ensure that contacts surfaces don't move once they are set
  solver().addConstraintSet(contactConstraint);

  // Ensure the robot is subject to joint position and velocity limits
  // solver().addConstraintSet(kinematicsConstraint);

  // Switch to dynamics mode, this will enable us to:
  //   - Compute external forces and associated torques;
  //   - Ensure these forces remain in the contact friction cone;
  //   - Ensure the torques remain within the robot's torque limits;
  // All constraints enabled by kinematicsConstraint are also enabled by dynamicsConstraint.
  solver().addConstraintSet(dynamicsConstraint);

  // Add contacts between the robot's feet and the ground:
  addContact({robot().name(), "ground", "LeftFoot", "AllGround"});
  addContact({robot().name(), "ground", "RightFoot", "AllGround"});

  // Add self-collision constraint
  selfCollisionConstraint = std::make_shared<mc_solver::CollisionsConstraint>(robots(), 0, 0, solver().dt());
  solver().addConstraintSet(*selfCollisionConstraint);
  selfCollisionConstraint->addCollisions(solver(), robot().module().minimalSelfCollisions());

  /* Tasks */
  // Task to control the robot posture, tries to bring all joints toward a target posture (usually the robot’s home)
  comTask = std::make_shared<mc_tasks::CoMTask>(robots(), 0, 10.0, 1000.0);
  solver().addTask(comTask);
  solver().addTask(postureTask);
  postureTask->stiffness(0.1);
  // Add hand task
  handTask = std::make_shared<mc_tasks::SurfaceTransformTask>("LeftGripper", robots(), 0);
  solver().addTask(handTask);
  // Look at frame task
  lookAtFrameTask = std::make_shared<mc_tasks::LookAtFrameTask>(robots(),
                                                                0, // robot index
                                                                "dcamera", // body to control
                                                                Eigen::Vector3d(0, 0, 1), // gaze vector (z axis)
                                                                0, // target robot index (same robot here)
                                                                "LeftGripper", // target frame name
                                                                10.0, // stiffness
                                                                500.0 // weight
  );
  solver().addTask(lookAtFrameTask);

  const auto & pose = handTask->surfacePose();
  initial_trans = pose.translation();
  initial_quat = Eigen::Quaterniond(pose.rotation());
  initial_rot = initial_quat.normalized().toRotationMatrix();

  target_trans = Eigen::Vector3d(0.5, 0.25, 1.1);
  target_quat = Eigen::Quaterniond(0, 0.7, 0, 0.7).normalized();
  target_rot = target_quat.toRotationMatrix();

  goToTarget = true;

  tol = 0.01;

  // Print all one-DoF joints (non-mimic) of the robot you control
  for(const auto & j : robot().mb().joints())
  {
    if(j.dof() >= 1 && !j.isMimic())
    {
      mc_rtc::log::info("- {}", j.name());
    }
  }

  mc_rtc::log::success("SimpleHumanoidController init done ");
}

bool SimpleHumanoidController::run()
{
  if(handTask->eval().norm() < tol)
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
  if(goToTarget)
  {
    handTask->target(sva::PTransformd{target_rot, target_trans});
  }
  else
  {
    handTask->target(sva::PTransformd{initial_rot, initial_trans});
  }
  goToTarget = !goToTarget;
}

CONTROLLER_CONSTRUCTOR("SimpleHumanoidController", SimpleHumanoidController)
