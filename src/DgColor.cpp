#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgColor.cpp: DgColor class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <vector>

#include "DgBase.h"
#include "DgColor.h"
#include "DgUtil.h"

const float DgColor::undefColor = -1.0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static float
max3 (float a, float b, float c)
//
// Return the max of three values.
//
{
   float max = (a > b) ? a : b;
   return (c > max) ? c : max;

} // static float max3

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static float
min3 (float a, float b, float c)
//
// Return the min of three values.
//
{
   float min = (a < b) ? a : b;
   return (c < min) ? c : min;

} // static float min3

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void 
DgColor::linearSpreadRGB (const DgColor& col1, const DgColor& col2,
                          std::vector<DgColor*>& spread, int steps)
//
// Add to spread a steps-step linear interpolation in RGB space of colors 
// from col1 to col2 inclusive.
//
{
    float dRed, dGreen, dBlue;

    dRed   = (col2.red()   - col1.red())   / (float) (steps - 1);
    dGreen = (col2.green() - col1.green()) / (float) (steps - 1);
    dBlue  = (col2.blue()  - col1.blue())  / (float) (steps - 1);

    DgColor* lastColor = new DgColor(col1);
    spread.push_back(lastColor);
    for (int i = 1; i < steps; i++)
    {
       DgColor* nextColor = new DgColor(lastColor->red() + dRed, 
                 lastColor->green() + dGreen, lastColor->blue() + dBlue);
      
       spread.push_back(nextColor);

       lastColor = nextColor;
    }

} // void linearSpreadRGB

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void 
DgColor::linearSpreadHLS (const DgColor& col1, const DgColor& col2,
                          std::vector<DgColor*>& spread, int steps)
