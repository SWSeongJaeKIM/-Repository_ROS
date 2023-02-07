#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
int a = 0;
int b = 0;

 // ros::Subscriber sub_steering_input1 = n.subscribe(steering_input1_topic, 10, CarSteer_vision_Callback);


void gugudanCallback(const std_msgs::Int32& msg)   //const std_msgs::Int8 &msg
{
  a = msg.data;

}
void gugudan2Callback(const std_msgs::Int32& msg)   //const std_msgs::Int8 &msg
{
  b = msg.data;
}


int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber gugudan_sub = n.subscribe("/gugudan", 50, gugudanCallback);
  ros::Subscriber gugudan_sub2 = n.subscribe("/list", 50, gugudan2Callback);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
  for(a =1; a<=9; a++)
  {
    for(b=1; b<=9; b++)
    {
      ROS_INFO("%d x %d = %d\n", a, b, (a*b));
    }
  }

    loop_rate.sleep();
  
  }
  return 0;
}