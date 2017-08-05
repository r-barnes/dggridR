#!/usr/bin/env python3

proj_arg = [
  ("long double", "pole_lon_deg"),
  ("long double", "pole_lat_deg"),
  ("long double", "azimuth_deg"),
  ("unsigned int", "aperture"),
  ("int", "res"),
  ("std::string", "topology"),
  ("std::string", "projection"),
]

in_arg = [
  ("GEO",     (("long double", "lon_deg"), ("long double", "lat_deg"))),
  ("PROJTRI", (("uint64_t",    "tnum"),    ("long double", "tx"), ("long double", "ty"))),
  ("Q2DD",    (("uint64_t",    "quad"),    ("long double", "qx"), ("long double", "qy"))),
  ("Q2DI",    (("uint64_t",    "quad"),    ("long double", "i"), ("long double", "j"))),
  ("SEQNUM",  (("uint64_t",    "seqnum"),))
]

out_arg = [
  ("GEO",     ( ("long double",  "lon_deg"), ("long double", "lat_deg"))),
  ("PROJTRI", ( ("uint64_t",     "tnum"),    ("long double", "tx"), ("long double", "ty"))),
  ("Q2DD",    ( ("uint64_t",     "quad"),    ("long double", "qx"), ("long double", "qy"))),
  ("Q2DI",    ( ("uint64_t",     "quad"),    ("long double", "i"), ("long double", "j"))),
  ("SEQNUM",  ( ("uint64_t",     "seqnum"),)),
  ("PLANE",   ( ("long double",  "px"),       ("long double", "py")))
]

sig = "void {intype}_to_{outtype}({typed_proj_args}, {in_args}, {out_args});"

ftemplate = """void {intype}_to_{outtype}({typed_proj_args}, unsigned int N, {in_args}, {out_args}){{
  dglib::DgTransformer dgt({proj_args});

  for(unsigned int i=0;i<{size};i++){{
    auto in = dgt.in{intype}({ini});
    dgt.out{outtype}(in, {outi});
  }}
}}
"""

codes = [sig,ftemplate]

for c in codes:
  print("")
  for i in in_arg:
    for o in out_arg:
      sintype          = i[0]
      souttype         = o[0]
      styped_proj_args = ', '.join(['{0} {1}'.format(*i) for i in proj_arg])
      sproj_args       = ', '.join(['{1}'.format(*i) for i in proj_arg])
      sin_args         = ', '.join(['const {0} *const in_{1}'.format(ia[0],ia[1]) for ia in i[1]])
      sout_args        = ', '.join(['{0} *const out_{1}'.format(oa[0],oa[1]) for oa in o[1]])
      sini             = ', '.join(['in_'+ia[1]+'[i]' for ia in i[1]])
      souti            = ', '.join(['out_'+oa[1]+'[i]' for oa in o[1]])
      print(c.format(
        intype          = sintype,
        outtype         = souttype,
        typed_proj_args = styped_proj_args,
        proj_args       = sproj_args,
        in_args         = sin_args,
        out_args        = sout_args,
        size            = 'N',
        ini             = sini,
        outi            = souti
      ))



# void DgGEO_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, const std::vector<long double> &lon_deg, const std::vector<long double> &lat_deg, std::vector<uint64_t> &seqnum){
#   dglib::DgTransformer dgt(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection);

#   for(unsigned int i=0;i<lon_deg.size();i++){
#     auto in = dgt.inGEO(lon_deg.at(i),lat_deg.at(i));
#     dgt.outSEQNUM(in, seqnum.at(i));
#   }
# }