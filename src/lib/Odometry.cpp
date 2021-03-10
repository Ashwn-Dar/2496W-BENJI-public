#include "../include/lib/Odometry.h"
using namespace std;

shared_ptr<Rotation> odom_right = make_shared<Rotation>(RIGHT_ENC);
shared_ptr<Rotation> odom_left = make_shared<Rotation>(LEFT_ENC);

Odometry::Odometry(){
    x_coord = y_coord = 0; 
    theta = prev_theta = d_theta = 0; 
    s = d_s = 0; 
    run_odom_task = false; 
}

void Odometry::begin_tracking(){
    run_odom_task = true; 
    while(1){
        prev_s = (odom_right->get_position() + odom_left->get_position()) / 2; 
        prev_theta = imu_sensor.get_rotation() * 1/RADIAN_TO_DEG;  

        pros::delay(ODOM_ITERATE_DELAY);

        s = (odom_right->get_position() + odom_left->get_position()) / 2;
        theta = imu_sensor.get_rotation() * 1/RADIAN_TO_DEG;  
        d_s = s - prev_s; 
        d_theta = theta - prev_theta; 

        x_coord += (int) (d_s * cos(theta + (d_theta/2))); 
        y_coord += (int) (d_s * sin(theta + (d_theta/2))); 

        if(run_odom_task){
            break; 
        }
    }
}

void Odometry::stop_tracking(){
    run_odom_task = false; 
}

int Odometry::get_X_pos(){
    return x_coord; 
}

int Odometry::get_Y_pos(){
    return y_coord; 
}