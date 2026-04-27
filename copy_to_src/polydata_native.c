#include <R.h>
#include <Rinternals.h>
#include <string.h>

static SEXP get_list_element(SEXP x, const char *name) {
  SEXP names = getAttrib(x, R_NamesSymbol);
  if (TYPEOF(names) != STRSXP) {
    return R_NilValue;
  }
  for (R_xlen_t i = 0; i < XLENGTH(x); ++i) {
    if (strcmp(CHAR(STRING_ELT(names, i)), name) == 0) {
      return VECTOR_ELT(x, i);
    }
  }
  return R_NilValue;
}

static SEXP make_character_vector2(const char *x, const char *y) {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, 2));
  SET_STRING_ELT(out, 0, Rf_mkChar(x));
  SET_STRING_ELT(out, 1, Rf_mkChar(y));
  UNPROTECT(1);
  return out;
}

static SEXP make_character_vector3(const char *x, const char *y, const char *z) {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, 3));
  SET_STRING_ELT(out, 0, Rf_mkChar(x));
  SET_STRING_ELT(out, 1, Rf_mkChar(y));
  SET_STRING_ELT(out, 2, Rf_mkChar(z));
  UNPROTECT(1);
  return out;
}

static void set_bbox(SEXP geometry, double xmin, double ymin, double xmax, double ymax) {
  SEXP bbox = PROTECT(Rf_allocVector(REALSXP, 4));
  SEXP bbox_names = PROTECT(Rf_allocVector(STRSXP, 4));
  SEXP bbox_class = PROTECT(Rf_mkString("bbox"));

  REAL(bbox)[0] = xmin;
  REAL(bbox)[1] = ymin;
  REAL(bbox)[2] = xmax;
  REAL(bbox)[3] = ymax;

  SET_STRING_ELT(bbox_names, 0, Rf_mkChar("xmin"));
  SET_STRING_ELT(bbox_names, 1, Rf_mkChar("ymin"));
  SET_STRING_ELT(bbox_names, 2, Rf_mkChar("xmax"));
  SET_STRING_ELT(bbox_names, 3, Rf_mkChar("ymax"));

  Rf_setAttrib(bbox, R_NamesSymbol, bbox_names);
  Rf_setAttrib(bbox, R_ClassSymbol, bbox_class);
  Rf_setAttrib(geometry, Rf_install("bbox"), bbox);

  UNPROTECT(3);
}

static void set_sfc_attributes(
    SEXP geometry,
    SEXP sfc_class,
    SEXP crs,
    double xmin,
    double ymin,
    double xmax,
    double ymax
) {
  SEXP precision = PROTECT(Rf_ScalarReal(0.0));
  SEXP n_empty = PROTECT(Rf_ScalarInteger(0));

  Rf_setAttrib(geometry, R_ClassSymbol, sfc_class);
  Rf_setAttrib(geometry, Rf_install("precision"), precision);
  Rf_setAttrib(geometry, Rf_install("crs"), crs);
  Rf_setAttrib(geometry, Rf_install("n_empty"), n_empty);
  set_bbox(geometry, xmin, ymin, xmax, ymax);

  UNPROTECT(2);
}

