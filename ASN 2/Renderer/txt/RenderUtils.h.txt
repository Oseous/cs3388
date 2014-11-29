#include <string>
#include <opencv2\core\core.hpp>

class RenderUtils{
private:

public:
  //Floats must point to a length 3 array of floats
  static bool parse3float(std::string line, float * floats);

  //Ints must point to an array of 6 ints
  static bool parse6int(std::string line, int * ints);

  //Calculates the cross product using only the first 3 entries
  static cv::Mat homoCross(cv::Mat a, cv::Mat b);

  //Normalize the first 3 entries
  static cv::Mat homoNormalize(cv::Mat a);

  //Calculates the dot product using the first three entries
  static float homoDot(cv::Mat a, cv::Mat b);

};