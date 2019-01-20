#include "ros/ros.h"//ros/ros.h是一个实用的头文件，它引用了ROS系统中大部分常用的头文件，使用它会使得编程很简便。
#include "std_msgs/String.h"//这引用了std_msgs/String 消息, 它存放在std_msgs package里，是由String.msg文件自动生成的头文件。
#include <sstream>
/*This tutorial demonstrates simple sending of messages over the ROS system.*/
int main(int argc, char **argv)
{
  /* The ros::init() function needs to see argc and argv so that it can perform any ROS arguments and name remapping that were provided at the command line. For programmatic remappings you can use a different version of init() which takes remappings directly, but for most command-line programs, passing argc and argv is the easiest way to do it.  The third argument to init() is the name of the node.You must call one of the versions of ros::init() before using any other part of the ROS system. */
  ros::init(argc, argv, "talker");//初始化ROS。它允许ROS通过命令行进行名称重映射——目前，这不是重点。同样，我们也在这里指定我们节点的名称——必须唯一。这里的名称必须是一个base name，不能包含/。
  /*NodeHandle is the main access point to communications with the ROS system.The first NodeHandle constructed will fully initialize this node, and the last NodeHandle destructed will close down the node.*/
  ros::NodeHandle n;//为这个进程的节点创建一个句柄。第一个创建的NodeHandle会为节点进行初始化，最后一个销毁的NodeHandle会清理节点使用的所有资源。
  /* The advertise() function is how you tell ROS that you want to publish on a given topic name. This invokes a call to the ROS master node, which keeps a registry of who is publishing and who is subscribing. After this advertise() call is made, the master node will notify anyone who is trying to subscribe to this topic name, and they will in turn negotiate a peer-to-peer connection with this node.  advertise() returns a Publisher object which allows you to publish messages on that topic through a call to publish().  Once all copies of the returned Publisher object are destroyed, the topic will be automatically unadvertised.The second parameter to advertise() is the size of the message queue used for publishing messages.  If messages are published more quickly than we can send them, the number here specifies how many messages to buffer up before throwing some away.*/
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);//告诉节点管理器master我们将要在chatter topic上发布一个std_msgs/String的消息。这样master就会告诉所有订阅了chatter topic的节点，将要有数据发布。第二个参数是发布序列的大小。在这样的情况下，如果我们发布的消息太快，缓冲区中的消息在大于1000个的时候就会开始丢弃先前发布的消息。NodeHandle::advertise() 返回一个 ros::Publisher对象,它有两个作用: 1) 它有一个publish()成员函数可以让你在topic上发布消息； 2) 如果消息类型不对,它会拒绝发布。
  ros::Rate loop_rate(10);//ros::Rate对象可以允许你指定自循环的频率。它会追踪记录自上一次调用Rate::sleep()后时间的流逝，并休眠直到一个频率周期的时间。在这个例子中，我们让它以10hz的频率运行。
  /*A count of how many messages we have sent. This is used to create a unique string for each message.*/
  int count = 0;
  while (ros::ok())
  {//roscpp会默认安装一个SIGINT句柄，它负责处理Ctrl-C键盘操作——使得ros::ok()返回FALSE。ros::ok()返回false，如果下列条件之一发生：SIGINT接收到(Ctrl-C);被另一同名节点踢出ROS网络;ros::shutdown()被程序的另一部分调用;所有的ros::NodeHandles都已经被销毁.一旦ros::ok()返回false, 所有的ROS调用都会失效。
    /*This is a message object. You stuff it with data, and then publish it.*/
    std_msgs::String msg;
    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();
//我们使用一个由msg file文件产生的‘消息自适应’类在ROS网络中广播消息。现在我们使用标准的String消息，它只有一个数据成员"data"。当然你也可以发布更复杂的消息类型。
    ROS_INFO("%s", msg.data.c_str());//ROS_INFO和类似的函数用来替代printf/cout. 
    /*The publish() function is how you send messages. The parameter
is the message object. The type of this object must agree with the type
given as a template parameter to the advertise<>() call, as was done
in the constructor above.*/
    chatter_pub.publish(msg);//现在我们已经向所有连接到chatter topic的节点发送了消息。
    ros::spinOnce();//在这个例子中并不是一定要调用ros::spinOnce()，因为我们不接受回调。然而，如果你想拓展这个程序，却又没有在这调用ros::spinOnce()，你的回调函数就永远也不会被调用。所以，在这里最好还是加上这一语句。
    loop_rate.sleep();//这条语句是调用ros::Rate对象来休眠一段时间以使得发布频率为10hz。
    ++count;
  }
  return 0;
}
