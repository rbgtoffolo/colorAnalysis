#ifndef COLORANALYSIS_H
#define COLORANALYSIS_H

#include "ofMain.h"
#include <map>
#include <vector>
#include <iostream>

class ColorAnalysis
{
public:
  ColorAnalysis();
  ColorAnalysis(const ofImage& myImage);
  void extractColors();
  void setHueLimit(int limit){m_hueLimit = limit;};
  void setSaturationLimit(int limit){m_satLimit = limit;};
  void setBrightnessLimit(int limit){m_briLimit = limit;};
  int getHueLimit(){return m_hueLimit;};
  int getSaturationLimit(){return m_satLimit;};
  int getBrightnessLimit(){return m_briLimit;};
  void sortColors(); // sort Vector colors by most proeminent colors

// Debug and Inspect Funcions
  void printSortedMap();
  void printClassifiedColors();
  void printRowColors();
  void drawColors();

private:
  int m_hueLimit, m_satLimit, m_briLimit;
  int checkSimilarity(const ofColor &pixelColor);
  bool checkItemSimilarity(int color1, int color2, int limit);


private:
  std::vector<std::vector<ofColor>> m_classifiedColors;
  ofImage m_image;
  std::map<int, int> m_sortedColorsIndex;

};

#endif // COLORANALYSIS_H
