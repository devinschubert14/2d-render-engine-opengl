#include "utils.hpp"

RGB hex2rgb(int hexValue)
{
  RGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

  return rgbColor; 
}
