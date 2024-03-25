#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;


class CirclePublisher : public rclcpp::Node
{
  public:
    CirclePublisher()
    : Node("husky_circle_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/a200_0000/cmd_vel", 10);
      this->declare_parameter("spinning_speed", 0.5);
      this->declare_parameter("spinning_direction", "left");
      timer_ = this->create_wall_timer(
      500ms, std::bind(&CirclePublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = geometry_msgs::msg::Twist();
      message.linear.x = this->get_parameter("spinning_speed").as_double();
      message.angular.z = this->get_parameter("spinning_speed").as_double();
      if(this->get_parameter("spinning_direction").as_string() == "right")
      {
          message.angular.z = -message.angular.z;
      }
      //RCLCPP_INFO(this->get_logger(), "Publishing: linear velocity: '%s', angular velocity: ", message.linear.x.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CirclePublisher>());
  rclcpp::shutdown();
  return 0;
}
