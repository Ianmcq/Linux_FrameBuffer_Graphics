#include "fbCanvas.h"
#include<iostream>
#include<fstream>

fbCanvas::fbCanvas(){
 for(int i = 0; i < height; i++){
  for(int j = 0; j < width; j++){
   screen[j][i] = new char[4];
  }
 }
}

void fbCanvas::drawPoint(int x, int y, char r, char g, char b, char a){
 screen[x][y][2] = r;
 screen[x][y][1] = g;
 screen[x][y][0] = b;
 screen[x][y][3] = a;
 return;
}

void fbCanvas::updateBuffer(){
 std::ofstream framebuffer;
 char * block;
 block = new char[height * width * 4];
 framebuffer.open("/dev/fb0", std::ios::binary | std::ios::out);
 int iter = 0;
 for(int i = 0; i < height; i++){
  for(int j = 0; j < width; j++){
   for(int k = 0; k < 4; k++){
    block[iter] = screen[j][i][k];
    iter++;
   }
  }
 }
 //int bad;
 //std::cin >> bad;
 framebuffer.write(block, height*width*4);
 framebuffer.close();
 return;
}

void fbCanvas::clear(){
 for(int i = 0; i < height; i++){
  for(int j = 0; j < width; j++){
   for(int k = 0; k < 4; k++){
    screen[j][i][k] = 0;
   }
  }
 }
}

void fbCanvas::drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a){
 if(x1 > width || x2 > width || y1 > height || y2 > height || x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0){
  return;
 }
 if(x1 == x2){
  if(y1 > y2){
   for(int i = y1; i >= y2; i--){
    drawPoint(x1,i,r,g,b,a);
   }
   return;
  } else {
   for(int i = y1; i < y2; i++){
    drawPoint(x1,i,r,g,b,a);
   }
   return;
  }
 }
 if(y1 == y2){
  if(x1 > x2){
   for(int i = x1; i >= x2; i--){
    drawPoint(i,y1,r,g,b,a);
   }
   return;
  } else {
   for(int i = x1; i < x2; i++){
    drawPoint(i,y1,r,g,b,a);
   }
   return;
  }
 }
 double m = ((double)y2-(double)y1)/((double)x2-(double)x1);
 float error;
 int xnow, ynow;
 //std::cout << m << "-(" << x1 << "," << x2 << std::endl;
 if(m > 0 && m <= 1 && x1 < x2){
  error = m - 0.5;
  ynow = y1;
  for(int i = x1; i <= x2; i++){
   drawPoint(i,ynow,r,g,b,a);
   error = error + m;
   if(error > 0.5){
    ynow++;
    error = error - 1;
   }
  }
  return;
 }
 if(m > 0 && m <= 1 && x1 > x2){
  //std::cout << "I'ts in!!" << std::endl;
  error = m - 0.5;
  ynow = y1;
  for(int i = x1; i >= x2; i--){
   drawPoint(i,ynow,r,g,b,a);
   error = error + m;
   if(error > 0.5){
    ynow--;
    error = error - 1;
   }
  }
  return;
 }
 if(m < 0 && m >= -1 && x1 < x2){
  //std::cout << "I'ts in!!" << std::endl;
  error = m + 0.5;
  ynow = y1;
  for(int i = x1; i <= x2; i++){
   drawPoint(i,ynow,r,g,b,a);
   error = error - m;
   if(error > 0.5){
    ynow--;
    error = error - 1;
   }
  }
  return;
 }
 if(m < 0 && m >= -1 && x1 > x2){
  //std::cout << "I'ts in!!" << std::endl;
  error = m + 0.5;
  ynow = y1;
  for(int i = x1; i >= x2; i--){
   drawPoint(i,ynow,r,g,b,a);
   error = error - m;
   if(error > 0.5){
    ynow++;
    error = error - 1;
   }
  }
  return;
 }
 if(m >= 1 && y1 < y2){
  //std::cout << "I'ts in!!" << std::endl;
  m = -1.0/m;
  error = m + 0.5;
  xnow = x1;
  for(int i = y1; i <= y2; i++){
   drawPoint(xnow,i,r,g,b,a);
   error = error - m;
   if(error > 0.5){
    xnow++;
    error = error - 1;
   }
  }
  return;
 }
 if(m >= 1 && y1 > y2){
  //std::cout << "I'ts in!!" << std::endl;
  m = -1.0/m;
  error = m + 0.5;
  xnow = x1;
  for(int i = y1; i >= y2; i--){
   drawPoint(xnow,i,r,g,b,a);
   error = error - m;
   if(error > 0.5){
    xnow--;
    error = error - 1;
   }
  }
  return;
 }
 if(m <= -1 && y1 < y2){
  //std::cout << "I'ts in!!" << std::endl;
  m = -1.0/m;
  error = m - 0.5;
  xnow = x1;
  for(int i = y1; i <= y2; i++){
   drawPoint(xnow,i,r,g,b,a);
   error = error + m;
   if(error > 0.5){
    xnow--;
    error = error - 1;
   }
  }
  return;
 }
 m = -1.0/m;
 error = m - 0.5;
 xnow = x1;
 for(int i = y1; i >= y2; i--){
  drawPoint(xnow,i,r,g,b,a);
  error = error + m;
  if(error > 0.5){
   xnow++;
   error = error - 1;
  }
 }
 return;
}
