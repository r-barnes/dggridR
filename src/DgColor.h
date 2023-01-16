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
// DgColor.h: DgColor class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCOLOR_H
#define DGCOLOR_H

#include <cmath>
#include <vector>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class DgColor {

   public:

      static const float undefColor;

      static void linearSpreadRGB (const DgColor& col1, const DgColor& col2, 
                                   std::vector<DgColor*>& spread, int steps);

      static void linearSpreadHLS (const DgColor& col1, const DgColor& col2, 
                                   std::vector<DgColor*>& spread, int steps);

      // constructors

      DgColor (void) 
         : red_ (0.0), green_ (0.0), blue_ (0.0) 
         { static const std::string defName("#000000"); name_ = defName; }

      DgColor (const std::string& name) { set(name); }

      DgColor (const DgColor& col) { operator=(col); }

      DgColor (const float* rgbVec)                 { set(rgbVec); }
      DgColor (const short* rgbVec)                 { set(rgbVec); }
      DgColor (float red, float green, float blue)  { set(red, green, blue); }

      DgColor (float cyan, float magenta, float yellow, float black)  
                                     { set(cyan, magenta, yellow, black); }

      // set functions

      void setRed   (float red)   { set(red,  green_, blue_); }
      void setGreen (float green) { set(red_, green,  blue_); }
      void setBlue  (float blue)  { set(red_, green_, blue); }

      void set (void) { name_ = "#000000"; red_ = green_ = blue_ = 0.0; }

      void set (const std::string& name, int setComponentsFlag = true);

      void set (const float* rgbVec) { set(rgbVec[0], rgbVec[1], rgbVec[2]); }
      void set (const short* rgbVec) { set(rgbVec[0], rgbVec[1], rgbVec[2]); }

      void set (float red, float green, float blue);

      void set (float cyan, float magenta, float yellow, float black);  

      void setHLS (float hue, float lightness = 0.5, float saturation = 1.0);

      DgColor& operator= (const DgColor& color) 
           { name_ = color.name(); red_ = color.red(); green_ = color.green();
             blue_ = color.blue(); return *this; }

      // get functions

      const std::string& name (void) const { return name_; }

      // RGB

      float red   (void) const { return red_; }
      float green (void) const { return green_; }
      float blue  (void) const { return blue_; }

      // CMYK

      void cmyk (float* cyan, float* magenta, float* yellow, 
                                               float* black) const;

      float cyan    (void) const;
      float magenta (void) const;
      float yellow  (void) const;
      float black   (void) const;

      // HLS

      void hls (float* hue, float* lightness, float* saturation) const;

      float hue        (void) const;
      float lightness  (void) const;
      float saturation (void) const;

   private:

      std::string name_;

      float red_;
      float green_;
      float blue_;

};

////////////////////////////////////////////////////////////////////////////////
inline bool operator== (const DgColor& color1, const DgColor& color2) 
             { return (color1.name() == color2.name()); }

inline bool operator!= (const DgColor& color1, const DgColor& color2) 
             { return !(color1 == color2); }

inline ostream& operator<< (ostream& stream, const DgColor& color)
             { return stream << color.name(); }

////////////////////////////////////////////////////////////////////////////////

class DgColorEq {

   public:

      explicit DgColorEq(const DgColor& key) : key_(key) {}
      bool operator() (const DgColor* d) const
               { return *d == key_; }

   private:

      DgColor key_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
