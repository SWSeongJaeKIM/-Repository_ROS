#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include <tf/transform_datatypes.h>

long encoder1data = 0;
long encoder2data = 0;
double angle = 0;
long encoder_old = 0;
double raduus = 0;

struct OdoData {
    double x;
    double y;
};

OdoData odom_data;

struct ImuData {
    geometry_msgs::Quaternion orientation;
    double angular_velocity_x;
    double angular_velocity_y;
    double angular_velocity_z;
    double linear_acceleration_x;
    double linear_acceleration_y;
    double linear_acceleration_z;
};

ImuData imu_data;

void encoder1Callback(const std_msgs::Int32& encoder_data1)
{
    encoder1data = encoder_data1.data;
}

void encoder2Callback(const std_msgs::Int32& encoder_data2)
{
    encoder2data = encoder_data2.data;
}

void imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    imu_data.orientation = msg->orientation;
    imu_data.angular_velocity_x = msg->angular_velocity.x;
    imu_data.angular_velocity_y = msg->angular_velocity.y;
    imu_data.angular_velocity_z = msg->angular_velocity.z;
    imu_data.linear_acceleration_x = msg->linear_acceleration.x;
    imu_data.linear_acceleration_y = msg->linear_acceleration.y;
    imu_data.linear_acceleration_z = msg->linear_acceleration.z;
}

void magCallback(const sensor_msgs::MagneticField::ConstPtr& msg)
{
    // 자기장 데이터 처리
    // msg->magnetic_field - 자기장 벡터 정보
}

void odma_calculation(void)
{
    angle = imu_data.orientation.z;
    raduus = (encoder1data - encoder_old) / angle;
    encoder_old = encoder1data;

    if (angle >= 0)
    {
        odom_data.y += raduus - raduus * cos(angle);
        odom_data.x += raduus * sin(angle);
    }
    else
    {
        odom_data.y -= raduus - raduus * cos(angle);
        odom_data.x += raduus * sin(angle);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odom_node");
    ros::NodeHandle n;

    ros::Subscriber imu_sub = n.subscribe("handsfree/imu", 10, imuCallback);
    ros::Subscriber mag_sub = n.subscribe("handsfree/mag", 10, magCallback);
    ros::Subscriber subEncoder1 = n.subscribe("/encoder1", 10, &encoder1Callback); // front  encoder data subscribe
    ros::Subscriber subEncoder2 = n.subscribe("/encoder2", 10, &encoder2Callback); // rear  encoder data subscribe

    ros::Rate loop_rate(10.0); //10.0HZ
    while (ros::ok())
    {
        odma_calculation();
        double r, p, y;
        tf::Quaternion quat;
        tf::quaternionMsgToTF(imu_data.orientation, quat);
        tf::Matrix3x3(quat).getRPY(r, p, y);
        r *= 180 / M_PI;
        p *= 180 / M_PI;
        y *= 180 / M_PI;

        ROS_INFO("Pitch = %f, Roll = %f, Yaw = %f", p, r, y);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
