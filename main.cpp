#include<linux/fb.h>//TODO use
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include"fbCanvas.h"
#include<unistd.h>
#include<term.h>
#include<array>
#include<chrono>
#include<thread>

//TODO more documentation
int main(){
 fbCanvas * canvas = new fbCanvas();
 canvas->drawLine(300,300,300,100,100,100,100,255);
 canvas->drawLine(300,300,400,100,255,0,0,255);
 canvas->drawLine(300,300,200,100,0,255,0,255);
 canvas->drawLine(300,300,100,300,100,100,100,255);
 canvas->drawLine(300,300,100,200,0,0,255,255);
 canvas->drawLine(300,300,100,400,0,255,255,255);
 canvas->drawLine(300,300,300,500,100,100,100,255);
 canvas->drawLine(300,300,200,500,255,0,255,255);
 canvas->drawLine(300,300,400,500,255,255,0,255);
 canvas->drawLine(300,300,500,300,100,100,100,255);
 canvas->drawLine(300,300,500,200,255,255,255,255);
 canvas->drawLine(300,300,500,400,25,25,25,255);
 canvas->updateBuffer();
 float pi = 3.14159265358979;
 std::array<std::array < float, 3 >, 8 > corners;//cube corner coordinates in 3 space
 std::array<std::array < float, 2 >, 8 > flatcorners;//cube corner coordinates in 2 space after projection
 std::array<std::array < double, 3 >, 3 > xrot = { { {1,0,0}, {0, cos(30*pi/180), -1 * sin(30*pi/180)}, {0, sin(30*pi/180), cos(30*pi/180)} } };//30 degree rotation about x axis to get cube off grid
 std::array<std::array < double, 3 >, 3 > yrot = { { {cos(pi/180),0,sin(pi/180)}, {0,1,0},{-1 * sin(pi/180), 0, cos(pi/180)} } };//1 degree rotation about y axis, will be repeatedly applied
 std::array<std::array < double, 3 >, 3 > zrot = { { {cos(5*pi/180),-1 * sin(5*pi/180),0}, {sin(5*pi/180),cos(5*pi/180),0},{0, 0, 1} } };//5 degree rotation about z axis to get cube off grid
 std::array<float, 3> pinhole = {{0,0,-300}};
 float planedist = -1.0;
 float flattenz [3][3] = { {1,0,0}, {0,1,0}, {0,0,0} };//projects cube into 2d
 corners[0] = {100,100,100};//x,y,z cube corner coordinates initialization
 corners[1] = {-100,100,100};
 corners[2] = {100,-100,100};
 corners[3] = {-100,-100,100};
 corners[4] = {100,100,-100};
 corners[5] = {-100,100,-100};
 corners[6] = {100,-100,-100};
 corners[7] = {-100,-100,-100};
 float product [3];//holds dot products during matrix calculations
 for(int i = 0; i < 8; i++){//apply x rotation
  for(int j = 0; j < 3; j++){
   product[j] = corners[i][0] * xrot[0][j] + corners[i][1] * xrot[1][j] + corners[i][2] * xrot[2][j];
  }
  corners[i][0] = product[0];
  corners[i][1] = product[1];
  corners[i][2] = product[2];
 }
 for(int i = 0; i < 8; i++){//apply z rotation
  for(int j = 0; j < 3; j++){
   product[j] = corners[i][0] * zrot[0][j] + corners[i][1] * zrot[1][j] + corners[i][2] * zrot[2][j];
  }
  corners[i][0] = product[0];
  corners[i][1] = product[1];
  corners[i][2] = product[2];
 }
 int end = 360;
 std::this_thread::sleep_for(std::chrono::milliseconds(3000));
 for(int lc = 0; lc < end; lc++){//animation loop
  canvas->updateBuffer();
  canvas->clear();
  for(int i = 0; i < 8; i++){//apply y rotation
   for(int j = 0; j < 3; j++){
    product[j] = corners[i][0] * yrot[0][j] + corners[i][1] * yrot[1][j] + corners[i][2] * yrot[2][j];
   }
   corners[i][0] = product[0];
   corners[i][1] = product[1];
   corners[i][2] = product[2];
  }
  for(int i = 0; i < 8; i++){//flatten
   //(y-y1)=((y2-y1)/(x2-x1)) * (x-x1) //but not this
   //(y-y1)=((y2-y1)/(z2-z1)) * (z-z1) //this
   //(z-z1)=((z2-z1)/(x2-x1)) * (x-x1) //and this
   //corners[i] is point 1
   //pinhole is point 2
   //y=((pinhole[1]-corners[i][1])/(pinhole[0]-corners[i][0])) * (x-corners[i][0]) + corners[i][1]
   //y=((pinhole[1]-corners[i][1])/(pinhole[2]-corners[i][2])) * ((pinhole[2]+planedist)-corners[i][2]) + corners[i][1] 
   //pinhole[2]+planedist-corners[i][2] = ((pinhole[2]-corners[i][2])/(pinhole[0]-corners[i][0])) * (x-corners[i][0])
   //    a          b           c                d            e             f            g                   h
   //not working
   //flatcorners[i][0] = ((pinhole[0]-corners[i][0])/(pinhole[2]-corners[i][2])) * ((pinhole[2]+planedist)-corners[i][2]) + corners[i][0];
   //flatcorners[i][1] = ((pinhole[1]-corners[i][1])/(pinhole[2]-corners[i][2])) * ((pinhole[2]+planedist)-corners[i][2]) + corners[i][1];
   for(int j = 0; j < 3; j++){
    product[j] = corners[i][0] * flattenz[0][j] + corners[i][1] * flattenz[1][j] + corners[i][2] * flattenz[2][j];
   }
   flatcorners[i][0] = product[0];
   flatcorners[i][1] = product[1];
  }
  canvas->drawLine(flatcorners[0][0] + 600,flatcorners[0][1] + 300,flatcorners[1][0] + 600 ,flatcorners[1][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[0][0] + 600,flatcorners[0][1] + 300,flatcorners[2][0] + 600 ,flatcorners[2][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[0][0] + 600,flatcorners[0][1] + 300,flatcorners[4][0] + 600 ,flatcorners[4][1] + 300,255,lc,lc,100);

  canvas->drawLine(flatcorners[5][0] + 600,flatcorners[5][1] + 300,flatcorners[1][0] + 600 ,flatcorners[1][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[5][0] + 600,flatcorners[5][1] + 300,flatcorners[7][0] + 600 ,flatcorners[7][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[5][0] + 600,flatcorners[5][1] + 300,flatcorners[4][0] + 600 ,flatcorners[4][1] + 300,255,lc,lc,100);

  canvas->drawLine(flatcorners[3][0] + 600,flatcorners[3][1] + 300,flatcorners[1][0] + 600 ,flatcorners[1][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[3][0] + 600,flatcorners[3][1] + 300,flatcorners[7][0] + 600 ,flatcorners[7][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[3][0] + 600,flatcorners[3][1] + 300,flatcorners[2][0] + 600 ,flatcorners[2][1] + 300,255,lc,lc,100);

  canvas->drawLine(flatcorners[6][0] + 600,flatcorners[6][1] + 300,flatcorners[2][0] + 600 ,flatcorners[2][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[6][0] + 600,flatcorners[6][1] + 300,flatcorners[7][0] + 600 ,flatcorners[7][1] + 300,255,lc,lc,100);
  canvas->drawLine(flatcorners[6][0] + 600,flatcorners[6][1] + 300,flatcorners[4][0] + 600 ,flatcorners[4][1] + 300,255,lc,lc,100);

 }
 for(int i = 0; i < 256; i++){
  for(int j = 0; j < 256; j++){
   canvas->drawPoint(i,j,i,j,0,0);
  }
 }
 for(int i = 0; i < 256; i++){
  for(int j = 0; j < 256; j++){
   canvas->drawPoint(i+256,j,0,i,j,0);
  }
 }
 for(int i = 0; i < 256; i++){
  for(int j = 0; j < 256; j++){
   canvas->drawPoint(i,j+256,i,0,j,0);
  }
 }
 canvas->updateBuffer();
 return 0;
}
