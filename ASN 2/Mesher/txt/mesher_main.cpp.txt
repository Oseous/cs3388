#include <iostream> //console io for user
#include <fstream> //read and write to files
#include <string> //used to read lines from file

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <vector>

#include "Face.h"
#include "MesherUtils.h"

//#define DEBUG
#define ToRad(x) (x*0.01745329252)  // *pi/180
#define ToDeg(x) (x*57.2957795131)  // *180/pi

using namespace std;
using namespace cv;

int main(int argc, char** argv){
  // -----------------------------------------------
  // Get path to profile file
  // This program requires one argument - nag user
  // if they didn't provide one.
  // -----------------------------------------------
  if (argc != 2){ // argc includes the program's name
    cout << "Error: ProfileToMesh requires an argument.\nEg: > ProfileToMesh.exe MyProfile.txt\n"
      "Please try again, but provide the name or fullpath to the file containing your profile.\n";
    return(1);
  }

  // Get file to read from user
  fstream profileFile;
  profileFile.open(argv[1], ios_base::in);
  if (!profileFile.is_open()){
    cout << "Error: file \"" << argv[1] << "\" could not be opened.\nPlease verify that the pathname is correct.\n";
    return 2;
  }

  // -----------------------------------------------
  // Get axis of rotation
  // Must be x, y, or z.  The axis is not arbitrary.
  // -----------------------------------------------
  string line;
  float rotationAxis[] = { 0, 0, 0 };
  if (!getline(profileFile, line)){
    cout << "Error: Axis of rotation could not be read from the specified file.\nThe axis of rotation is a single char on the first line - x, y, or z.\n";
    return 3;
  }
  if (!(line.length() > 0)){
    cout << "Error: Axis of rotation could not be read from the specified file.\nThe axis of rotation is a single char on the first line, x, y, or z.\n";
    return 4;
  }
  switch (line[0]){
  case 'x':
    rotationAxis[0] = 1;
    break;

  case 'y':
    rotationAxis[1] = 1;
    break;

  case 'z':
    rotationAxis[2] = 1;
    break;

  default:
    cout << "Error: Specified profile file is invalid, the axis of rotation (x, y, or z) must be the first char on the first line.\n";
    return 5;
    break;
  }

  // -----------------------------------------------
  // Get angular increment
  // -----------------------------------------------
  if (!getline(profileFile, line)){
    cout << "Error: Too few lines in profile file.\nAngular increment must be provided as a float on the second line.\n";
  }
  float angularInc;
  angularInc = atof(line.c_str());
  /*if (){
    //Not really sure how to test... Just trust it for now.
  }*/

  // -----------------------------------------------
  // Get set of points
  // -----------------------------------------------
  vector<cv::Mat> vertices;
  vector<cv::Mat> profile;
  vector<Face> faces;
  vector<cv::Mat> normals;
  // I assume that the list is well formatted... This course isn't about parsing text.
  while (getline(profileFile, line)){
    if ((line[0] >= '0') && (line[0] <= '9')){
      //Get first float
      cv::Mat tempPoint = cv::Mat::zeros(4, 1, CV_32FC1);
      float floats[4];
      MesherUtils::parse4float(line, floats);
      for (int i = 0; i < 4; i++)
        tempPoint.at<float>(i) = floats[i];

#ifdef DEBUG
      cout << "Point " << vertices.size() << ":\n" << tempPoint << endl;
#endif

      //Put in both lists
      vertices.emplace_back(tempPoint);
      profile.emplace_back(tempPoint);
    }
  }

  profileFile.close();

  int N = vertices.size();
  // Represent rotation axis
  Mat Jv = Mat::zeros(Size(3, 3), CV_32FC1);
  //                 col, row (weird...)
  Jv.at<float>(cvPoint(1, 0)) = -rotationAxis[2];
  Jv.at<float>(cvPoint(2, 0)) = rotationAxis[1];
  Jv.at<float>(cvPoint(0, 1)) = rotationAxis[2];
  Jv.at<float>(cvPoint(2, 1)) = -rotationAxis[0];
  Jv.at<float>(cvPoint(0, 2)) = -rotationAxis[1];
  Jv.at<float>(cvPoint(1, 2)) = rotationAxis[0];

#ifdef DEBUG
  cout << "Jv:\n" << Jv << endl;
#endif

  //Figure out the number of steps to take to trace all the way around
  int steps = 360 / angularInc;

  // Do profiling
  //rotate profile
  for (int i = 1; i < steps; i++){
    //Build rotation matrix
    float angularPos = i*angularInc;
#ifdef DEBUG
    cout << angularPos << endl;
#endif
    //Build rotation matrix
    Mat R = Mat::eye(Size(3, 3), CV_32FC1) + sin(ToRad(angularPos))*Jv + (1 - cos(ToRad(angularPos)))*Jv*Jv;
    //Put into homogeneous coordinates by padding with zeros
    copyMakeBorder(R, R, 0, 1, 0, 1, BORDER_CONSTANT, 0);
    //Add one in bottom right
    R.at<float>(Point(3, 3)) = 1;

#ifdef DEBUG
    cout << "R(" << angularPos << "):\n" << R << endl;
#endif

    for (int j = 0; j < N; j++){
      //Iterate through original points and rotate them to the new contour to add.
      vertices.emplace_back(R*profile[j]);
      //Build up Face and Normals lists
      if (j > 0){ //Not first vertex to add
        //Add face below vertex
        Face face;
        //I assume that the points in the contour ascend along the axis of rotation.
        face.vertices[0] = N*(i - 1) + j;
        face.vertices[1] = N*i + j - 1;
        face.vertices[2] = N*i + j;
        //Sort of contrived until we get to smooth surfaces...
        face.normals[0] = normals.size();
        face.normals[1] = normals.size();
        face.normals[2] = normals.size();
        //Compute and insert normal
        normals.emplace_back(MesherUtils::homoNormalize(MesherUtils::homoCross(vertices[face.vertices[1]] - vertices[face.vertices[0]], vertices[face.vertices[2]] - vertices[face.vertices[0]])));
        faces.emplace_back(face);
      }
      if (j < (N - 1)){ //Not last vertex to add
        //Add face above added vertex
        Face face;
        //I assume that the points in the contour ascend along the axis of rotation.
        face.vertices[0] = N*(i - 1) + j;
        face.vertices[1] = N*i + j;
        face.vertices[2] = N*(i - 1) + j + 1;
        //Sort of contrived until we get to smooth surfaces...
        face.normals[0] = normals.size();
        face.normals[1] = normals.size();
        face.normals[2] = normals.size();
        //Compute and insert normal
        normals.emplace_back(MesherUtils::homoNormalize(MesherUtils::homoCross(vertices[face.vertices[1]] - vertices[face.vertices[0]], vertices[face.vertices[2]] - vertices[face.vertices[0]])));
        faces.emplace_back(face);
      }
    }
  }
  //Add faces between first and last contour traces
  for (int j = 0; j < N; j++){
    //Finalize Face and Normals lists
    if (j > 0){ //Not first vertex to add
      //Add face below vertex
      Face face;
      //I assume that the points in the contour ascend along the axis of rotation.
      face.vertices[0] = N*(steps - 1) + j;
      face.vertices[1] = j - 1;
      face.vertices[2] = j;
      //Sort of contrived until we get to smooth surfaces...
      face.normals[0] = normals.size();
      face.normals[1] = normals.size();
      face.normals[2] = normals.size();
      //Compute and insert normal
      normals.emplace_back(MesherUtils::homoNormalize(MesherUtils::homoCross(vertices[face.vertices[1]] - vertices[face.vertices[0]], vertices[face.vertices[2]] - vertices[face.vertices[0]])));
      faces.emplace_back(face);
    }
    if (j < (N - 1)){ //Not last vertex to add
      //Add face above added vertex
      Face face;
      //I assume that the points in the contour ascend along the axis of rotation.
      face.vertices[0] = N*(steps - 1) + j;
      face.vertices[1] = j;
      face.vertices[2] = N*(steps - 1) + j + 1;
      //Sort of contrived until we get to smooth surfaces...
      face.normals[0] = normals.size();
      face.normals[1] = normals.size();
      face.normals[2] = normals.size();
      //Compute and insert normal
      normals.emplace_back(MesherUtils::homoNormalize(MesherUtils::homoCross(vertices[face.vertices[1]] - vertices[face.vertices[0]], vertices[face.vertices[2]] - vertices[face.vertices[0]])));
      faces.emplace_back(face);
    }
  }

  //List out vertices
#ifdef DEBUG
  for (int i = 0; i < vertices.size(); i++){
    cout << "Vertices " << i << ":\n" << vertices[i] << endl;
  }
#endif
  ////List out all of the normals
#ifdef DEBUG
  for (int i = 0; i < normals.size(); i++)
    cout << "Normal " << i << ":\n" << normals[i] << endl;
#endif

  // Save to disk
  fstream outFile;
  char* nameBuffer = new char[strlen(argv[1]) + 4];
  //Copy until '.' is reached
  int i;
  for (i = 0; i < strlen(argv[1]); i++){
    if (argv[1][i] == '.')
      break;
    nameBuffer[i] = argv[1][i];
  }
  nameBuffer[i] = 'M';
  nameBuffer[i + 1] = 'e';
  nameBuffer[i + 2] = 's';
  nameBuffer[i + 3] = 'h';
  for (i; i <= strlen(argv[1]); i++){ // <= so that the '\0' is also copied
    nameBuffer[i + 4] = argv[1][i];
  }
  outFile.open(nameBuffer, ios_base::out);
  // Write vertices
  outFile << "Vertices:\n";
  for (i = 0; i < vertices.size(); i++){
    for (int j = 0; j < 3; j++)
      outFile << fixed << vertices[i].at<float>(j) << ' ';
    outFile << endl;
  }
  // Write faces
  outFile << "Faces:\n";
  for (i = 0; i < faces.size(); i++){
    for (int j = 0; j < 3; j++)
      outFile << faces[i].vertices[j] << ' ';
    for (int j = 0; j < 3; j++)
      outFile << faces[i].normals[j] << ' ';
    outFile << endl;
  }
  // Write normals
  outFile << "Normals:\n";
  for (i = 0; i < normals.size(); i++){
    for (int j = 0; j < 3; j++)
      outFile << fixed << normals[i].at<float>(j) << ' ';
    outFile << endl;
  }
  outFile.close();

  cout << "Meshing complete.\n";

  return 0;
}