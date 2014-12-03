/*
  main.cpp

  CS3388 Assignment 4

  By: Andrew Simpson
  SN: 250 633 280
  EM: asimps53@uwo.ca

  The main file of my ray tracing program.

  Sets up everything needed for making a basic scene and
  provides a simple text-based menu for inserting and removing
  objects from the scene.

*/

#include <iostream> //console io for user
#include <fstream> //read and write to files
#include <string> //used to read lines from file
#include <vector> //use to hold lists

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "GenericObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"

#include "GenericLight.h"
#include "PointLight.h"
#include "SunLight.h"

#include "Camera.h"
#include "RenderUtils.h"
#include "ImageManip.h"
#include "RayTracer.h"

#include "TextUI.h"

#define PI ((float)3.14159265358979323846)
#define WIDTH (640/1) // Adjust this to speed up rendering for debugging
#define HEIGHT (480/1)
#define VIEWING_ANGLE (float)45.0
#define N 5.0
//#define F 25.0f

using namespace std;
using namespace cv;

#define DEBUG

// Just initializes all of the objects to the desired default scene
void setupDefaultScene(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour,
    vector<GenericLight*> &lights, PointLight &ambientLight);

// Initializes the second default scene. Shifts existing objects. Adds a sun light.
void setupScene2(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour, 
    vector<GenericLight*> &lights, PointLight &ambientLight);

// Initializes the Christmas scene. Shifts existing objects. Adds a sun light.
void buildSnowman(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour,
  vector<GenericLight*> &lights, PointLight &ambientLight);

