#ifndef FBCANVAS_H
#define FBCANVAS_H
#include<math.h>
#include<cstdlib>
#include<array>

class fbCanvas{
 private:
  static const int width = 1280, height = 800;
  std::array<std::array<char *, height >, width > screen;//Must be changed to screen pixel dimensions on port
 public:
  fbCanvas();
  void drawPoint(int x, int y, char r, char g, char b, char a);
  void updateBuffer();
  void clear();
  void drawLine(int x1, int y1, int x2, int y2, char r, char g, char b, char a);
  void printText(int x, int y, std::string s);
};
#endif
