#include "coloranalysis.h"
#include "progressbar.h"


ColorAnalysis::ColorAnalysis(){} // Default constructor

ColorAnalysis::ColorAnalysis(const ofImage &myImage)
{
  m_image = myImage;
  m_hueLimit = 7.0f; // deviation factor to consider a hue as a same color (+7, -7);
  m_satLimit = 20.0f; // deviation factor to consider a saturation as a same color (+20, -20);
  m_briLimit = 20.0f; // deviation factor to consider a brightness as a same color (+20, -20);

}
// TODO: correct algorithm error: Saturation and Brightness are not circular spaces!!!

void ColorAnalysis::extractColors()
{
  ofColor tempColor;
  progressBar progress{std::cout, 70u, "Analysing"}; /// uncomment to use progress bar
  int width = m_image.getWidth();
  int height = m_image.getHeight();
  float imageSize = (float)width*(float)height;
  float i = 0; // uncomment to use progress bar
  m_classifiedColors.reserve(imageSize);
  for  (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
      tempColor = m_image.getColor(x,y);

      /// For debug
      // std::cout << "Pix-> HSB: " << tempColor.getHue() << ", " << tempColor.getSaturation() << ", " << tempColor.getBrightness() << " pixel: " << i << std::endl;
      // std::cin.get();
      ///

      int position = checkSimilarity(tempColor);
      if (position != -1)
        {
          /// For debug
          // std::cout << "Putting this color in Position: "<< position ;
          // std::cout << " - Pix-> HSB: " << tempColor.getHue() << ", " << tempColor.getSaturation() << ", " << tempColor.getBrightness() << std::endl;
          // std::cin.get();
          ///
          m_classifiedColors[position].emplace_back(tempColor);
        }
      else
        {
          /// For debug
          // std::cout << "No match founded. Putting this color at end: ";
          //  std::cout << " - Pix-> HSB: " << tempColor.getHue() << ", " << tempColor.getSaturation() << ", " << tempColor.getBrightness() << std::endl;
           ///

          std::vector<ofColor> tempVector = {tempColor};
          m_classifiedColors.emplace_back(tempVector);
        }

      i++; // uncomment to use progress bar
      progress.write(i/imageSize);

    }
  }
}

int ColorAnalysis::checkSimilarity(const ofColor &pixelColor)
{
  if (m_classifiedColors.empty()){
      /// For debug
      // std::cout << "main array yet empty " << std::endl;
      ///
    return -1;
}

  float tempH, tempS, tempB;
  float testH, testS, testB;
  int size = (int)m_classifiedColors.size();
  testH = pixelColor.getHue();
  testS = pixelColor.getSaturation();
  testB = pixelColor.getBrightness();


  for (int i = 0; i < size; i++)
    {
      /// For Debug
      // std::cout << "check similarity loop: " << i << std::endl;
      ///
      std::vector<ofColor> tempVector;
      tempVector = m_classifiedColors[i];

      for (auto it1 : tempVector)
        {
          tempH = it1.getHue();
          tempS = it1.getSaturation();
          tempB = it1.getBrightness();
          /// For Debug
          // std::cout << "comparando pix com:";
          //           std::cout << " - HSB: " << tempH << ", " << tempS << ", " << tempB << std::endl;
          // //std::cin.get();
          ///
          // se cor for similar a alguma do array
          bool hue = checkHueSimilarity(testH, tempH, m_hueLimit);
          bool sat = checkSatBriSimilarity(testS, tempS, m_satLimit);
          bool bri = checkSatBriSimilarity(testB, tempB, m_briLimit);

          if (hue && sat && bri){
              // std::cout << "3 trues " << " i= " << i << std::endl;
            return i;
            }
        }
    }
  // se não achou nenhuma cor similar
  // std::cout << " uma delas não similar " << std::endl;
  return -1;
}

bool ColorAnalysis::checkHueSimilarity(float color1, float color2, float limit){
  bool result = false;
  //  std::cout << "color1: " << color1 << " color2: " << color2 << " limit " << limit << std::endl;
  // caso esteja no limite normal para comparar
  if ( (color1 < 255-limit && color1 > limit) && (color2 < 255-limit && color2 > limit) ){
      // std::cout << "if 1 ";


      if ( abs(color2-color1) <= limit )
        result = true;
      else
        result = false;

      // std::cout << "result " << result << std::endl;
            //std::cin.get();
    }

  // caso cor1 esteja na área de cálculo positivo e cor2 esteja nos extremos
  if ( (color1 > limit && color1 < 255-limit) && ((color2 >= 0 && color2 <= limit) || (color2 >= 255-limit && color2 <= 255)) ){
      // std::cout << "if 2 ";
      if (abs(color2-color1) <= limit)
        result = true;
      else
        result = false;

      // std::cout << "result " << result << std::endl;
            //std::cin.get();
    }

  // caso cor1 esteja nos extremos e cor2 na área de cálculo positivo
  if ( ((color1 >= 0 && color1 <= limit) || (color1 >= 255-limit && color1 <= 255)) && (color2 > limit && color2 < 255-limit)  ){
      // std::cout << "if 3 ";
      if (abs(color2-color1) <= limit)
        result = true;
      else
        result = false;

      // std::cout << "result " << result << std::endl;
            //std::cin.get();
    }

  // caso cor1 e cor2 estiverem nos extremos

  if ( ((color1 >= 255-limit && color1 <=255) || (color1 >=0 && color1 <= limit)) && ((color2 >= 255-limit && color2 <=255) || (color2 >=0 && color2 <= limit)) ){
      // std::cout << "if 4 ";
      if ( ((color1 >= 255-limit-color2) && (color1 <= 255)) || (color1 >= 0 && (color1 <= limit - 255-color2)) || ((color2 <= limit - 255-color1) && color2 >= 0) || (abs(color2-color1)<= limit))
        result = true;
      else
        result = false;

      // std::cout << "result " << result << std::endl;
            //std::cin.get();
    }
  return result;
}

bool ColorAnalysis::checkSatBriSimilarity(float color1, float color2, float limit){
  // caso esteja no limite normal para comparar

  // std::cout << "sat Bri " << std::endl;
  //     std::cout << "color1: " << color1 << " color2: " << color2 << " limit " << limit << std::endl;
       if ( abs(color2-color1) <= limit ){
        // std::cout << "SB true" << std::endl;
        //std::cin.get();
          return true;
        }
      else {
          // std::cout << "SB false" << std::endl;
          //std::cin.get();

        return false;
        }
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
          std::cout << "HSB (" << it1.getHue() << ", " << it1.getSaturation() << ", " << it1.getBrightness() << ") - RGB (" ;
          std::cout << (int)it1.r << ", " << (int)it1.g << ", " << (int)it1.b << ")" << std::endl;
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
  int width = m_image.getWidth()-1;
  for (int i =0; i < size; i++)
    {
      std::vector<ofColor> tempVector;
      tempVector = m_classifiedColors[i];
      for (auto it1 : tempVector)
        {
          tempImage.setColor(x,y,it1);
          // std::cout << "(" << x << ", " << y <<") - RBG: " << (int)it1.r << ", " << (int)it1.g << ", " << (int)it1.b << ")" << std::endl;
          x++;
          if (x > width){
              x=0;
              y++;
            }
        }

    }
  tempImage.update();
  tempImage.draw(posX, posY);
  tempImage.save("tempImage.jpg");
}
