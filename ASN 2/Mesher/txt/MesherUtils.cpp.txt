#include "MesherUtils.h"

cv::Mat MesherUtils::homoCross(cv::Mat a, cv::Mat b){
  cv::Mat ret = cv::Mat::zeros(4, 1, CV_32FC1);
  ret.at<float>(0) = a.at<float>(1)*b.at<float>(2) - a.at<float>(2)*b.at<float>(1);
  ret.at<float>(1) = a.at<float>(2)*b.at<float>(0) - a.at<float>(0)*b.at<float>(2);
  ret.at<float>(2) = a.at<float>(0)*b.at<float>(1) - a.at<float>(1)*b.at<float>(0);
  //Note that zero magnitude normals mean no area!
  return ret;
}

cv::Mat MesherUtils::homoNormalize(cv::Mat a){
  float mag;
  mag = sqrt(a.at<float>(0)*a.at<float>(0) + a.at<float>(1)*a.at<float>(1) + a.at<float>(2)*a.at<float>(2));
  if (mag != 0){
    a.at<float>(0) = a.at<float>(0) / mag;
    a.at<float>(1) = a.at<float>(1) / mag;
    a.at<float>(2) = a.at<float>(2) / mag;
  }
  return a;
}

//Floats must point to a length 3 array of floats
bool MesherUtils::parse4float(std::string line, float * floats){
  char * cstr = new char[line.length() + 1];
  strcpy(cstr, line.c_str());
  floats[0] = atof(cstr);
  int i = 0;
  for (int j = 1; j < 4; j++){
    while ((i < line.length()) && (((line[i] >= '0') && (line[i] <= '9')) || (line[i] == '.') || (line[i] == '-'))){
      i++;
    }
    while ((i < line.length()) && (line[i] == ' ')){
      i++;
    }
    cstr += i;
    floats[j] = atof(cstr);
    cstr -= i;
  }

  delete[] cstr;

  return true;
}