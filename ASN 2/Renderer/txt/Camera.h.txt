#include <opencv2\core\core.hpp>

class Camera{
public:
  cv::Mat e, g, u, v, n;
public:
  void rotate(cv::Mat axis, float rads);
};