int main(int argc, char **argv)
{
  // Declare objects list
  vector<GenericObject*> objects;

  // Declare camera
  Camera cam;

  // Ray tracing window parameters
  const float AR = ((float)WIDTH) / HEIGHT;
  const float H = N*(float)tan(PI*VIEWING_ANGLE / 360);
  const float W = H*AR;

  // Declare colour
  Scalar backColour;
  
  // Declare lights list
  vector<GenericLight*> lights;

  // Declare the ambient light
  PointLight ambientLight;

  // Setup function
  setupDefaultScene(objects, cam, backColour, lights, ambientLight);
  setupScene2(objects, cam, backColour, lights, ambientLight);
  if (TextUI::snowManPrompt())
    buildSnowman(objects, cam, backColour, lights, ambientLight);


  // Allocate the image to draw to
  namedWindow("Render Window", WINDOW_AUTOSIZE);
  Mat image = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
  Mat renderImage = Mat::zeros(HEIGHT * 2, WIDTH * 2, CV_8UC3);
  if (!image.isContinuous()||!renderImage.isContinuous())
  {
    // Quit, my hack won't work (this will only happen with large
    // images or little memory - it's never happened to me during
    // testing).
    return 1;
  }

  // Display the startup message before we basically freeze
  TextUI::displayHeader();
  TextUI::displayStartupMessage();

  // Enter drawing loop
  while (1)
  {
    // Trace rays for each pixel
    RayTracer::traceImage(renderImage, W, H, N, objects, cam, backColour, lights, ambientLight);

    // Show the image
    resize(renderImage, image, Size(image.cols, image.rows));
    imshow("Render Window", image);

    // Used for moving the camera
    Mat rot = Mat::zeros(4, 1, CV_32FC1);

    // Handle the menu
    TextUI::displayHeader();
    TextUI::displayMenu();
    bool quitting = false;
    bool cont = false;
    while (!cont)
    {
      char c;
      c = cvWaitKey(10); // wWit 10 ms for key stroke
      // Only draw update if needed!!!
      if ((c == 27) || (c == 'q'))
      {
        quitting = true;
        break; // if ESC, break and quit
      }
      if (c == -1)
        continue; // Keep waiting for a key press

      //Handle each character that can be used for input
      switch (c){
      case 'a':
        //Yaw right
        rot.at<float>(0) = 0;
        rot.at<float>(1) = 0;
        rot.at<float>(2) = 1;
        cam.rotate(rot, 0.1f); //right 1/100 of a rad
        break;
      case 'd':
        //Yaw left
        rot.at<float>(0) = 0;
        rot.at<float>(1) = 0;
        rot.at<float>(2) = 1;
        cam.rotate(rot, -0.1f); //left 1/100 of a rad
        break;
      case 'w':
        //Pitch up
        rot.at<float>(0) = cam.u.at<float>(0);
        rot.at<float>(1) = cam.u.at<float>(1);
        rot.at<float>(2) = cam.u.at<float>(2);
        cam.rotate(rot, -0.1f); //up 1/10 of a rad
        break;
      case 's':
        //Pitch down
        rot.at<float>(0) = cam.u.at<float>(0);
        rot.at<float>(1) = cam.u.at<float>(1);
        rot.at<float>(2) = cam.u.at<float>(2);
        cam.rotate(rot, 0.1f); //down 1/10 of a rad
        break;
      case 'z':
        //Roll left
        rot.at<float>(0) = cam.n.at<float>(0);
        rot.at<float>(1) = cam.n.at<float>(1);
        rot.at<float>(2) = cam.n.at<float>(2);
        cam.rotate(rot, 0.1f); //left 1/10 of a rad
        break;
      case 'c':
        //Roll right
        rot.at<float>(0) = cam.n.at<float>(0);
        rot.at<float>(1) = cam.n.at<float>(1);
        rot.at<float>(2) = cam.n.at<float>(2);
        cam.rotate(rot, -0.1f); //right 1/10 of a rad
        break;
      case 'r':
        //Zoom in
        cam.e = cam.e - 0.8*cam.n;
        cam.e.at<float>(3) = 1;
        break;
      case 'f':
        //Zoom out
        cam.e = cam.e + 0.8*cam.n;
        cam.e.at<float>(3) = 1;
        break;
      case 'k':
        TextUI::addLight(lights);
        TextUI::displayMenu();
        break;
      case 'l':
        TextUI::removeLight(lights);
        TextUI::displayMenu();
        break;
      case 'n':
        //Insert a new object
        TextUI::addObject(objects);
        TextUI::displayMenu();
        break;
      case 'm':
        //Delete an object
        TextUI::removeObject(objects);
        TextUI::displayMenu();
        break;
      case 't':
        //Transform an object
        TextUI::transformObject(objects);
        TextUI::displayMenu();
        break;
      case 'g':
        //Adjust ambient lighting
        TextUI::adjustAmbient(ambientLight);
        TextUI::displayMenu();
        break;
      case ' ':
        //Continue to rendering the next frame
        cont = true;
        break;
      default:
        //Do nothing
        break;
      }
    }

    if (quitting)
      break;

    TextUI::displayProcessingMessage();
  }

  // Free everything in the objects vector
  while (!objects.empty())
  {
    // Delete
    delete objects.back();
    // Remove from list
    objects.pop_back();
  }

  return 0;
}

