#!/usr/bin/env python3

proj_arg = [
  ("long double", "pole_lon_deg", ""),
  ("long double", "pole_lat_deg"),
  ("long double", "azimuth_deg"),
  ("unsigned int", "aperture"),
  ("int", "res"),
  ("std::string", "topology"),
  ("std::string", "projection")
]

in_arg = [
  ("GEO",     (("long double", "lon_deg", "Vector of longitude, in degrees"), ("long double", "lat_deg", "Vector of latitude, in degrees"))),
  ("PROJTRI", (("uint64_t",    "tnum", "Vector of triangle numbers"),    ("long double", "tx", "Vector of triangle x values"), ("long double", "ty", "Vector of triangle y values"))),
  ("Q2DD",    (("uint64_t",    "quad", "Vector of quad numbers"),    ("long double", "qx", "Vector of quadrant x values"), ("long double", "qy", "Vector of quadrant y values"))),
  ("Q2DI",    (("uint64_t",    "quad", "Vector of quad numbers"),    ("long double", "i", "Vector of quadrant i values"), ("long double", "j", "Vector of quadrant j values"))),
  ("SEQNUM",  (("uint64_t",    "seqnum", "Globally unique number identifying the surface polygon"),))
]

out_arg = [
  ("GEO",     (("long double", "lon_deg", "Vector of longitude, in degrees"), ("long double", "lat_deg", "Vector of latitude, in degrees"))),
  ("PROJTRI", (("uint64_t",    "tnum", "Vector of triangle numbers"),    ("long double", "tx", "Vector of triangle x values"), ("long double", "ty", "Vector of triangle y values"))),
  ("Q2DD",    (("uint64_t",    "quad", "Vector of quad numbers"),    ("long double", "qx", "Vector of quadrant x values"), ("long double", "qy", "Vector of quadrant y values"))),
  ("Q2DI",    (("uint64_t",    "quad", "Vector of quad numbers"),    ("long double", "i", "Vector of quadrant i values"), ("long double", "j", "Vector of quadrant j values"))),
  ("SEQNUM",  (("uint64_t",    "seqnum", "Globally unique number identifying the surface polygon"),)),
  ("PLANE",   ( ("long double",  "px", "Vector of plane X values"),       ("long double", "py", "Vector of plane Y values")))
]

sig = """void {intype}_to_{outtype}({typed_proj_args}, {in_args}, {out_args});
"""

#// [[Rcpp::export]]
ftemplate = """
void {intype}_to_{outtype}({typed_proj_args}, unsigned int N, {in_args}, {out_args}){{
  dglib::Transformer dgt({proj_args});

  for(unsigned int i=0;i<{size};i++){{
{translatein}
{translateout}
    auto in = dgt.in{intype}({ini});
    dgt.out{outtype}(in, {outi});
{backtranslate}
  }}
}}
"""

rcode = """
#' @name {intype}_to_{outtype}
#' 
#' @title      Convert from {intype} to {outtype}
#'
#' @description
#'             Uses a discrete global grid system to convert between {intype}
#'             and {outtype} (see below for details)
#' 
{Rparams}
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' {intype}_to_{outtype}(dggs, {Rin})
#'
#' @export 
dg{intype}_to_{outtype} <- function(dggs, {Rin}){{
  dgverify(dggs)

  N <- length({Rin1})
{init_out}

  dggridR:::{intype}_to_{outtype}({dgproj_args}, N, {Rin}, {Rout})

  list(
{Routlist}
  )
}}
"""

codes = [
  ('../cgen_head.h',sig),
  ('../cgen_body.h',ftemplate),
  ('../../R/cwrapper.R',rcode)
]

for c in codes:
  fout = open(c[0],'w')
  for i in in_arg:
    for o in out_arg:
      outstr           = c[1].format(
        intype          = i[0],
        outtype         = o[0],
        typed_proj_args = ', '.join(['const {0} {1}'.format(*i) for i in proj_arg]),
        proj_args       = ', '.join(['{1}'.format(*i) for i in proj_arg]),
        dgproj_args     = ', '.join(['dggs[["{1}"]]'.format(*i) for i in proj_arg]),
        translatein     = '\n'.join(['    const {0} tin_{1} = in_{1}[i];'.format(ia[0],ia[1]) for ia in i[1]]),
        translateout    = '\n'.join(['    {0} tout_{1} = out_{1}[i];'.format(oa[0],oa[1]) for oa in o[1]]),
        backtranslate   = '\n'.join(['    out_{1}[i] = tout_{1};'.format(oa[0],oa[1]) for oa in o[1]]),
        ini             = ', '.join(['tin_'+ia[1] for ia in i[1]]),
        outi            = ', '.join(['tout_'+oa[1] for oa in o[1]]),
        in_args         = ', '.join(['Rcpp::NumericVector in_{1}'.format(ia[0],ia[1]) for ia in i[1]]),
        out_args        = ', '.join(['Rcpp::NumericVector out_{1}'.format(oa[0],oa[1]) for oa in o[1]]),
        size            = 'N',
        Rparams         = '\n'.join(["#' @param {0}  {1}".format(ia[1],ia[2]) for ia in i[1]]),
        Rin             = ', '.join('in_'+ia[1] for ia in i[1]),
        Rout            = ', '.join('out_'+oa[1] for oa in o[1]),
        init_out        = '\n'.join(['  out_{0} <- rep(0,N)'.format(oa[1]) for oa in o[1]]),
        Rin1            = 'in_'+i[1][0][1],
        Routlist        = ',\n'.join(['    {0} = out_{0}'.format(oa[1]) for oa in o[1]])
      )
      fout.write(outstr)

fout_funcs = open('../cgen_funcs.h','w')
fout_funcs.write('RCPP_MODULE(dgfuncs){\n')
for i in in_arg:
  for o in out_arg:
    fout_funcs.write('  Rcpp::function("{0}_to_{1}", &{0}_to_{1});\n'.format(i[0],o[0]))
fout_funcs.write("}")

# void DgGEO_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, const std::vector<long double> &lon_deg, const std::vector<long double> &lat_deg, std::vector<uint64_t> &seqnum){
#   dglib::Transformer dgt(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection);

#   for(unsigned int i=0;i<lon_deg.size();i++){
#     auto in = dgt.inGEO(lon_deg.at(i),lat_deg.at(i));
#     dgt.outSEQNUM(in, seqnum.at(i));
#   }
# }