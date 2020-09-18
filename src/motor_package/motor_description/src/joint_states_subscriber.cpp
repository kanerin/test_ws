#include "ros/ros.h"
#include "rospy_tutorials/Floats.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void ParamCallback(const rospy_tutorials::Floats& msg)
{
  ROS_INFO("position : [%.3f]", msg.data[0]);
  ROS_INFO("velocity : [%.3f]", msg.data[1]);

}

int main(int argc, char **argv){
    ros::init(argc, argv, "joint_states_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/joint_states_from_arduino", 1000, ParamCallback);
    ros::spin();

  return 0;
}