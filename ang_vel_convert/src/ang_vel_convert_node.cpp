#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include "sensor_msgs/Imu.h"
#include <vector>

std::vector<double> shoulder_data;
std::vector<double> elbow_data;
float refresh_rate = 0.1;
std_msgs::UInt16 pub_pos;
int shoulder_cur_pos;
int elbow_cur_pos;
ros::Publisher servo_shoulder_pub;
ros::Publisher servo_elbow_pub;

void imu_cb(const sensor_msgs::Imu::ConstPtr& msg)
{
    if(msg->header.frame_id == "bicep"){
        shoulder_data.push_back(msg->angular_velocity.z);
    }
    else if(msg->header.frame_id == "forearm"){
        elbow_data.push_back(msg->angular_velocity.z);
    }
    //data.push_back(msg->angular_velocity.z);
}



void convert_cb(const ros::TimerEvent&){
    if(shoulder_data.size() > 0){
        //Assuming theres data stored in our buffer we need to average that and create a position estimate
        double rad_delta_s;
        ROS_INFO("DATA BEGIN: ");
        for(int i = 0; i < shoulder_data.size(); i++){
            ROS_INFO("%f", shoulder_data.at(i));
            rad_delta_s += shoulder_data.at(i);
        }
        ROS_INFO("DATA END");
        //Avg data
        rad_delta_s = rad_delta_s/shoulder_data.size();
        //Turn data from rad/sec to rad
        rad_delta_s = rad_delta_s/refresh_rate;
        //Convert from rad to angle
        double degree_delta = rad_delta_s * (180/3.1415926);
        degree_delta = degree_delta/100;

        shoulder_cur_pos += degree_delta;

        if(shoulder_cur_pos > 180){
            shoulder_cur_pos = 180;
        }
        else if(shoulder_cur_pos < 0){
            shoulder_cur_pos = 0;
        }

        pub_pos.data = static_cast<uint16_t>(shoulder_cur_pos);
        servo_shoulder_pub.publish(pub_pos);
        shoulder_data.clear();
    }
    if(elbow_data.size() > 0){
        //Assuming theres data stored in our buffer we need to average that and create a position estimate
        double rad_delta;
        ROS_INFO("DATA BEGIN: ");
        for(int i = 0; i < elbow_data.size(); i++){
            ROS_INFO("%f", elbow_data.at(i));
            rad_delta += elbow_data.at(i);
        }
        ROS_INFO("DATA END");
        //Avg data
        rad_delta = rad_delta/elbow_data.size();
        //Turn data from rad/sec to rad
        rad_delta = rad_delta/refresh_rate;
        //Convert from rad to angle
        double degree_delta = rad_delta * (180/3.1415926);
        degree_delta = degree_delta/100;

        elbow_cur_pos += degree_delta;

        if(elbow_cur_pos > 180){
            elbow_cur_pos = 180;
        }
        else if(elbow_cur_pos < 0){
            elbow_cur_pos = 0;
        }

        pub_pos.data = static_cast<uint16_t>(elbow_cur_pos);
        servo_elbow_pub.publish(pub_pos);
        elbow_data.clear();
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ang_vel_convert_node");
  ros::NodeHandle n;
  //Set initial position for servo position
  pub_pos.data = 0;
  shoulder_cur_pos = 0;
  elbow_cur_pos = 0;
  ros::Subscriber sub = n.subscribe("/imu/data", 1000, imu_cb);
  servo_shoulder_pub = n.advertise<std_msgs::UInt16>("servo_shoulder", 1000);    
  servo_elbow_pub = n.advertise<std_msgs::UInt16>("servo_elbow", 1000);
  ros::Timer convert_timer = n.createTimer(ros::Duration(refresh_rate), convert_cb);

  ros::spin();
  return 0;
}