#include <SoftwareSerial.h>
#include <ODriveArduino.h>

#define AXIS 1

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

//// Serial to the ODrive
//SoftwareSerial Serial1(8, 9); //RX (ODrive TX), TX (ODrive RX)
//// Note: you must also connect GND on ODrive to GND on Arduino!


//ros::NodeHandle nh;   //create Node handler
//std_msgs::Int32MultiArray encoder_data;

//rospy_tutorials::Floats joint_state;  //vector for encoder pos and vel

// ODrive object
ODriveArduino odrive(Serial1);

//ros::Subscriber<rospy_tutorials::Floats> sub("/joints_to_aurdino", set_angle_cb);

//ros::Publisher pub("/joint_states_from_arduino", &joint_state); //topic name:[joint_states_from_arduino] ,msg: [joint_state] 

void setup() {
  //nh.getHardware()->setBaud(115200);
  Serial1.begin(115200);  // ODrive uses 115200 baud
  Serial.begin(115200); // Serial to PC
  //while (!Serial) ; // wait for Arduino Serial Monitor to open

  Serial1 << "w axis" << AXIS << ".controller.config.vel_limit " << 22000.0f << '\n';
  Serial1 << "w axis" << AXIS << ".motor.config.current_lim " << 15.0f << '\n';
  odrive.run_state(AXIS, ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION, true);
  odrive.run_state(AXIS, ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true);
  odrive.run_state(AXIS, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false); // don't wait
  
//  
//  nh.initNode();
//  nh.subscribe(sub);
//  nh.advertise(pub);
}

void loop() {
    
  for (float ph = 0.0f; ph < 6.28318530718f; ph += 0.01f) {
        float vel  = 20000.0f * sin(ph);
        odrive.SetVelocity(AXIS, vel);
//        joint_state.data[0]=GetPosition(AXIS);
//        joint_state.data[1]=odrive.GetVelocity(AXIS);
//        joint_state.data[0]=GetPosition(AXIS);
//        joint_state.data[1]=odrive.GetVelocity(AXIS);        
//        Serial.print("Position: ");
//        Serial.print(joint_state.data[0],3) ;
//        Serial.print("    ");
//        Serial.print("Velocity: ");
//        Serial.println(joint_state.data[1],3) ;
//        Serial << "Position: " << joint_state.data[0] <<'\n';

//        pub.publish(&joint_state);              
        delay(100);
      }
//      nh.spinOnce();
}

float GetPosition(int axis){
  float encoder_pos=0, position=0;
  Serial1 << "r axis" << axis << ".encoder.pos_estimate\n";
  encoder_pos=odrive.readFloat();
  return position=(360*encoder_pos)/2000 ;
}