//
// Add to spread a steps-step linear interpolation in HLS space of colors 
// from col1 to col2 inclusive.
//
{
    float dHue, dLight, dSat;

    dHue   = (col2.hue()   - col1.hue())   / (float) (steps - 1);
    dLight = (col2.lightness() - col1.lightness()) / (float) (steps - 1);
    dSat  =  (col2.saturation()  - col1.saturation())  / (float) (steps - 1);

    DgColor* lastColor = new DgColor(col1);
    spread.push_back(lastColor);
    for (int i = 1; i < steps; i++)
    {
       DgColor* nextColor = new DgColor();
       nextColor->setHLS(lastColor->hue() + dHue, 
                         lastColor->lightness() + dLight, 
                         lastColor->saturation() + dSat);
      
       spread.push_back(nextColor);

       lastColor = nextColor;
    }

} // void linearSpread

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgColor::set (const std::string& name, int setComponentsFlag)
//
// Set my name to the specific name. If setComponentsFlag is true, then
// set me to the named color. If the named color is not in #rrggbb format,
// set my rgb components to undefColor.
//
{
   name_ = name;
   if (!setComponentsFlag) return;

   unsigned int red, green, blue;
   const char* colStr = name.c_str();
   if (name.length() == 7 && colStr[0] == '#')
   {
      sscanf(colStr, "%*c%2x%2x%2x", &red, &green, &blue);

      red_   = red   / 255.0;
      green_ = green / 255.0;
      blue_  = blue  / 255.0;
   }
   else
   {
      red_ = green_ = blue_ = undefColor;
   }

} // void DgColor::set

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgColor::set (float r, float g, float b)
//
// Set me to the indicated rgb color.
//
{
   // truncate the values

   if (r > 1.0) r = 1.0; else if (r < 0.0) r = 0.0;
   if (g > 1.0) g = 1.0; else if (g < 0.0) g = 0.0;
   if (b > 1.0) b = 1.0; else if (b < 0.0) b = 0.0;

   // set my rgb values 

   red_   = r;
   green_ = g;
   blue_  = b;

   // build a hexadecimal color specification string

   const unsigned int maxColStr = 8;
   char colStr[maxColStr];
   snprintf(colStr, maxColStr, "#%02x%02x%02x", (int) dgg::util::lrint(r * 255.0), 
           (int) dgg::util::lrint(g * 255.0), (int) dgg::util::lrint(b * 255.0));

   name_ = colStr;

} // void DgColor::set

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgColor::set (float cyan, float magenta, float yellow, float black)
//
// Set me to the indicated cmyk color.
//
{
  set((1.0 - cyan + black), (1.0 - magenta + black), (1.0 - yellow + black));

} // void DgColor::set

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static float 
hlsHelper (float n1, float n2, float hue)
//
// See Foley & VanDam 2nd edition p. 596
//
{

   while (hue > 360.0) hue -= 360.0;
   while (hue < 0.0) hue += 360.0;

   if (hue < 60.0)
   {
      return n1 + (n2 - n1) * hue / 60.0;
   }
   else if (hue < 180.0)
   {
      return n2;
   }
   else if (hue < 240.0)
   {
      return n1 + (n2 - n1) * (240.0 - hue) / 60.0;
   }
   else
   {
      return n1;
   }

} // float hlsHelper

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgColor::setHLS (float hue, float lightness, float saturation)
//
// Set me to the indicated hls color.
//
// See Foley & VanDam 2nd edition p. 596
//
{
   float red, green, blue, m1, m2;

   if (saturation == 0.0)  // achromatic/grey: hue is irrelevant
   {
      red = green = blue = lightness;
   }
   else
   {
      if (lightness <= 0.5)
      {
         m2 = lightness * (1.0 + saturation);
      }
      else
      {
         m2 = lightness + saturation - lightness * saturation;
      }

      m1 = 2.0 * lightness - m2;

      red = hlsHelper(m1, m2, hue + 120.0);
      green = hlsHelper(m1, m2, hue);
      blue = hlsHelper(m1, m2, hue - 120.0);
   }

   set(red, green, blue);

} // void DgColor::setHLS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// See Foley & VanDam 2nd edition p. 595
//
void
DgColor::hls (float* hue, float* lightness, float* saturation) const
{
   float max = max3(red(), green(), blue());
   float min = min3(red(), green(), blue());

   *lightness = (max + min) / 2.0;

   if (max == min)  // achromatic/grey: hue doesn't matter
   {
      *saturation = 0.0;
      *hue = 0.0;
   }
   else
   {
      // calculate saturation

      float delta = max - min;
      if (*lightness <= 0.0)
      {
         *saturation = delta / (max + min);
      }
      else
      {
         *saturation = delta / (2.0 - delta);
      }

      // calculate hue

      if (red() == max)
      {
         *hue = (green() - blue()) / delta;
      }
      else if (green() == max)
      {
         *hue = 2.0 + (blue() - red()) / delta;
      }
      else // blue had better be max
      {
         *hue = 4.0 + (red() - green()) / delta;
      }

      *hue *= 60.0;

      while (*hue > 360.0) *hue -= 360.0;
      while (*hue < 0.0) *hue += 360.0;
   }

} // void DgColor::hls

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::hue (void) const
{
   float hue, lightness, saturation;

   hls(&hue, &lightness, &saturation);

   return hue;

} // float DgColor::hue

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::lightness (void) const
{
   float hue, lightness, saturation;

   hls(&hue, &lightness, &saturation);

   return lightness;

} // float DgColor::lightness

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::saturation (void) const
{
   float hue, lightness, saturation;

   hls(&hue, &lightness, &saturation);

   return saturation;

} // float DgColor::saturation

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void 
DgColor::cmyk (float* cyan, float* magenta, float* yellow, float* black) const
{
   *cyan    = 1.0 - red();
   *magenta = 1.0 - green();
   *yellow  = 1.0 - blue();

   *black = min3(*cyan, *magenta, *yellow);

   *cyan    -= *black;
   *magenta -= *black;
   *yellow  -= *black;

} // DgColor::cmyk

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::cyan (void) const
{
   float c, m, y, k;

   cmyk(&c, &m, &y, &k);

   return c;

} // float DgColor::cyan

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::magenta (void) const
{
   float c, m, y, k;

   cmyk(&c, &m, &y, &k);

   return m;

} // float DgColor::magenta

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::yellow (void) const
{
   float c, m, y, k;

   cmyk(&c, &m, &y, &k);

   return y;

} // float DgColor::yellow

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float 
DgColor::black (void) const
{
   float c, m, y, k;

   cmyk(&c, &m, &y, &k);

   return k;

} // float DgColor::black

////////////////////////////////////////////////////////////////////////////////
