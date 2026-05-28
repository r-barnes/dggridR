//
// Rstreambuf.h: dggridR override of Rcpp's Rstreambuf.h
//
// The upstream <Rcpp/iostream/Rstreambuf.h> places two file-scope
//
//     static Rostream<true>   Rcout;
//     static Rostream<false>  Rcerr;
//
// (or `extern` references when RCPP_USE_GLOBAL_ROSTREAM is defined) into
// every translation unit that includes <Rcpp.h>.  Each static Rostream
// instance is a `std::ostream`-derived object: its constructor runs at
// shared-library load time, BEFORE R has finished initialising the .so
// and before `R_init_dggridR` is invoked.  On the CRAN pretest Debian
// box (R-devel built with gcc-16 + libstdc++-16 + -D_FORTIFY_SOURCE=3)
// that constructor has been observed to segfault inside libstdc++'s
// locale / `std::basic_ios::init` initialisation, aborting `dyn.load()`
// with `address (nil), cause 'memory not mapped'`.  The R Extensions
// manual (§5.6) explicitly warns against using C++ I/O in shared
// libraries loaded by R for exactly this class of reason.
//
// dggridR routes all R-aware console output through `dgcout` / `dgcerr`,
// which are backed by `Rprintf` / `REprintf` via a tiny `std::streambuf`
// subclass in `DgBase.cpp`.  We never use `Rcpp::Rcout` or `Rcpp::Rcerr`.
// This override therefore drops the static Rostream instances entirely
// while keeping the `Rcpp::Rostream` / `Rcpp::Rstreambuf` template
// declarations that `Rcpp/routines.h` requires for the (unused) inline
// `Rcpp_cout_get()` / `Rcpp_cerr_get()` callable lookups.
//
// The file is included via `src/Makevars` `PKG_CPPFLAGS = -Idggridr_overrides`
// so that this version is found by the C preprocessor before the version
// shipped by the Rcpp package.  It uses the same `RCPP__IOSTREAM__RSTREAMBUF_H`
// include guard as the upstream file, so the upstream version is never
// re-included even if Rcpp's own include order should change.
//

#ifndef RCPP__IOSTREAM__RSTREAMBUF_H
#define RCPP__IOSTREAM__RSTREAMBUF_H

#include <cstdio>
#include <streambuf>

namespace Rcpp {

    template <bool OUTPUT>
    class Rstreambuf : public std::streambuf {
    public:
        Rstreambuf() {}

    protected:
        virtual std::streamsize xsputn(const char *s, std::streamsize n);
        virtual int overflow(int c = traits_type::eof());
        virtual int sync();
    };

    template <bool OUTPUT>
    class Rostream : public std::ostream {
        typedef Rstreambuf<OUTPUT> Buffer;
        Buffer buf;
    public:
        Rostream() : std::ostream( &buf ) {}
    };
                                                                // #nocov start
    template <> inline std::streamsize Rstreambuf<true>::xsputn(const char *s, std::streamsize n) {
        Rprintf("%.*s", static_cast<int>(n), s);
        return n;
    }
    template <> inline std::streamsize Rstreambuf<false>::xsputn(const char *s, std::streamsize n) {
        REprintf("%.*s", static_cast<int>(n), s);
        return n;
    }

    template <> inline int Rstreambuf<true>::overflow(int c) {
        if (c != traits_type::eof()) {
            char ch = static_cast<char>(c);
            return xsputn(&ch, 1) == 1 ? c : traits_type::eof();
        }
        return c;
    }
    template <> inline int Rstreambuf<false>::overflow(int c) {
        if (c != traits_type::eof()) {
            char ch = static_cast<char>(c);
            return xsputn(&ch, 1) == 1 ? c : traits_type::eof();
        }
        return c;
    }

    template <> inline int Rstreambuf<true>::sync() {
        ::R_FlushConsole();
        return 0;
    }
    template <> inline int Rstreambuf<false>::sync() {
        ::R_FlushConsole();
        return 0;
    }                                                           // #nocov end

    // Intentionally omitted relative to upstream Rcpp:
    //
    //     #ifdef RCPP_USE_GLOBAL_ROSTREAM
    //         extern Rostream<true>&  Rcout;
    //         extern Rostream<false>& Rcerr;
    //     #else
    //         static Rostream<true>   Rcout;
    //         static Rostream<false>  Rcerr;
    //     #endif
    //
    // dggridR does not reference `Rcpp::Rcout` / `Rcpp::Rcerr` anywhere
    // (all R-aware output goes through `dgcout` / `dgcerr`, backed by
    // `Rprintf` / `REprintf`).  Dropping the declarations here means no
    // `std::ostream`-derived object is constructed at static-init / dyn.load
    // time in any of the package's translation units, eliminating an entire
    // class of locale/iostream-init crashes inside the dynamic loader.

}

#endif
