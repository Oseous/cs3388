#include "RenderUtils.h"

//Floats must point to a length 3 array of floats
bool RenderUtils::parse3float(std::string line, float * floats){
  char * cstr = new char[line.length() + 1];
  strcpy(cstr, line.c_str());
  floats[0] = atof(cstr);
  int i = 0;
  for (int j = 1; j < 3; j++){
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

//Ints must point to an array of 6 ints
bool RenderUtils::parse6int(std::string line, int * ints){
  char * cstr = new char[line.length() + 1];
  strcpy(cstr, line.c_str());
  ints[0] = atoi(cstr);
  int i = 0;
  for (int j = 1; j < 6; j++){
    while ((i < line.length()) && (((line[i] >= '0') && (line[i] <= '9')) || (line[i] == '.') || (line[i] == '-'))){
      i++;
    }
    while ((i < line.length()) && (line[i] == ' ')){
      i++;
    }
    cstr += i;
    ints[j] = atoi(cstr);
    cstr -= i;
  }

  delete[] cstr;

  return true;
}

//Calculates the cross product using only the first 3 entries
cv::Mat RenderUtils::homoCross(cv::Mat a, cv::Mat b){
  cv::Mat ret = cv::Mat::zeros(4, 1, CV_32FC1);
  ret.at<float>(0) = a.at<float>(1)*b.at<float>(2) - a.at<float>(2)*b.at<float>(1);
  ret.at<float>(1) = a.at<float>(2)*b.at<float>(0) - a.at<float>(0)*b.at<float>(2);
  ret.at<float>(2) = a.at<float>(0)*b.at<float>(1) - a.at<float>(1)*b.at<float>(0);
  //Note that zero magnitude normals mean no area!
  return ret;
}

//Normalize the first 3 entries
cv::Mat RenderUtils::homoNormalize(cv::Mat a){
  float mag;
  mag = sqrt(a.at<float>(0)*a.at<float>(0) + a.at<float>(1)*a.at<float>(1) + a.at<float>(2)*a.at<float>(2));
  if (mag != 0){
    a.at<float>(0) = a.at<float>(0) / mag;
    a.at<float>(1) = a.at<float>(1) / mag;
    a.at<float>(2) = a.at<float>(2) / mag;
  }
  return a;
}

//Calculates the dot product using the first three entries
float RenderUtils::homoDot(cv::Mat a, cv::Mat b){
  float sum = 0;
  for (int i = 0; i < 3; i++){
    sum += a.at<float>(i)*b.at<float>(i);
  }
  return sum;
}