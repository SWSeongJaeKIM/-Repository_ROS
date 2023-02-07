#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"


std_msgs::Int32 gugudan_msg;
std_msgs::Int32 gugudan2_msg;


int main(int argc, char **argv)
{
  int a = 0;
 
  ros::init(argc, argv, "talker");


  ros::NodeHandle n;

  ros::Publisher gugudan_pub     = n.advertise<std_msgs::Int32>("/gugudan", 50);
  ros::Publisher gugudan2_pub     = n.advertise<std_msgs::Int32>("/list", 50);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    
    ros::spinOnce();
    gugudan_msg.data = 0;
    gugudan2_msg.data = 0;
    gugudan_pub.publish(gugudan_msg);
    gugudan2_pub.publish(gugudan2_msg);


    ros::spinOnce();

    loop_rate.sleep();

    ++count;
  }


  return 0;
}