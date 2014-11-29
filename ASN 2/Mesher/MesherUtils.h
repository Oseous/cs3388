#include <opencv2\core\core.hpp>
#include <iostream>
#include <string>

class MesherUtils{
private:

public:
  static cv::Mat homoCross(cv::Mat a, cv::Mat b);

  static cv::Mat homoNormalize(cv::Mat a);

  static bool parse4float(std::string line, float * floats);
};