#include "coloranalysis.h"
#include "progressbar.h"


ColorAnalysis::ColorAnalysis(){} // Default constructor

ColorAnalysis::ColorAnalysis(const ofImage &myImage)
{
  m_image = myImage;
  m_hueLimit = 7; // deviation factor to consider a hue as a same color (+7, -7);
  m_satLimit = 20; // deviation factor to consider a saturation as a same color (+20, -20);
  m_briLimit = 20; // deviation factor to consider a brightness as a same color (+20, -20);

}
// TODO: correct algorithm error: Saturation and Brightness are not circular spaces!!!

void ColorAnalysis::extractColors()
{
  ofColor tempColor;
  progressBar progress{std::clog, 70u, "Analysing"};
  int width = m_image.getWidth();
  int height = m_image.getHeight();
  float imageSize = (float)width*(float)height;
  float i = 0; // uncomment to use debug cout below
  m_classifiedColors.reserve(imageSize);
  for  (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
      tempColor = m_image.getColor(x,y);
      int position = checkSimilarity(tempColor);
      if (position != -1)
        {
          m_classifiedColors[position].emplace_back(tempColor);
        }
      else
        {
          std::vector<ofColor> tempVector = {tempColor};
          m_classifiedColors.emplace_back(tempVector);
        }

      i++; // uncomment to use debug cout in next line
      progress.write(i/imageSize);
      //std::cout << "Pass: " << i << " of " << imageSize << std::endl;
      //std::cout << "pass: " << i << " of "<< imageSize << " - " << tempColor.getHue() << ", " << tempColor.getSaturation() << ", " << tempColor.getBrightness() << std::endl;
    }
  }
}

int ColorAnalysis::checkSimilarity(const ofColor &pixelColor)
{
  if (m_classifiedColors.empty())
    return -1;


  int tempH, tempS, tempB;
  int testH, testS, testB;
  int size = (int)m_classifiedColors.size();
  testH = pixelColor.getHue();
  testS = pixelColor.getSaturation();
  testB = pixelColor.getBrightness();

  for (int i = 0; i < size; i++)
    {
      std::vector<ofColor> tempVector;
      tempVector = m_classifiedColors[i];
      for (auto it1 : tempVector)
        {
          tempH = it1.getHue();
          tempS = it1.getSaturation();
          tempB = it1.getBrightness();

          // se cor for similar a alguma do array
          bool hue = checkItemSimilarity(testH, tempH, m_hueLimit);
          bool sat = checkItemSimilarity(testS, tempS, m_satLimit);
          bool bri = checkItemSimilarity(testB, tempB, m_briLimit);

          if (hue && sat && bri)
            return i;
        }
    }
  // se não achou nenhuma cor similar
  return -1;
}

bool ColorAnalysis::checkItemSimilarity(int color1, int color2, int limit){
  bool result = false;
  // caso esteja no limite normal para comparar
  if ( (color1 < 255-limit && color1 > limit) && (color2 < 255-limit && color2 > limit) ){
      if ( abs(color2-color1) <= limit )
        result = true;
      else
        result = false;
    }

  // caso cor1 esteja na área de cálculo positivo e cor2 esteja nos extremos
  if ( (color1 > limit && color1 < 255-limit) && ((color2 >= 0 && color2 <= limit) || (color2 >= 255-limit && color2 <= 255)) ){
      if (abs(color2-color1) <= limit)
        result = true;
      else
        result = false;
    }

  // caso cor1 esteja nos extremos e cor2 na área de cálculo positivo
  if ( ((color1 >= 0 && color1 <= limit) || (color1 >= 255-limit && color1 <= 255)) && (color2 > limit && color2 < 255-limit)  ){
      if (abs(color2-color1) <= limit)
        result = true;
      else
        result = false;
    }

  // caso cor1 e cor2 estiverem nos extremos

  if ( ((color1 >= 255-limit && color1 <=255) || (color1 >=0 && color1 <= limit)) && ((color2 >= 255-limit && color2 <=255) || (color2 >=0 && color2 <= limit)) ){
      if ( ((color1 >= 255-limit-color2) && (color1 <= 255)) || (color1 >= 0 && (color1 <= limit - 255-color2)) || ((color2 <= limit - 255-color1) && color2 >= 0) || (abs(color2-color1)<= limit))
        result = true;
      else
        result = false;
    }
  return result;
}

void ColorAnalysis::sortColors()
{
  std::vector<ofColor> tempVector;
  for (int position = 0; position < (int)m_classifiedColors.size(); position++){
      tempVector = m_classifiedColors[position];
      int size = tempVector.size();
      m_sortedColorsIndex.insert({size, position});
    }
}

// Debug and inspect functions

void ColorAnalysis::printSortedMap(){
  for (auto it : m_sortedColorsIndex){
      std::cout << "quantity: " << it.first << " - position: " << it.second << std::endl;
    }
}

void ColorAnalysis::printClassifiedColors()
{
  std::cout << "*** Gruped Colors ****" << std::endl;
  for (int i =0; i < (int)m_classifiedColors.size(); i++)
    {
      std::vector<ofColor> tempVector;
      tempVector = m_classifiedColors[i];
      for (auto it1 : tempVector)
        {
          std::cout << "H: " << it1.getHue() << ", S: " << it1.getSaturation() << ", B: " << it1.getBrightness() << std::endl;
        }
      std::cout << " ************ " << std::endl;
    }
}

void ColorAnalysis::printRowColors(){
  ofColor tempColor;
  for  (int y = 0; y < m_image.getWidth(); y++){
      for (int x = 0; x < m_image.getHeight(); x++)
      tempColor = m_image.getColor(x,y);
      std::cout << "H: " << tempColor.getHue() << ", S: " << tempColor.getSaturation() << ", B: " << tempColor.getBrightness() << std::endl;
    }

}

void ColorAnalysis::drawColors(int posX, int posY){
  ofImage tempImage;
  tempImage.allocate(m_image.getWidth(), m_image.getHeight(), OF_IMAGE_COLOR);
  int x=0, y=0, size = (int)m_classifiedColors.size();
  int width = m_image.getWidth();
  for (int i =0; i < size; i++)
    {
      std::vector<ofColor> tempVector;
      tempVector = m_classifiedColors[i];
      for (auto it1 : tempVector)
        {
          tempImage.setColor(x,y,it1);
          x++;
          if (x > width){
              x=0;
              y++;
            }
        }

    }
  tempImage.update();
  tempImage.draw(posX, posY);
}