SEXP dg_process_polydata_native(SEXP polydata, SEXP n_cells_sexp, SEXP crs) {
  int nprotect = 0;

  if (TYPEOF(polydata) != VECSXP) {
    Rf_error("'polydata' must be a list with x, y, and seqnum vectors.");
  }

  SEXP x = get_list_element(polydata, "x");
  SEXP y = get_list_element(polydata, "y");
  SEXP seqnum = get_list_element(polydata, "seqnum");

  if (x == R_NilValue || y == R_NilValue || seqnum == R_NilValue) {
    Rf_error("'polydata' must contain named components 'x', 'y', and 'seqnum'.");
  }

  if (TYPEOF(x) != REALSXP || TYPEOF(y) != REALSXP || TYPEOF(seqnum) != REALSXP) {
    Rf_error("'x', 'y', and 'seqnum' must be numeric vectors.");
  }

  const R_xlen_t n = XLENGTH(x);
  if (XLENGTH(y) != n || XLENGTH(seqnum) != n) {
    Rf_error("'x', 'y', and 'seqnum' must have the same length.");
  }

  if (n == 0) {
    SEXP out = PROTECT(Rf_allocVector(VECSXP, 2));
    ++nprotect;
    SEXP out_seq = PROTECT(Rf_allocVector(REALSXP, 0));
    ++nprotect;
    SEXP geometry = PROTECT(Rf_allocVector(VECSXP, 0));
    ++nprotect;
    SEXP names = PROTECT(Rf_allocVector(STRSXP, 2));
    ++nprotect;
    SEXP sfc_class = PROTECT(make_character_vector2("sfc_POLYGON", "sfc"));
    ++nprotect;

    SET_STRING_ELT(names, 0, Rf_mkChar("seqnum"));
    SET_STRING_ELT(names, 1, Rf_mkChar("geometry"));
    set_sfc_attributes(geometry, sfc_class, crs, NA_REAL, NA_REAL, NA_REAL, NA_REAL);
    SET_VECTOR_ELT(out, 0, out_seq);
    SET_VECTOR_ELT(out, 1, geometry);
    Rf_setAttrib(out, R_NamesSymbol, names);

    UNPROTECT(nprotect);
    return out;
  }

  const R_xlen_t n_cells = (R_xlen_t)Rf_asInteger(n_cells_sexp);
  const double *x_ptr = REAL(x);
  const double *y_ptr = REAL(y);
  const double *seq_ptr = REAL(seqnum);

  R_xlen_t *starts = R_Calloc((size_t)n_cells, R_xlen_t);
  R_xlen_t *counts = R_Calloc((size_t)n_cells, R_xlen_t);
  double *seq_values = R_Calloc((size_t)n_cells, double);

  if (seq_ptr[0] == 1.0 && seq_ptr[n - 1] == (double)n_cells) {
    for (R_xlen_t i = 0; i < n; ++i) {
      counts[(R_xlen_t)seq_ptr[i] - 1] += 1;
    }
    starts[0] = 0;
    seq_values[0] = 1.0;
    for (R_xlen_t g = 1; g < n_cells; ++g) {
      starts[g] = starts[g - 1] + counts[g - 1];
      seq_values[g] = (double)(g + 1);
    }
  } else {
    R_xlen_t g = 0;
    starts[0] = 0;
    counts[0] = 1;
    seq_values[0] = seq_ptr[0];
    for (R_xlen_t i = 1; i < n; ++i) {
      if (seq_ptr[i] == seq_values[g]) {
        counts[g] += 1;
      } else {
        ++g;
        starts[g] = i;
        counts[g] = 1;
        seq_values[g] = seq_ptr[i];
      }
    }
  }

  SEXP out_seq = PROTECT(Rf_allocVector(REALSXP, n_cells));
  ++nprotect;
  SEXP geometry = PROTECT(Rf_allocVector(VECSXP, n_cells));
  ++nprotect;
  SEXP sfg_class = PROTECT(make_character_vector3("XY", "POLYGON", "sfg"));
  ++nprotect;
  SEXP sfc_class = PROTECT(make_character_vector2("sfc_POLYGON", "sfc"));
  ++nprotect;

  double xmin = x_ptr[0];
  double ymin = y_ptr[0];
  double xmax = x_ptr[0];
  double ymax = y_ptr[0];

  for (R_xlen_t g = 0; g < n_cells; ++g) {
    const R_xlen_t start = starts[g];
    const R_xlen_t count = counts[g];
    const R_xlen_t last = start + count - 1;
    const double first_x = x_ptr[start];
    const double first_y = y_ptr[start];
    const double last_x = x_ptr[last];
    const double last_y = y_ptr[last];
    const int closed = (first_x == last_x) && (first_y == last_y);
    const R_xlen_t n_points = closed ? count : (count + 1);

    SEXP ring = PROTECT(Rf_allocMatrix(REALSXP, n_points, 2));
    SEXP polygon = PROTECT(Rf_allocVector(VECSXP, 1));
    double *ring_ptr = REAL(ring);

    for (R_xlen_t j = 0; j < count; ++j) {
      const R_xlen_t idx = start + j;
      const double x_val = x_ptr[idx];
      const double y_val = y_ptr[idx];

      ring_ptr[j] = x_val;
      ring_ptr[j + n_points] = y_val;

      if (x_val < xmin) xmin = x_val;
      if (y_val < ymin) ymin = y_val;
      if (x_val > xmax) xmax = x_val;
      if (y_val > ymax) ymax = y_val;
    }

    if (!closed) {
      ring_ptr[n_points - 1] = first_x;
      ring_ptr[(2 * n_points) - 1] = first_y;
    }

    SET_VECTOR_ELT(polygon, 0, ring);
    Rf_setAttrib(polygon, R_ClassSymbol, sfg_class);
    SET_VECTOR_ELT(geometry, g, polygon);
    REAL(out_seq)[g] = seq_values[g];
    UNPROTECT(2); /* ring, polygon */
  }

  set_sfc_attributes(geometry, sfc_class, crs, xmin, ymin, xmax, ymax);

  SEXP out = PROTECT(Rf_allocVector(VECSXP, 2));
  ++nprotect;
  SEXP names = PROTECT(Rf_allocVector(STRSXP, 2));
  ++nprotect;

  SET_STRING_ELT(names, 0, Rf_mkChar("seqnum"));
  SET_STRING_ELT(names, 1, Rf_mkChar("geometry"));
  SET_VECTOR_ELT(out, 0, out_seq);
  SET_VECTOR_ELT(out, 1, geometry);
  Rf_setAttrib(out, R_NamesSymbol, names);

  R_Free(seq_values);
  R_Free(counts);
  R_Free(starts);
  UNPROTECT(nprotect);
  return out;
}
