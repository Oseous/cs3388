#include <iostream> //console io for user
#include <fstream> //read and write to files
#include <string> //used to read lines from file
#include <vector> //use to hold lists

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "RenderUtils.h"

#include "Face.h"

#include "ImageManip.h"
#include "Camera.h"

//#define DEBUG

using namespace std;
using namespace cv;

int main(int argc, char ** argv){
  // -----------------------------------------------
  // Get path to mesh file
  // This program requires one argument - nag user
  // if they didn't provide one.
  // -----------------------------------------------
  if (argc != 2){ // argc includes the program's name
    cout << "Error: ProfileToMesh requires an argument.\nEg: > ProfileToMesh.exe MyMesh.txt\n"
      "Please try again, but provide the name or fullpath to the file containing your profile.\n";
    return(1);
  }

  // Get file to read from user
  fstream meshFile;
  meshFile.open(argv[1], ios_base::in);
  if (!meshFile.is_open()){
    cout << "Error: file \"" << argv[1] << "\" could not be opened.\nPlease verify that the pathname is correct.\n";
    return 2;
  }

  string line;
  if (!getline(meshFile, line)){
    cout << "Error, too few lines in file.";
    return 3;
  }
  while (line[0] != 'V'){
    if (!getline(meshFile, line)){
      cout << "Reached end of file without finding info!\n";
      return 4;
    }
  }
  //At beginning of the vertex list
  float vert[3];
  vector<Mat> vertices;
  while (getline(meshFile, line)){
    if (line[0] == 'F')
      break;
    Mat vertex = Mat::zeros(4, 1, CV_32FC1);
    RenderUtils::parse3float(line, vert);
    vertex.at<float>(0) = vert[0];
    vertex.at<float>(1) = vert[1];
    vertex.at<float>(2) = vert[2];
    vertices.emplace_back(vertex);
#ifdef DEBUG
    cout << "Vertex " << vertices.size() << ":\n" << vertex << endl;
#endif
  }
  //At beginning of the face list
  int ints[6];
  Face face;
  vector<Face> faces;
  while (getline(meshFile, line)){
    if (line[0] == 'N')
      break;
    RenderUtils::parse6int(line, ints);
    face.vertices[0] = ints[0];
    face.vertices[1] = ints[1];
    face.vertices[2] = ints[2];
    face.normals[0] = ints[3];
    face.normals[1] = ints[4];
    face.normals[2] = ints[5];
    faces.emplace_back(face);
#ifdef DEBUG
    cout << "Face " << faces.size() << ":\n"
      << face.vertices[0] << " " << face.vertices[1] << " " << face.vertices[2] << endl
      << face.normals[0] << " " << face.normals[1] << " " << face.normals[2] << endl;
#endif
  }
  //At beginning of the faces list
  float norm[3];
  vector<Mat> normals;
  while (getline(meshFile, line)){
    if ((line[0]!='-')&&((line[0]<'0') || (line[0]>'9')))
      break;
    RenderUtils::parse3float(line, norm);
    Mat normal = Mat::zeros(4, 1, CV_32FC1);
    normal.at<float>(0) = norm[0];
    normal.at<float>(1) = norm[1];
    normal.at<float>(2) = norm[2];
    normals.emplace_back(normal);
  }
#ifdef DEBUG
  for (int i = 0; i < normals.size(); i++)
  cout << "Normal " << i << ":\n" << normals[i] << endl;
#endif
  //vector<Mat> normals_v(normals.size());  //Important later
  //Done parsing file

  //Print out usage:
  cout << "Wire Mesh Render Tool v1.0\n"
    << "Usage:\n"
    << "w/s - pitch forward/back\n"
    << "a/d - yaw left/right\n"
    << "z/c - roll left/right\n"
    << "r/f - zoom in/out\n"
    << "esc/q - quit\n";

  //Open 16:9 window
  namedWindow("Render Window", WINDOW_AUTOSIZE);
  Mat image = Mat::zeros(Size(1280, 720), CV_8UC3); //Dimensions must be even!!!

  //Setup camera
  Mat p;
  Camera cam;
  // Camera position
  cam.e = Mat::ones(4, 1, CV_32FC1);
  cam.e.at<float>(0) = 175;
  cam.e.at<float>(1) = 0;
  cam.e.at<float>(2) = 15.5;
  // Gaze point
  cam.g = Mat::ones(4, 1, CV_32FC1);
  cam.g.at<float>(0) = 0;
  cam.g.at<float>(1) = 0;
  cam.g.at<float>(2) = 15.5;
  // Up vector
  p = Mat::zeros(4, 1, CV_32FC1);
  p.at<float>(2) = 1;  //Point up
  // Gaze vector
  cam.n = RenderUtils::homoNormalize(cam.e - cam.g);
  cam.u = RenderUtils::homoNormalize(RenderUtils::homoCross(p, cam.n));
  // Camera's up vector
  cam.v = RenderUtils::homoNormalize(RenderUtils::homoCross(cam.n, cam.u));
  
  // Near and far plane parameters
  float N = 5, F = 25;
  float a = -1.0 * (F + N) / (F - N);
  float b = -2.0 * F*N / (F - N);

  // List of transformed verts
  //vector<Mat> vertices_v(vertices.size());
  vector<Mat> vertices_s(vertices.size());
  //Initialize
  for (int i = 0; i < vertices.size(); i++){
    p.copyTo(vertices_s[i]);
  }

  // Camera transformation component matrices that never change
  float r = 1;
  float t = r*((float)image.rows)/((float)image.cols);  //Preserve aspect ratio
  Mat T1 = Mat::eye(4, 4, CV_32FC1); // l=-r, b=-t
  Mat S1 = Mat::eye(4, 4, CV_32FC1);
  S1.at<float>(0) = 1.0f / r;
  S1.at<float>(5) = 1.0f / t;

  Mat T2 = Mat::eye(4, 4, CV_32FC1);
  T2.at<float>(3) = 1;
  T2.at<float>(7) = 1;
  Mat S2 = Mat::eye(4, 4, CV_32FC1);
  S2.at<float>(0) = image.cols / 2;
  S2.at<float>(5) = image.rows / 2;

  Mat W = Mat::eye(4, 4, CV_32FC1);
  W.at<float>(5) = -1;
  W.at<float>(7) = image.rows;
  
  Mat WS2T2S1T1 = W*S2*T2*S1*T1;

  long frameCnt = 0;
  double ticks = (double)getTickCount();
  Mat rot = Mat::zeros(4, 1, CV_32FC1);  // Matrix for tracking rotations
  Mat Mpa;
  Mat Mv;
  while (true){
      
    // Build perspective transform matrix
    Mpa = Mat::zeros(4, 4, CV_32FC1);
    Mpa.at<float>(0) = N;
    Mpa.at<float>(5) = N;
    Mpa.at<float>(10) = a;
    Mpa.at<float>(11) = b;
    Mpa.at<float>(14) = -1;

    // Build camera transform matrix
    Mv = Mat::zeros(4, 4, CV_32FC1);
    //Lots of fun...
    for (int i = 0; i < 3; i++){
      Mv.at<float>(i) = cam.u.at<float>(i);
      Mv.at<float>(4 + i) = cam.v.at<float>(i);
      Mv.at<float>(8 + i) = cam.n.at<float>(i);
    }
    Mv.at<float>(3) = -RenderUtils::homoDot(cam.e, cam.u);
    Mv.at<float>(7) = -RenderUtils::homoDot(cam.e, cam.v);
    Mv.at<float>(11) = -RenderUtils::homoDot(cam.e, cam.n);
    Mv.at<float>(15) = 1;

    // Transform all verts
    for (int i = 0; i < vertices.size(); i++){
      Mat temp = vertices_s[i];
      vertices[i].copyTo(temp);
      temp.at<float>(3) = 1; //Transforming vectors doesn't work (or make sense)
      temp = Mpa*Mv*temp;
      //temp.copyTo(vertices_v[i]); //Copy into world view verts
      temp = temp / temp.at<float>(3);
      temp = WS2T2S1T1*temp;
      //vertices_s[i] = temp;
    }

    //Transform all vertices into the camera's world.
    ImageManip::whiten(image);
    for (int i = 0; i < faces.size(); i++){
      //Check if normal points away from camera
      //Mat normal = normals[faces[i].normals[0]];
      //normals_v[faces[i].normals[0]] = homoNormalize(homoCross(vertices_v[faces[i].vertices[1]] - vertices_v[faces[i].vertices[0]], vertices_v[faces[i].vertices[2]] - vertices_v[faces[i].vertices[0]]));
      //Need to recalculate normals in the transformed space :(
      //if (homoDot(normals_v[faces[i].normals[0]], -p) >  0){
      if (RenderUtils::homoDot(normals[faces[i].normals[0]], cam.n) >  0){
        //Draw all three lines
        my::Point a, b, c;
        a.x = vertices_s[faces[i].vertices[0]].at<float>(0);
        a.y = vertices_s[faces[i].vertices[0]].at<float>(1);
        b.x = vertices_s[faces[i].vertices[1]].at<float>(0);
        b.y = vertices_s[faces[i].vertices[1]].at<float>(1);
        c.x = vertices_s[faces[i].vertices[2]].at<float>(0);
        c.y = vertices_s[faces[i].vertices[2]].at<float>(1);
        Scalar colour(200.0*vertices[faces[i].vertices[0]].at<float>(0) / 30.0,
                      200.0*vertices[faces[i].vertices[0]].at<float>(1) / 30.0,
                      200.0*vertices[faces[i].vertices[0]].at<float>(2) / 31.0);
        ImageManip::drawLine(image, a, b, colour);
        ImageManip::drawLine(image, b, c, colour);
        ImageManip::drawLine(image, c, a, colour);
      }
    }

    imshow("Render Window", image);

    //Do wait/key check
    char c;
    c = cvWaitKey(10); // wait 10 ms or for key stroke
    if ((c == 27) || (c == 'q'))
      break; // if ESC, break and quit
    switch (c){
    case 'a':
      //Yaw right
      rot.at<float>(0) = 0;
      rot.at<float>(1) = 0;
      rot.at<float>(2) = 1;
      cam.rotate(rot, 0.04); //right 1/100 of a rad
      break;
    case 'd':
      //Yaw left
      rot.at<float>(0) = 0;
      rot.at<float>(1) = 0;
      rot.at<float>(2) = 1;
      cam.rotate(rot, -0.04); //left 1/100 of a rad
      break;
    case 'w':
      //Pitch up
      rot.at<float>(0) = cam.u.at<float>(0);
      rot.at<float>(1) = cam.u.at<float>(1);
      rot.at<float>(2) = cam.u.at<float>(2);
      cam.rotate(rot, -0.04); //up 1/100 of a rad
      break;
    case 's':
      //Pitch down
      rot.at<float>(0) = cam.u.at<float>(0);
      rot.at<float>(1) = cam.u.at<float>(1);
      rot.at<float>(2) = cam.u.at<float>(2);
      cam.rotate(rot, 0.04); //down 1/100 of a rad
      break;
    case 'z':
      //Roll left
      rot.at<float>(0) = cam.n.at<float>(0);
      rot.at<float>(1) = cam.n.at<float>(1);
      rot.at<float>(2) = cam.n.at<float>(2);
      cam.rotate(rot, 0.04); //left 1/100 of a rad
      break;
    case 'c':
      //Roll right
      rot.at<float>(0) = cam.n.at<float>(0);
      rot.at<float>(1) = cam.n.at<float>(1);
      rot.at<float>(2) = cam.n.at<float>(2);
      cam.rotate(rot, -0.04); //right 1/100 of a rad
      break;
    case 'r':
      //Zoom in
      cam.e = cam.e - 4.0*cam.n;
      cam.e.at<float>(3) = 1;
      break;
    case 'f':
      //Zoom out
      cam.e = cam.e + 4.0*cam.n;
      cam.e.at<float>(3) = 1;
      break;
    default:
      //Do nothing
      break;
    }

    ++frameCnt;

    if (frameCnt % 100 == 0){
      //std::cout << frameCnt / (((double)getTickCount() - ticks) / getTickFrequency()) << " fps\n";
    }
  }

  return 0;
}