void setupDefaultScene(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour, 
    vector<GenericLight*> &lights, PointLight &ambientLight){
  // Setup objects
  // Insert the default objects
  Scalar rho_ad(0.25, 0.25, 0.5), rho_s(0.333, 0.333, 0.333);
  cv::Mat trans = cv::Mat::eye(4, 4, CV_32FC1);
  //trans.at<float>(2, 3) = 0.6f;
  trans.at<float>(2, 3) = 0.8f;
  Sphere *defaultSphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
  defaultSphere->name = "Default red sphere";
  objects.push_back(defaultSphere);
  rho_ad = Scalar(0.5, 0.25, 0.25);
  rho_s = Scalar(0.45, 0.25, 0.25);
  trans = trans*0.25; // Scale down
  trans.at<float>(0, 3) = 1.0f;
  trans.at<float>(1, 3) = -1.0f;
  trans.at<float>(2, 3) = 1.0f;
  trans.at<float>(3, 3) = 1.0f;
  Sphere *sphere2 = new Sphere(trans, rho_ad, rho_ad, rho_s);
  sphere2->name = "Default blue sphere";
  objects.push_back(sphere2);
  // Add a gray plane
  rho_ad = Scalar(0.333, 0.333, 0.333);
  rho_s = Scalar(0.333, 0.333, 0.333);
  trans = cv::Mat::eye(4, 4, CV_32FC1);
  Plane *defaultPlane = new Plane(trans, rho_ad, rho_ad, rho_s);
  defaultPlane->name = "Default plane";
  objects.push_back(defaultPlane);



  // Setup camera
  // Camera position
  cam.e = Mat::ones(4, 1, CV_32FC1);
  cam.e.at<float>(0) = 5;
  cam.e.at<float>(1) = 5;
  cam.e.at<float>(2) = 5;
  // Gaze point
  cam.g = Mat::zeros(4, 1, CV_32FC1);
  cam.g.at<float>(2) = 0;
  cam.g.at<float>(3) = 1;
  // Up vector
  Mat p = Mat::zeros(4, 1, CV_32FC1);
  p.at<float>(2) = 1;
  p.at<float>(3) = 1;
  // Camera's m, u, and v vectors
  cam.n = RenderUtils::homoNormalize(cam.e - cam.g);
  cam.u = RenderUtils::homoNormalize(RenderUtils::homoCross(p, cam.n));
  cam.v = RenderUtils::homoNormalize(RenderUtils::homoCross(cam.n, cam.u));



  // Setup background colour
  backColour = Scalar(30, 30, 30);
  


  // Setup light sources
  GenericLight *light = new PointLight();
  light->centre = Mat::ones(4, 1, CV_32FC1);
  light->centre = light->centre * 5;
  light->centre.at<float>(0) = -light->centre.at<float>(0);
  light->centre.at<float>(3) = 1; // Point
  light->direction = Mat::zeros(4, 1, CV_32FC1); // Zero vector
  light->intensity = Scalar(0.1, 0.5, 0.1); // Mostly green light
  light->name = "Default green light";
  lights.emplace_back(light);
  light = new PointLight();
  light->centre = Mat::ones(4, 1, CV_32FC1);
  light->centre = light->centre * 5;
  light->centre.at<float>(0) = -light->centre.at<float>(0);
  light->centre.at<float>(1) = -light->centre.at<float>(1);
  light->centre.at<float>(3) = 1; // Point
  light->direction = Mat::zeros(4, 1, CV_32FC1); // Zero vector
  light->intensity = Scalar(0.5, 0.1, 0.1); // Mostly blue light
  light->name = "Default blue light";
  lights.emplace_back(light);
  light = new PointLight();
  light->centre = Mat::ones(4, 1, CV_32FC1);
  light->centre = light->centre * 5;
  light->centre.at<float>(0) = sqrt(pow(light->centre.at<float>(0), 2) * 2);
  light->centre.at<float>(1) = 0;
  light->centre.at<float>(3) = 1; // Point
  light->direction = Mat::zeros(4, 1, CV_32FC1); // Zero vector
  light->intensity = Scalar(0.1, 0.1, 0.5); // Mostly red light
  light->name = "Default red light";
  lights.emplace_back(light);

  // Setup ambient light
  ambientLight.centre = Mat::ones(4, 1, CV_32FC1); // Location is unused
  ambientLight.intensity = Scalar(0.4, 0.4, 0.4);
  ambientLight.name = "Ambient light";

  return;
}

