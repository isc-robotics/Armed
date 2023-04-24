# Project Armed
Overview
How does it work
Steps to INSTALL AND RUN (hard and soft ware)
Recommendations

## Project Overview
Project Armed is made as a final project for Jaerock Kwons ECE 3641 Robotic Manipulation Class.  The idea behind the project is to create a simple robotic arm that will mimic the movements of the user's own arm.  This is done by attaching IMUs to the user on the bicep and forearm to collect data on the movements on the two links.

## High Level How It Works
To achieve this goal we decided to utilize the Bosch BNO055 IMUs for data collection of the users arm movements.  For the control of the joints of the robot arm we opted to use an Arduino Uno to control servo motors for the shoulder and elbow joints.

## Low Level How It Works
To implement this system the output from the BNO055 is sent through a UART adapter to our computer, the data input is sent through the [BNO055 ROS Driver](https://github.com/RoboticArts/ros_imu_bno055) where the data is converted from the raw UART data to a test message which is sent to the `/imu/data` 
