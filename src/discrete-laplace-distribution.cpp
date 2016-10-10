#include <Rcpp.h>
#include "shared.h"

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using std::sin;
using std::cos;
using std::tan;
using std::atan;
using Rcpp::IntegerVector;
using Rcpp::NumericVector;
using Rcpp::NumericMatrix;


double pmf_dlaplace(double x, double p, double mu) {
  if (ISNAN(x) || ISNAN(p) || ISNAN(mu))
    return NA_REAL;
  if (p <= 0.0 || p >= 1.0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (!isInteger(x))
    return 0.0;
  return (1.0-p)/(1.0+p) * pow(p, abs(x-mu));
} 

double cdf_dlaplace(double x, double p, double mu) {
  if (ISNAN(x) || ISNAN(p) || ISNAN(mu))
    return NA_REAL;
  if (p <= 0.0 || p >= 1.0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < 0.0)
    return pow(p, -floor(x-mu))/(1.0+p);
  else
    return 1.0 - (pow(p, floor(x-mu)+1.0)/(1.0+p));
} 

double rng_dlaplace(double p, double mu) {
  if (ISNAN(p) || ISNAN(mu))
    return NA_REAL;
  if (p <= 0.0 || p >= 1.0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  double q, u, v;
  q = 1.0 - p;
  u = R::rgeom(q); 
  v = R::rgeom(q); 
  return u-v + mu;
} 


// [[Rcpp::export]]
NumericVector cpp_ddlaplace(
    const NumericVector& x,
    const NumericVector& scale,
    const NumericVector& location,
    bool log_prob = false
) {
  
  int n = x.length();
  int ns = scale.length();
  int nl = location.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, ns, nl));
  NumericVector p(Nmax);
  
  for (int i = 0; i < Nmax; i++)
    p[i] = pmf_dlaplace(x[i % n], scale[i % ns], location[i % nl]);
  
  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);
  
  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pdlaplace(
    const NumericVector& x,
    const NumericVector& scale,
    const NumericVector& location,
    bool lower_tail = true, bool log_prob = false
) {
  
  int n = x.length();
  int ns = scale.length();
  int nl = location.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, ns, nl));
  NumericVector p(Nmax);
  
  for (int i = 0; i < Nmax; i++)
    p[i] = cdf_dlaplace(x[i % n], scale[i % ns], location[i % nl]);
  
  if (!lower_tail)
    for (int i = 0; i < Nmax; i++)
      p[i] = 1.0 - p[i];
  
  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);
  
  return p;
}


// [[Rcpp::export]]
NumericVector cpp_rdlaplace(
    const int n,
    const NumericVector& scale,
    const NumericVector& location
) {
  
  int ns = scale.length();
  int nl = location.length();
  NumericVector x(n);
  
  for (int i = 0; i < n; i++)
    x[i] = rng_dlaplace(scale[i % ns], location[i % nl]);
  
  return x;
}
