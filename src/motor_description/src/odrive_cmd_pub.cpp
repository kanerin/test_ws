#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>

std_msgs::String odrive_cmd;
void joy_callback(const sensor_msgs::Joy& joy_msg)
{
    if(joy_msg.buttons[0])
        odrive_cmd.data="A";
    else if(joy_msg.buttons[1])
        odrive_cmd.data="B";
    else if(joy_msg.buttons[2])
        odrive_cmd.data="X";
    else if(joy_msg.buttons[3])
        odrive_cmd.data="Y";
    else if(joy_msg.buttons[4])
        odrive_cmd.data="LB";
    else if(joy_msg.buttons[5])
        odrive_cmd.data="RB";
    else if(joy_msg.buttons[6])
        odrive_cmd.data="BACK";
    else if(joy_msg.buttons[7])
        odrive_cmd.data="START";
    else if(joy_msg.buttons[8])
        odrive_cmd.data="OPTION";
    else
        odrive_cmd.data=""; 
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odrive_cmd_pub");
  ros::NodeHandle nh;
  ros::Publisher cmd_pub = nh.advertise<std_msgs::String>("odrive", 1000);
  ros::Subscriber joy_sub = nh.subscribe("joy", 10, joy_callback);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    cmd_pub.publish(odrive_cmd);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}