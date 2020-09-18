
#include <SoftwareSerial.h>
#include <ODriveArduino.h>

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

// ODrive object
ODriveArduino odrive(Serial2);


/* ROS */
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/Joy.h>
#include <ros/time.h>
/******/

#include <string.h>

//void odrive_cb(const std_msgs::Int32& cmd_msg);
void odrive_cb(const std_msgs::String& cmd_msg);


/* ROS */
ros::NodeHandle  nh;
//ros::Subscriber<std_msgs::Int32> odrivecmd_sub("odrive", odrive_cb);
ros::Subscriber<std_msgs::String> odrivecmd_sub("odrive", odrive_cb);

std_msgs::Int32MultiArray position_data;
ros::Publisher position_pub("position", &position_data);

std_msgs::Float32 voltage_data;
ros::Publisher voltage_pub("voltage", &voltage_data);
/******/

void ros_init1()
{
//    /* ROS */
    position_data.data_length = 2;
    position_data.data = (int32_t *)malloc(sizeof(int32_t)*2);
    position_data.data[0] = 0;
    position_data.data[1] = 0;

    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.subscribe(odrivecmd_sub);
    nh.advertise(position_pub);
    nh.advertise(voltage_pub);

    //_timer.start();
    
}

void odrive_init(){
  Serial2.begin(115200);
  nh.loginfo("ODriveArduino");
  nh.loginfo("Setting parameters...");
  int requested_state;
  char buf[50];
  for (int axis = 1; axis < 2; ++axis) {
    Serial2 << "w axis" << axis << ".controller.config.vel_limit " << 22000.0f << '\n';
    Serial2 << "w axis" << axis << ".motor.config.current_lim " << 11.0f << '\n';
    // This ends up writing something like "w axis0.motor.config.current_lim 10.0\n"
    requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
    sprintf(buf,"Axis %d : Requesting state [ %d ]", axis, requested_state);
    nh.loginfo(buf);
    odrive.run_state(axis, requested_state, true);

    requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
    sprintf(buf,"Axis %d : Requesting state [ %d ]", axis, requested_state);
    nh.loginfo(buf);
    odrive.run_state(axis, requested_state, true);

    requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
    sprintf(buf,"Axis %d : Requesting state [ %d ]", axis, requested_state);
    nh.loginfo(buf);
    odrive.run_state(axis, requested_state, false);
  }
  nh.loginfo("Ready!");

}

void odrive_reboot(){
  Serial2.begin(115200);
  Serial2 << "sr\n";
  delay(3000);
}
void setup() {
  odrive_reboot();
  ros_init1();
  odrive_init();
}

void loop() {
  nh.spinOnce();
  delay(500);
}

float GetPosition(int axis){
  float encoder_pos=0, position=0;
  Serial2 << "r axis" << axis << ".encoder.pos_estimate\n";
  encoder_pos=odrive.readFloat();
  return position=(360*encoder_pos)/2000 ;
}

void odrive_cb(const std_msgs::String& cmd_msg){
  nh.loginfo(cmd_msg.data);

    // Sinusoidal test move
    if (!strcmp(cmd_msg.data,"A")) {
      nh.loginfo("Executing test move");
      for (float ph = 0.0f; ph < 6.28318530718f; ph += 0.01f) {
        float pos_m0 = 20000.0f * cos(ph);
        float pos_m1 = 20000.0f * sin(ph);
        odrive.SetPosition(0, pos_m0);
        odrive.SetPosition(1, pos_m1);
        delay(5);
      }
    }

    // Read bus voltage
    if (!strcmp(cmd_msg.data,"B")) {
      Serial2 << "r vbus_voltage\n";
      voltage_data.data=odrive.readFloat();
      voltage_pub.publish(&voltage_data);
    }

    // print motor positions in a 10s loop
    if (!strcmp(cmd_msg.data,"X")) {
        for (int motor = 0; motor < 2; ++motor) {
          Serial2 << "r axis" << motor << ".encoder.pos_estimate\n";
          position_data.data[motor] = odrive.readFloat();
        }
        position_pub.publish(&position_data);
      
    }  
}
