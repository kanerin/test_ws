/*インクルードガード*/
#ifndef ROS_CONTROL__ROBOT_HARDWARE_H 
#define ROS_CONTROL__ROBOT_HARDWARE_H

/*ハードウェアインターフェース用ヘッダ　*/
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <controller_manager/controller_manager.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>


class MyRobot : public hardware_interface::RobotHW 
{
    public:
        MyRobot(ros::NodeHandle& nh);　// コンストラクタ宣言（引数にノードハンドル）
        void init();  // 初期化関数の宣言
        void update(const ros::TimerEvent& e);  // 定周期でセンサ情報の読み取り及び、制御指示を書き込むUPDATE関数
        void read(); // センサ情報
        void write(ros::Duration elapsed_time); // ハードウェアに対して制御指示を送る関数
        
    protected:
        hardware_interface::JointStateInterface joint_state_interface_;
        hardware_interface::EffortJointInterface effort_joint_interface_;
        hardware_interface::VelocityJointInterface velocity_joint_interface_;
        hardware_interface::PositionJointInterface position_joint_interface_;
        
        joint_limits_interface::EffortJointSaturationInterface effort_joint_saturation_interface_;
        joint_limits_interface::EffortJointSoftLimitsInterface effort_joint_limits_interface_;
        joint_limits_interface::PositionJointSaturationInterface position_joint_saturation_interface_;
        joint_limits_interface::PositionJointSoftLimitsInterface position_joint_limits_interface_;
        joint_limits_interface::VelocityJointSaturationInterface velocity_joint_saturation_interface_;
        joint_limits_interface::VelocityJointSoftLimitsInterface velocity_joint_limits_interface_;        
        
        double joint_position_[2];  // 
        double joint_velocity_[2];  
        double joint_effort_[2];
        double joint_effort_command_[2];
        double joint_velocity_command_[2];
        double joint_position_command_[2];
        
        ros::NodeHandle nh_;
        ros::Timer my_control_loop_;
        ros::Duration elapsed_time_;
        double loop_hz_;
        boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
};
#endif //インクルードガード