
#include <SoftwareSerial.h>
#include <ODriveArduino.h>
/* ROS */
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <ros/time.h>
/******/


// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

// ODrive object
ODriveArduino odrive(Serial2);


void odrive_cb(const std_msgs::Float32MultiArray& joy_msg);

float GetPosition(int axis);

/* ROS */
ros::NodeHandle  nh;
ros::Subscriber<std_msgs::Float32MultiArray> joy_sub("joy_array", odrive_cb);

std_msgs::Int32MultiArray position_data;
ros::Publisher position_pub("position", &position_data);

std_msgs::Float32MultiArray velocity_data;
ros::Publisher velocity_pub("velocity", &velocity_data);

std_msgs::Float32 voltage_data;
ros::Publisher voltage_pub("voltage", &voltage_data);
/******/

float vel1,vel2;

void array_init(std_msgs::Int32MultiArray& data, int array){
    data.data_length=array;
    data.data=(int32_t *)malloc(sizeof(int32_t)*array);
    for(int i=0; i<array ;i++){
        data.data[i]=0;
    }
}

void array_init(std_msgs::Float32MultiArray& data, int array){
    data.data_length=array;
    data.data=(float *)malloc(sizeof(float)*array);
    for(int i=0; i<array ;i++){
        data.data[i]=0;
    }
}
void ros_init1()
{
    array_init(position_data,2);
    array_init(velocity_data,2);
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.subscribe(joy_sub);
    nh.advertise(position_pub);
    nh.advertise(velocity_pub);
    nh.advertise(voltage_pub);
}

void odrive_init(){
  Serial2.begin(115200);
  nh.loginfo("ODriveArduino");
  nh.loginfo("Setting parameters...");
  int requested_state;
  char buf[50];
  for (int axis = 0; axis < 2; ++axis) {
    Serial2 << "w axis" << axis << ".controller.config.vel_limit " << 42000.0f << '\n';
    Serial2 << "w axis" << axis << ".motor.config.current_lim " << 50.0f << '\n';
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
  Serial2 << "r vbus_voltage\n";
  voltage_data.data=odrive.readFloat();
  voltage_pub.publish(&voltage_data);

  for(int motor=0; motor<2 ;motor++){
    position_data.data[motor]=GetPosition(motor);
    velocity_data.data[motor]=odrive.GetVelocity(motor);
  }
  position_pub.publish(&position_data);
  velocity_pub.publish(&velocity_data);
  
  nh.spinOnce();
  delay(500);
}

void odrive_cb(const std_msgs::Float32MultiArray& joy_msg){
    joy_msg.data[1] > 0 ? vel1=pow((joy_msg.data[1]*3.0),2)*(40000.0/9.0) : vel1=pow((joy_msg.data[1]*3.0),2)*(-40000.0/9.0);
    joy_msg.data[4] > 0 ? vel2=pow((joy_msg.data[4]*3.0),2)*(40000.0/9.0) : vel2=pow((joy_msg.data[4]*3.0),2)*(-40000.0/9.0);

    odrive.SetVelocity(0,vel1);
    odrive.SetVelocity(1,vel2);
}


float GetPosition(int axis){
  float encoder_pos=0, position=0;
  Serial2 << "r axis" << axis << ".encoder.pos_estimate\n";
  encoder_pos=odrive.readFloat();
  return position=(360*(fmod(encoder_pos,360)))/2000 ;
}