void setupScene2(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour,
    vector<GenericLight*> &lights, PointLight &ambientLight){
  // Shift all objects
  Mat shiftTrans = Mat::eye(4, 4, CV_32FC1); // Do not scale!
  shiftTrans.at<float>(0, 3) = -5; // Move back 5 along X
  for (int i = 0; i < objects.size()-1; i++){
    objects[i]->transformBy(shiftTrans);
  }

  // Add a 'sun' light
  GenericLight *light = new SunLight();
  light->centre = Mat::ones(4, 1, CV_32FC1);
  light->centre.at<float>(0) = 0;
  light->centre.at<float>(1) = 1;
  light->centre.at<float>(2) = 20;
  light->centre.at<float>(3) = 1; // Point
  light->direction = RenderUtils::homoNormalize(-light->centre); // Aim at origin
  light->intensity = Scalar(0.75, 0.75, 0.75); // Fairly bright white
  light->name = "Scene2: 'sun' light";
  lights.emplace_back(light);

  // Add a black cylinder
  Scalar rho_ad(0.15, 0.15, 0.15), rho_s(0.1, 0.1, 0.1);
  cv::Mat trans = cv::Mat::eye(4, 4, CV_32FC1);
  trans = trans*0.25; // Scale down
  trans.at<float>(0, 0) = 0.2f;
  trans.at<float>(1, 1) = 0.2f;
  trans.at<float>(2, 2) = 0.75f;
  trans.at<float>(2, 3) = 1.5f;
  trans.at<float>(3, 3) = 1.0f;
  Cylinder *cylinder = new Cylinder(trans, rho_ad, rho_ad, rho_s);
  cylinder->name = "Black cylinder";
  objects.emplace_back(cylinder);

  // Move the cylinder over on the y-axis
  shiftTrans.at<float>(0, 3) = 0; // Move back 5 along X
  shiftTrans.at<float>(1, 3) = -5; // Move back 5 along Y
  cylinder->transformBy(shiftTrans);

  // Rotate the cone about its x axis
  Mat rotTrans = Mat::eye(4, 4, CV_32FC1);
  rotTrans.at<float>(1, 1) = cos(PI / 4);
  rotTrans.at<float>(1, 2) = -sin(PI / 4);
  rotTrans.at<float>(2, 1) = sin(PI / 4);
  rotTrans.at<float>(2, 2) = cos(PI / 4);

  // Scale before rotation
  trans = Mat::eye(4, 4, CV_32FC1);
  trans.at<float>(0, 0) = 0.2f;
  trans.at<float>(1, 1) = 0.2f;
  trans.at<float>(2, 2) = 0.75f;
  trans = rotTrans*trans;

  // Then shift up
  trans.at<float>(2, 3) = 1.5f;

  // Put a purple cone in the middle
  rho_ad = Scalar(0.45, 0.1, 0.45);
  rho_s = Scalar(0.3333, 0.3333, 0.3333);
  Cone *cone = new Cone(trans, rho_ad, rho_ad, rho_s);
  cone->name = "Purple cone";
  objects.emplace_back(cone);
}

