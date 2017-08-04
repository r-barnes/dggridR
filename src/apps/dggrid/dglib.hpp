#ifndef _dglib_hpp_
#define _dglib_hpp_

namespace dglib {

  enum class InType {
    GEO,
    PROJTRI,
    Q2DD,
    Q2DI,
    SEQNUM
  };
  //VERTEX2DD,

  enum class OutType {
    GEO,
    PROJTRI,
    Q2DD,
    PLANE,
    Q2DI,
    SEQNUM
  };
  //VERTEX2DD,
  //INTERLEAVE,

  void DgTransform (
     long double  pole_lon_deg,
     long double  pole_lat_deg,
     long double  azimuth_deg,
     unsigned int aperture,
     int          res,
     std::string topology, //"HEXAGON", "DIAMOND", "TRIANGLE"
     std::string projection, //ISEA/FULLER
     InType       in_type,
     const double *const in1,
     const double *const in2,
     const double *const in3,
     OutType      out_type,
     double       *const out1,
     double       *const out2,
     double       *const out3,
     const unsigned int N
  );

}

#endif