#include <R.h>
#include <Rinternals.h>
#include <stdint.h>
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

static int is_little_endian(void) {
  const uint16_t value = 1;
  return *((const unsigned char *)&value) == 1;
}

static void write_uint32_native(unsigned char *dest, uint32_t value) {
  memcpy(dest, &value, sizeof(uint32_t));
}

static void write_double_native(unsigned char *dest, double value) {
  memcpy(dest, &value, sizeof(double));
}

SEXP dg_process_polydata_native(SEXP polydata, SEXP n_cells_sexp) {
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
    SEXP out_wkb = PROTECT(Rf_allocVector(VECSXP, 0));
    ++nprotect;
    SEXP names = PROTECT(Rf_allocVector(STRSXP, 2));
    ++nprotect;

    SET_STRING_ELT(names, 0, Rf_mkChar("seqnum"));
    SET_STRING_ELT(names, 1, Rf_mkChar("wkb"));
    SET_VECTOR_ELT(out, 0, out_seq);
    SET_VECTOR_ELT(out, 1, out_wkb);
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
  SEXP out_wkb = PROTECT(Rf_allocVector(VECSXP, n_cells));
  ++nprotect;
  const unsigned char byte_order = is_little_endian() ? 1 : 0;

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

    const R_xlen_t wkb_bytes = 13 + (16 * n_points);
    SEXP raw = PROTECT(Rf_allocVector(RAWSXP, wkb_bytes));
    unsigned char *buf = RAW(raw);
    R_xlen_t pos = 0;

    buf[pos++] = byte_order;
    write_uint32_native(buf + pos, (uint32_t)3); /* WKB Polygon */
    pos += 4;
    write_uint32_native(buf + pos, (uint32_t)1); /* one ring */
    pos += 4;
    write_uint32_native(buf + pos, (uint32_t)n_points);
    pos += 4;

    for (R_xlen_t j = 0; j < count; ++j) {
      const R_xlen_t idx = start + j;
      write_double_native(buf + pos, x_ptr[idx]);
      pos += 8;
      write_double_native(buf + pos, y_ptr[idx]);
      pos += 8;
    }

    if (!closed) {
      write_double_native(buf + pos, first_x);
      pos += 8;
      write_double_native(buf + pos, first_y);
      pos += 8;
    }

    SET_VECTOR_ELT(out_wkb, g, raw);
    REAL(out_seq)[g] = seq_values[g];
    UNPROTECT(1); /* raw */
  }

  SEXP out = PROTECT(Rf_allocVector(VECSXP, 2));
  ++nprotect;
  SEXP names = PROTECT(Rf_allocVector(STRSXP, 2));
  ++nprotect;

  SET_STRING_ELT(names, 0, Rf_mkChar("seqnum"));
  SET_STRING_ELT(names, 1, Rf_mkChar("wkb"));
  SET_VECTOR_ELT(out, 0, out_seq);
  SET_VECTOR_ELT(out, 1, out_wkb);
  Rf_setAttrib(out, R_NamesSymbol, names);

  R_Free(seq_values);
  R_Free(counts);
  R_Free(starts);
  UNPROTECT(nprotect);
  return out;
}