void buildSnowman(vector<GenericObject*> &objects, Camera &cam, Scalar &backColour,
  vector<GenericLight*> &lights, PointLight &ambientLight){
  // Get current count of objects
  int startIdx = objects.size();

  // Shift all objects into background
  Mat shift = Mat::eye(4, 4, CV_32FC1);
  shift.at<float>(0, 3) = -3;
  shift.at<float>(1, 3) = -3;
  for (int i = 0; i < startIdx; i++){
    objects[i]->transformBy(shift);
  }

  // Build body
  float scale = 1.2;
  {
    Scalar rho_ad(0.9, 0.9, 0.9), rho_s(0.1, 0.1, 0.1);
    // BASE
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.5;
      trans.at<float>(2, 3) = 0.5;
      trans = trans * scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Snowman base";
      objects.emplace_back(sphere);
    }
    // TORSO
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.4;
      trans.at<float>(2, 3) = 1.2;
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Snowman torso";
      objects.emplace_back(sphere);
    }
    // HEAD
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.3;
      trans.at<float>(2, 3) = 1.7;
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Snowman head";
      objects.emplace_back(sphere);
    }
  }

  // Build buttons + eyes
  {
    Scalar rho_ad(0.1, 0.1, 0.1), rho_s(0.05, 0.05, 0.05);
    // Button A
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.05;
      trans.at<float>(0, 3) = 0.4*cos(25 * PI / 180);
      trans.at<float>(2, 3) = 1.2 - 0.4*sin(25*PI/180);
      trans = trans * scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Button A";
      objects.emplace_back(sphere);
    }
    // Button B
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.05;
      trans.at<float>(0, 3) = 0.4;
      trans.at<float>(2, 3) = 1.2;
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Button B";
      objects.emplace_back(sphere);
    }
    // Button C
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.05;
      trans.at<float>(0, 3) = 0.4*cos(25 * PI / 180);
      trans.at<float>(2, 3) = 1.2 + 0.4*sin(25 * PI / 180);
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Button C";
      objects.emplace_back(sphere);
    }

    // Right Eye
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.05;
      trans.at<float>(0, 3) = sqrt(0.3*0.3 - 0.03*0.03)*cos(16 * PI / 180);
      trans.at<float>(1, 3) = -sqrt(0.3*0.3 - 0.03*0.03)*sin(16 * PI / 180);
      trans.at<float>(2, 3) = 1.7 + 0.03;
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Right Eye";
      objects.emplace_back(sphere);
    }
    // Left Eye
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans = trans * 0.05;
      trans.at<float>(0, 3) = sqrt(0.3*0.3 - 0.03*0.03)*cos(16 * PI / 180);
      trans.at<float>(1, 3) = sqrt(0.3*0.3 - 0.03*0.03)*sin(16 * PI / 180);
      trans.at<float>(2, 3) = 1.7 + 0.03;
      trans = trans*scale;
      trans.at<float>(3, 3) = 1;
      Sphere *sphere = new Sphere(trans, rho_ad, rho_ad, rho_s);
      sphere->name = "Left Eye";
      objects.emplace_back(sphere);
    }
  }

  // Build Nose
  {
    Scalar rho_ad(0, (float)102/255, 1.0), rho_s(0.05, 0.05, 0.05);
    // Nose
    {
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      // Scale
      trans.at<float>(0, 0) = 0.04 * scale;
      trans.at<float>(1, 1) = 0.04 * scale;
      trans.at<float>(2, 2) = 0.3 / 2 * scale;
      trans = trans * scale;
      trans.at<float>(3, 3) = 1;
      Cone *cone = new Cone(trans, rho_ad, rho_ad, rho_s);
      cone->name = "Nose";
      // Rotate
      Mat rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(0, 0) = rot.at<float>(2,2) = cos(PI / 2);
      rot.at<float>(0, 2) = sin(PI / 2);
      rot.at<float>(2, 0) = -sin(PI / 2);
      cone->transformBy(rot);
      // Translate
      Mat transl = Mat::eye(4, 4, CV_32FC1);
      transl.at<float>(0, 3) = (0.3 + 0.14) * scale;
      transl.at<float>(2, 3) = (1.7 - 0.03) * scale;
      cone->transformBy(transl);
      objects.emplace_back(cone);
    }
  }

  // Build Hat
  {
    Scalar rho_ad(0.1, 0.1, 0.1), rho_s(0.35, 0.35, 0.35);
    // Barrel
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans.at<float>(0, 0) = 0.38 / 2 * scale;
      trans.at<float>(1, 1) = 0.38 / 2 * scale;
      trans.at<float>(2, 2) = 0.35 / 2 * scale;
      trans.at<float>(2, 3) = (0.35) * scale; // Penetrate head a bit
      Cylinder *cylinder = new Cylinder(trans, rho_ad, rho_ad, rho_s);
      cylinder->name = "Hat Barrel";
      Mat rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(0, 0) = rot.at<float>(2, 2) = cos(15 * PI / 180);
      rot.at<float>(0, 2) = -sin(15 * PI / 180);
      rot.at<float>(2, 0) = sin(15 * PI / 180);
      cylinder->transformBy(rot);
      Mat transl = Mat::eye(4, 4, CV_32FC1);
      transl.at<float>(2, 3) = 1.7 * scale;
      cylinder->transformBy(transl);
      objects.emplace_back(cylinder);
    }
    // Rim
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans.at<float>(0, 0) = 0.65 / 2 * scale;
      trans.at<float>(1, 1) = 0.65 / 2 * scale;
      trans.at<float>(2, 2) = 0.01 / 2 * scale;
      trans.at<float>(2, 3) = (0.25) * scale; // Penetrate head a bit
      Cylinder *cylinder = new Cylinder(trans, rho_ad, rho_ad, rho_s);
      cylinder->name = "Hat Rim";
      Mat rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(0, 0) = rot.at<float>(2, 2) = cos(15 * PI / 180);
      rot.at<float>(0, 2) = -sin(15 * PI / 180);
      rot.at<float>(2, 0) = sin(15 * PI / 180);
      cylinder->transformBy(rot);
      Mat transl = Mat::eye(4, 4, CV_32FC1);
      transl.at<float>(2, 3) = 1.7 * scale;
      cylinder->transformBy(transl);
      objects.emplace_back(cylinder);
    }
  }

  // Build Arms
  {
    Scalar rho_ad((float)19/255, (float)69/255, (float)139/255), rho_s(0.1, 0.1, 0.1);
    // Right Arm
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans.at<float>(0, 0) = 0.03 / 2 * scale;
      trans.at<float>(1, 1) = 0.03 / 2 * scale;
      trans.at<float>(2, 2) = 0.5 * scale;
      Cylinder *cylinder = new Cylinder(trans, rho_ad, rho_ad, rho_s);
      cylinder->name = "Right Arm";
      Mat rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(1, 1) = rot.at<float>(2, 2) = cos(70 * PI / 180);
      rot.at<float>(1, 2) = -sin(70 * PI / 180);
      rot.at<float>(2, 1) = sin(70 * PI / 180);
      cylinder->transformBy(rot);
      Mat transl = Mat::eye(4, 4, CV_32FC1);
      transl.at<float>(1, 3) = -0.6 * scale;
      transl.at<float>(2, 3) = 1.4 * scale;
      cylinder->transformBy(transl);
      rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(0, 0) = rot.at<float>(1, 1) = cos(30 * PI / 180);
      rot.at<float>(0, 1) = -sin(30 * PI / 180);
      rot.at<float>(1, 0) = sin(30 * PI / 180);
      cylinder->transformBy(rot);
      objects.emplace_back(cylinder);
    }
    // Left Arm
    {
      // Force transform to go out of scope
      Mat trans = Mat::eye(4, 4, CV_32FC1);
      trans.at<float>(0, 0) = 0.03 / 2 * scale;
      trans.at<float>(1, 1) = 0.03 / 2 * scale;
      trans.at<float>(2, 2) = 0.5 / 2 * scale;
      Cylinder *cylinder = new Cylinder(trans, rho_ad, rho_ad, rho_s);
      cylinder->name = "Left Arm";
      Mat rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(1, 1) = rot.at<float>(2, 2) = cos(-70 * PI / 180);
      rot.at<float>(1, 2) = -sin(-70 * PI / 180);
      rot.at<float>(2, 1) = sin(-70 * PI / 180);
      cylinder->transformBy(rot);
      Mat transl = Mat::eye(4, 4, CV_32FC1);
      transl.at<float>(1, 3) = 0.6 * scale;
      transl.at<float>(2, 3) = 1.4 * scale;
      cylinder->transformBy(transl);
      rot = Mat::eye(4, 4, CV_32FC1);
      rot.at<float>(0, 0) = rot.at<float>(1, 1) = cos(-30 * PI / 180);
      rot.at<float>(0, 1) = -sin(-30 * PI / 180);
      rot.at<float>(1, 0) = sin(-30 * PI / 180);
      cylinder->transformBy(rot);
      objects.emplace_back(cylinder);
    }
  }

  // Get the last index
  int endIdx = objects.size();
  Mat trans = Mat::eye(4, 4, CV_32FC1);
  trans.at<float>(0, 3) = 0.5;
  trans.at<float>(1, 3) = 0.5;
  Mat rot = Mat::eye(4, 4, CV_32FC1);
  rot.at<float>(0, 0) = rot.at<float>(1, 1) = cos(35 * PI / 180);
  rot.at<float>(0, 1) = -sin(35 * PI / 180);
  rot.at<float>(1, 0) = sin(35 * PI / 180);
  for (int i = startIdx; i < endIdx; i++){
    objects[i]->transformBy(rot);
    objects[i]->transformBy(trans);
  }
}